#ifndef dv_BinaryErosion_h
#define dv_BinaryErosion_h

#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryErodeImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
BinaryErosion(const std::string& IImage,
              const std::string& OImage,
              const unsigned int radius,
              const unsigned int foreground)
{
  typedef itk::Image<TPixel, Dimension> TImage;
  typedef itk::ImageFileReader<TImage> TReader;
  typedef itk::BinaryBallStructuringElement<TPixel, Dimension> TElement;
  typedef itk::BinaryErodeImageFilter<TImage, TImage, TElement>
    TErode;
  typedef itk::ImageFileWriter<TImage> TWriter;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  TElement structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  const auto erosion = TErode::New();
  erosion->SetInput(reader->GetOutput());
  erosion->SetKernel(structuringElement);
  erosion->SetForegroundValue(foreground);
  erosion->SetBackgroundValue(0);

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(erosion->GetOutput());
  writer->Update();
}

}

#endif
