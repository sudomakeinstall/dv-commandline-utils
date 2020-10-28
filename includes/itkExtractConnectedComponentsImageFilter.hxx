#ifndef itkExtractConnectedComponentsImageFilter_hxx
#define itkExtractConnectedComponentsImageFilter_hxx

#include "itkExtractConnectedComponentsImageFilter.h"

// ITK
#include <itkBinaryThresholdImageFilter.h>
#include <itkConnectedComponentImageFilter.h>
#include <itkLabelShapeKeepNObjectsImageFilter.h>
#include <itkMaskImageFilter.h>

// Custom
#include <dvImageToSet.h>

namespace itk
{

template <typename TInputImage, typename TOutputImage>
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::ExtractConnectedComponentsImageFilter()
{
  this->m_N = 1;
  this->m_Background = 0;
}


template <typename TInputImage, typename TOutputImage>
void
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputImage>
void
ExtractConnectedComponentsImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  using TLabel = itk::ConnectedComponentImageFilter<InputImageType, InputImageType>;
  using TKeep = itk::LabelShapeKeepNObjectsImageFilter<InputImageType>;
  using TThresh = itk::BinaryThresholdImageFilter<InputImageType, InputImageType>;
  using TMask = itk::MaskImageFilter<InputImageType, OutputImageType>;

  const InputImageType * input = this->GetInput();
  OutputImageType *      output = this->GetOutput();

  const auto labels_set = dv::ImageToSet<InputImageDimension, InputPixelType>(input);

  output->Graft( input );

  for (const auto& s : labels_set) {
    // Ignore background.
    if (this->m_Background == s)
      continue;

    // Extract Label:
    const auto thresh = TThresh::New();
    thresh->SetInput(output);
    thresh->SetLowerThreshold(s);
    thresh->SetUpperThreshold(s);
    thresh->SetInsideValue(1);
    thresh->SetOutsideValue(0);

    // Connected Components:
    const auto connected = TLabel::New();
    connected->SetInput(thresh->GetOutput());
    connected->FullyConnectedOff();
    connected->Update();

    const auto NumObjects = connected->GetObjectCount();

    // Continue if only N objects are detected.
    if (NumObjects <= this->GetN())
      continue;

    // Keep (ObjectCount - N) smallest components.
    const auto keep = TKeep::New();
    keep->SetInput(connected->GetOutput());
    keep->SetBackgroundValue(0);
    keep->SetNumberOfObjects(NumObjects - this->GetN());
    keep->SetAttribute(TKeep::LabelObjectType::NUMBER_OF_PIXELS);
    keep->ReverseOrderingOn();

    // Binarize:
    const auto thresh2 = TThresh::New();
    thresh2->SetInput(keep->GetOutput());
    thresh2->SetLowerThreshold(1);
    thresh2->SetInsideValue(1);
    thresh2->SetOutsideValue(0);

    // Mask the input
    const auto mask = TMask::New();
    mask->SetInput(output);
    mask->SetMaskImage(thresh2->GetOutput());
    mask->SetMaskingValue(1);
    mask->SetOutsideValue(0);
    mask->Update();

    // Replace the input
    output->Graft(mask->GetOutput());
  }

}

} // end namespace itk

#endif // itkExtractConnectedComponentsImageFilter_hxx
