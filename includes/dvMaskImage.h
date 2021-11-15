#ifndef dv_MaskImage_h
#define dv_MaskImage_h

#include <itkImageFileReader.h>
#include <itkMaskImageFilter.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
MaskImage(const std::string& IImage,
          const std::string& MImage,
          const std::string& OImage)
{
  typedef itk::Image<TPixel, Dimension> TImage;
  typedef itk::Image<unsigned char, Dimension> TMask;
  typedef itk::ImageFileReader<TImage> TImageReader;
  typedef itk::ImageFileReader<TMask> TMaskReader;
  typedef itk::ImageFileWriter<TImage> TWriter;
  using TMaskFilter = itk::MaskImageFilter<TImage, TMask>;

  const auto imageReader = TImageReader::New();
  imageReader->SetFileName(IImage);

  const auto maskReader = TMaskReader::New();
  maskReader->SetFileName(MImage);

  const auto mask = TMaskFilter::New();
  mask->SetInput( imageReader->GetOutput() );
  mask->SetMaskImage( maskReader->GetOutput() );

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(mask->GetOutput());
  writer->Update();

}

}

#endif
