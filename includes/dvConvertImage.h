#ifndef dv_ConvertImage_h
#define dv_ConvertImage_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
ConvertImage(const std::string& IImage, const std::string& OImage)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TWriter = itk::ImageFileWriter<TImage>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto writer = TWriter::New();
  writer->SetInput(reader->GetOutput());
  writer->SetFileName(OImage);
  writer->UseCompressionOn();
  writer->Update();
}

}

#endif
