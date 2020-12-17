// ITK
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkImageFileReader.h>

#include <itkCuberilleImageToMeshFilter.h>

// Custom
#include <itkGenerateInitialModelImageToMeshFilter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <itkCleanSegmentationImageFilter.h>
#include <dvQuickViewSideBySidePolyData.h>

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cerr << "Must provide a filename." << std::endl;
    return EXIT_FAILURE;
  }

  const std::string file_name(argv[1]);

  const double sigma = 0.1;
  const unsigned int count = 512;
  const unsigned int lv_radius = 10;
  const unsigned int gn_radius = 5;

  using TPixel = unsigned char;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TImage = itk::Image<TPixel, Dimension>;
  using TReader = itk::ImageFileReader<TImage>;
  using TModel = itk::GenerateInitialModelImageToMeshFilter<TImage,TMesh>;
  using TClean = itk::CleanSegmentationImageFilter<TImage>;

  using TCuberille = itk::CuberilleImageToMeshFilter<TImage, TMesh>;

  const auto reader = TReader::New();
  reader->SetFileName(file_name);

  const auto clean = TClean::New();
  clean->SetInput( reader->GetOutput() );

  const auto cuberille = TCuberille::New();
  cuberille->SetInput(clean->GetOutput());
  cuberille->GenerateTriangleFacesOn();
  cuberille->RemoveProblematicPixelsOn();
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();
  cuberille->Update();

  const auto model = TModel::New();
  model->SetInput(reader->GetOutput());
  model->SetNumberOfCellsInDecimatedMesh(count);
  model->SetMeshNoiseSigma(sigma);
  model->SetLVClosingRadius(lv_radius);
  model->SetGeneralClosingRadius(gn_radius);
  model->Update();

  const auto d = dv::ITKTriangleMeshToVTKPolyData< TMesh >( cuberille->GetOutput() );
  const auto m = dv::ITKTriangleMeshToVTKPolyData< TMesh >( model->GetOutput() );

  dv::QuickViewSideBySidePolyData( d, m );

  return EXIT_SUCCESS;

}
