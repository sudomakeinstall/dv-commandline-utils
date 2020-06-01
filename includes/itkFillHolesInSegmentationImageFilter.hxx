/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkFillHolesInSegmentationImageFilter_hxx
#define itkFillHolesInSegmentationImageFilter_hxx

#include "itkFillHolesInSegmentationImageFilter.h"

// ITK
#include <itkBinaryFillholeImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkMaskImageFilter.h>

// Custom
#include <dvImageToSet.h>

namespace itk
{

template <typename TInputImage, typename TOutputImage>
FillHolesInSegmentationImageFilter<TInputImage, TOutputImage>
::FillHolesInSegmentationImageFilter()
{}


template <typename TInputImage, typename TOutputImage>
void
FillHolesInSegmentationImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputImage>
void
FillHolesInSegmentationImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  const InputImageType * input = this->GetInput();
  OutputImageType *      output = this->GetOutput();

  output->Graft( input );

  using TFilter = itk::BinaryFillholeImageFilter<TInputImage>;
  using TThresh = itk::BinaryThresholdImageFilter<TInputImage, TInputImage>;
  using TMask = itk::MaskImageFilter<TInputImage, TOutputImage>;

  const auto labels_set = dv::ImageToSet<InputImageDimension, InputPixelType>(input);

  for (const auto& s : labels_set) {
    // Ignore background.
    if (0 == s)
      continue;

    // Extract Label:
    const auto thresh = TThresh::New();
    thresh->SetInput(output);
    thresh->SetLowerThreshold(s);
    thresh->SetUpperThreshold(s);
    thresh->SetInsideValue(s);
    thresh->SetOutsideValue(0);

    const auto filter = TFilter::New();
    filter->SetInput(thresh->GetOutput());
    filter->SetForegroundValue(s);

    // Mask the input
    const auto mask = TMask::New();
    mask->SetInput(output);
    mask->SetMaskImage(filter->GetOutput());
    mask->SetMaskingValue(s);
    mask->SetOutsideValue(s);
    mask->Update();

    // Replace the input
    output->Graft(mask->GetOutput());
    output->DisconnectPipeline();
  }


}

} // end namespace itk

#endif // itkFillHolesInSegmentationImageFilter_hxx
