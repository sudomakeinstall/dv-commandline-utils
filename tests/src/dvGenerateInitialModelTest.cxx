// ITK
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkImageFileReader.h>

// Custom
#include <itkGenerateInitialModelImageToMeshFilter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvQuickViewPolyData.h>

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

  const auto reader = TReader::New();
  reader->SetFileName(file_name);

  const auto model = TModel::New();
  model->SetInput(reader->GetOutput());
  model->SetNumberOfCellsInDecimatedMesh(count);
  model->SetMeshNoiseSigma(sigma);
  model->SetLVClosingRadius(lv_radius);
  model->SetGeneralClosingRadius(gn_radius);
  model->Update();

  const auto poly_data = dv::ITKTriangleMeshToVTKPolyData< TMesh >( model->GetOutput() );
  dv::QuickViewPolyData( poly_data );

  return EXIT_SUCCESS;

}
