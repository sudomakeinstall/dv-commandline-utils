#ifndef dv_FillHolesInSegmentation_h
#define dv_FillHolesInSegmentation_h

#include <itkImageFileReader.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkBinaryFillholeImageFilter.h>
#include <itkImageFileWriter.h>
#include <itkMaskImageFilter.h>


#include <dvImageToSet.h>

namespace dv
{

  template<unsigned int Dimension, typename TPixel>
  void
  FillHolesInSegmentation(const std::string& IImage, const std::string& OImage)
  {
    using TImage  = itk::Image< TPixel, Dimension >;
    using TReader = itk::ImageFileReader< TImage >;
    using TFilter = itk::BinaryFillholeImageFilter< TImage >;
    using TThresh = itk::BinaryThresholdImageFilter< TImage, TImage >;
    using TMask   = itk::MaskImageFilter< TImage, TImage >;
    using TWriter = itk::ImageFileWriter< TImage >;

    auto input = TImage::New();

    {
      const auto reader = TReader::New();
      reader->SetFileName( IImage );
      reader->Update();
      input->Graft( reader->GetOutput() );
    }

    const auto labels_set = dv::ImageToSet<Dimension, TPixel>( input );

    for (const auto &s : labels_set)
    {
      // Ignore background.
      if (0 == s) continue;

      // Extract Label:
      const auto thresh = TThresh::New();
      thresh->SetInput( input );
      thresh->SetLowerThreshold( s );
      thresh->SetUpperThreshold( s );
      thresh->SetInsideValue( s );
      thresh->SetOutsideValue( 0 );

      const auto filter = TFilter::New();
      filter->SetInput( thresh->GetOutput() );
      filter->SetForegroundValue( s );

      // Mask the input
      const auto mask = TMask::New();
      mask->SetInput( input );
      mask->SetMaskImage( filter->GetOutput() );
      mask->SetMaskingValue( s );
      mask->SetOutsideValue( s );
      mask->Update();

      // Replace the input
      input->Graft( mask->GetOutput() );
      input->DisconnectPipeline();

    }

    const auto writer = TWriter::New();
    writer->SetFileName( OImage );
    writer->SetInput( input );
    writer->Update();
  }

}

#endif
