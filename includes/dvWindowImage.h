#ifndef dv_WindowImage_h
#define dv_WindowImage_h

// ITK includes
#include <itkImage.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkIntensityWindowingImageFilter.h>
#include <itkNumericTraits.h>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
WindowImage(const std::string& IImage,
            const std::string& OImage,
            const itk::FixedArray<double, 2>& scale,
            itk::FixedArray<double, 2>& o_range,
            const bool& o_range_exists)
{

  if (!o_range_exists) {
    o_range[0] = itk::NumericTraits<double>::min();
    o_range[1] = itk::NumericTraits<double>::max();
  }

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto i_ext = OImage.substr(OImage.size() - 3, 3);

  if (i_ext == "png") {

    using TImagePNG = itk::Image<unsigned short, Dimension>;
    using TFilterPNG = itk::IntensityWindowingImageFilter<TImage, TImagePNG>;
    using TWriterPNG = itk::ImageFileWriter<TImagePNG>;

    const auto filter = TFilterPNG::New();
    filter->SetInput(reader->GetOutput());
    filter->SetWindowMinimum(scale[0]);
    filter->SetWindowMaximum(scale[1]);
    filter->SetOutputMinimum(itk::NumericTraits<unsigned short>::min());
    filter->SetOutputMaximum(itk::NumericTraits<unsigned short>::max());

    // Add a cast of the image to unsigned SHORT
    const auto writer = TWriterPNG::New();
    writer->SetInput(filter->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();

  } else {

    using TFilter = itk::IntensityWindowingImageFilter<TImage>;
    using TWriter = itk::ImageFileWriter<TImage>;

    const auto filter = TFilter::New();
    filter->SetInput(reader->GetOutput());
    filter->SetWindowMinimum(scale[0]);
    filter->SetWindowMaximum(scale[1]);
    filter->SetOutputMinimum(o_range[0]);
    filter->SetOutputMaximum(o_range[1]);

    const auto writer = TWriter::New();
    writer->SetInput(filter->GetOutput());
    writer->SetFileName(OImage);
    writer->Update();
  }
}

}

#endif
