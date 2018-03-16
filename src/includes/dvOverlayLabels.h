#ifndef dv_OverlayLabels_h
#define dv_OverlayLabels_h

//ITK Includes
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkLabelObject.h>
#include <itkLabelMap.h>
#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelMapOverlayImageFilter.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
OverlayLabels(
  const std::string IImage,
  const std::string LImage,
  const std::string OImage
)
{

  using TImage     = itk::Image< TPixel, Dimension >;
  using TReader    = itk::ImageFileReader< TImage >;
  using TWriter    = itk::ImageFileWriter< TImage >;

  const auto iReader = TReader::New();
  const auto lReader = TReader::New();
  iReader->SetFileName( IImage );
  lReader->SetFileName( LImage );

  using TLabel = TPixel;
  using TLabelObject = itk::LabelObject< TLabel, Dimension >;
  using TLabelMap = itk::LabelMap< TLabelObject >;

  using TConverter = itk::LabelImageToLabelMapFilter< TImage, TLabelMap >;
  ConverterType::Pointer converter = TConverter::New();
  converter->SetInput( lReader->GetOutput() );

  using TFilter = itk::LabelMapOverlayImageFilter< TLabelMap, TImage >;
  FilterType::Pointer filter = TFilter::New();
  filter->SetInput( converter->GetOutput() );
  filter->SetFeatureImage( iReader->GetOutput() );
  filter->SetOpacity( 0.5 );

  const auto writer = TWriter::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
