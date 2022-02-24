
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkSimplexMesh.h>
#include <itkSimplexMeshVolumeCalculator.h>
#include <itkTriangleMeshToSimplexMeshFilter.h>
#include <itkTriangleHelper.h>

// Typedefs
using TMesh = itk::Mesh<float, 3>;
using TSimplex = itk::SimplexMesh<float, 3>;
using TReader = itk::MeshFileReader<TMesh>;
using TConvert = itk::TriangleMeshToSimplexMeshFilter<TMesh, TSimplex>;
using TVolume = itk::SimplexMeshVolumeCalculator<TSimplex>;
using TPoint = TMesh::PointType;
using TTriangleHelper = itk::TriangleHelper<TPoint>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-mesh",
    po::value<std::string>()->required(),
    "Filename of input mesh.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const auto reader = TReader::New();
  reader->SetFileName(vm["input-mesh"].as<std::string>());
  reader->Update();

  std::cout << "Points: " << reader->GetOutput()->GetNumberOfPoints()
            << std::endl;
  std::cout << "Cells: " << reader->GetOutput()->GetNumberOfCells()
            << std::endl;

  std::map<int, double> area_map;

  // Ensure that all cells of the mesh are triangles.
  for (TMesh::CellsContainerIterator it =
         reader->GetOutput()->GetCells()->Begin();
       it != reader->GetOutput()->GetCells()->End();
       ++it) {
    TMesh::CellAutoPointer cell;
    reader->GetOutput()->GetCell(it->Index(), cell);
    if (3 != cell->GetNumberOfPoints()) {
      std::cerr << "ERROR: All cells must be trianglar." << std::endl;
      return EXIT_FAILURE;
    }

    const auto p0 = reader->GetOutput()->GetPoints()->ElementAt(cell->GetPointIds()[0]);
    const auto p1 = reader->GetOutput()->GetPoints()->ElementAt(cell->GetPointIds()[1]);
    const auto p2 = reader->GetOutput()->GetPoints()->ElementAt(cell->GetPointIds()[2]);
    area_map[reader->GetOutput()->GetCellData()->ElementAt(it->Index())] = TTriangleHelper::ComputeArea(p0, p1, p2);

  }

  // Convert the triangle mesh to a simplex mesh.
  TConvert::Pointer convert = TConvert::New();
  convert->SetInput(reader->GetOutput());
  convert->Update();

  // Calculate the volume and area of the simplex mesh.
  TVolume::Pointer volume = TVolume::New();
  volume->SetSimplexMesh(convert->GetOutput());
  volume->Compute();

  // Compare with the volume and area of an ideal sphere.
  std::cout << "Volume: " << volume->GetVolume() << std::endl;
  std::cout << "Surface Area: " << volume->GetArea() << std::endl;

  for (const auto x : area_map) {
    std::cout << "Surface Area " << x.first << ": " << x.second << std::endl;
  }

  return EXIT_SUCCESS;
}
