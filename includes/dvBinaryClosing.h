#ifndef dv_BinaryClosing_h
#define dv_BinaryClosing_h

#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
BinaryClosing(const std::string& IImage,
              const std::string& OImage,
              const unsigned int radius,
              const unsigned int foreground)
{
  typedef itk::Image<TPixel, Dimension> TImage;
  typedef itk::ImageFileReader<TImage> TReader;
  typedef itk::BinaryBallStructuringElement<TPixel, Dimension> TElement;
  typedef itk::BinaryMorphologicalClosingImageFilter<TImage, TImage, TElement>
    TOpen;
  typedef itk::ImageFileWriter<TImage> TWriter;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  TElement structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();

  const auto closingFilter = TOpen::New();
  closingFilter->SetInput(reader->GetOutput());
  closingFilter->SetKernel(structuringElement);
  closingFilter->SetForegroundValue(foreground);

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(closingFilter->GetOutput());
  writer->Update();
}

}

#endif
