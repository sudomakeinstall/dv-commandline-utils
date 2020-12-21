#ifndef dv_GenerateInitialModel_h
#define dv_GenerateInitialModel_h

// ITK
#include <itkImageFileReader.h>

// VTK
#include <vtkPolyDataWriter.h>

// Custom
#include <itkGenerateInitialModelImageToMeshFilter.h>
#include <dvITKMeshToVTKPolyData.h>

namespace dv {

void
GenerateInitialModel(const std::string& inputSegmentationName,
                     const std::string& outputMeshName,
                     const unsigned int& count,
                     const double& sigma,
                     const unsigned int& radius)
{

  using TPixel = unsigned char;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TModel = itk::GenerateInitialModelImageToMeshFilter<TImage,TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName( inputSegmentationName );

  const auto model = TModel::New();
  model->SetInput( reader->GetOutput() );
  model->SetNumberOfCellsInDecimatedMesh(count);
  model->SetMeshNoiseSigma(sigma);
  model->SetLVClosingRadius(radius);
  model->Update();

  const auto poly_data = dv::ITKMeshToVTKPolyData< TMesh >( model->GetOutput() );

  const auto writer = vtkSmartPointer< vtkPolyDataWriter >::New();
  writer->SetInputData( poly_data );
  writer->SetFileName(outputMeshName.c_str());
  writer->Update();

}

}

#endif
