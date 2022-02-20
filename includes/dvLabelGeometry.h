#ifndef dv_LabelGeometry_h
#define dv_LabelGeometry_h

// ITK
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkLabelGeometryImageFilter.h>

// Custom
#include <dvImageToSet.h>

// STD
#include <map>

namespace dv {

template<unsigned int Dimension, typename TPixel>
void
LabelGeometry(const std::string& IImage)
{

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TFilter = itk::LabelGeometryImageFilter<TImage>;

  //
  // Calculate Stats
  //

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto filter = TFilter::New();
  filter->SetInput(reader->GetOutput());
  filter->Update();

  const auto spacing = reader->GetOutput()->GetSpacing();
  const auto p = std::accumulate(spacing.cbegin(), spacing.cend(), 1.0, std::multiplies<double>{});

  //
  // Calculate Label Stats
  //

  const auto labels = dv::ImageToSet<Dimension, TPixel>(reader->GetOutput());

  for (const auto& l : labels) {
    itk::ContinuousIndex<double, Dimension> index;
    const auto indexCentroid = filter->GetCentroid(l);
    for (size_t i = 0; i < Dimension; ++i) {
      index[i] = indexCentroid[i];
    }

    typename TImage::PointType centroid;
    reader->GetOutput()->TransformContinuousIndexToPhysicalPoint(index,
                                                                 centroid);

    // Convert number of pixels to mL
    std::cout << "Volume," << static_cast<int>(l) << ',' << p*filter->GetVolume(l)/1000.0
              << std::endl;
    std::cout << "CentroidX," << static_cast<int>(l) << ',' << centroid[0]
              << std::endl;
    std::cout << "CentroidY," << static_cast<int>(l) << ',' << centroid[1]
              << std::endl;
    std::cout << "CentroidZ," << static_cast<int>(l) << ',' << centroid[2]
              << std::endl;
    std::cout << "Eccentricity," << static_cast<int>(l) << ','
              << filter->GetEccentricity(l) << std::endl;
    std::cout << "Elongation," << static_cast<int>(l) << ','
              << filter->GetElongation(l) << std::endl;
  }
}

}

#endif
