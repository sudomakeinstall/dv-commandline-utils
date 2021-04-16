// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMeshFileReader.h>
#include <itkQuadEdgeMesh.h>

const unsigned int Dimension = 3;
using TCoordinate = float;

using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
using TReader = itk::MeshFileReader<TMesh>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("mesh", po::value<std::string>()->required(), "Filename of the source mesh.")
    ("label-1", po::value<unsigned int>()->required(), "First label.")
    ("label-2", po::value<unsigned int>()->required(), "Second label.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string mesh1File(vm["mesh"].as<std::string>());
  const unsigned int n1 = vm["label-1"].as<unsigned int>();
  const unsigned int n2 = vm["label-2"].as<unsigned int>();

  const auto reader1 = TReader::New();
  reader1->SetFileName(mesh1File);
  reader1->Update();

  const auto mesh1 = TMesh::New();
  mesh1->Graft( reader1->GetOutput() );

  std::vector<TMesh::PointType> points;

  for (auto it = mesh1->GetPoints()->Begin(); it != mesh1->GetPoints()->End(); ++it) {
    std::set<unsigned int> labels;
    const auto edge = mesh1->FindEdge(it->Index());
    auto temp = edge;
    do {
      temp = temp->GetOnext();
      labels.emplace(mesh1->GetCellData()->ElementAt(temp->GetLeft()));
    } while (temp != edge);
    if (labels.count(n1) && labels.count(n2)) {
      points.emplace_back(it->Value());
    }
  }

  TMesh::PointType point;
  point.Fill(0.0);
  for (const auto &p : points) {
    point[0] += p[0];
    point[1] += p[1];
    point[2] += p[2];
  }
  point[0] /= points.size();
  point[1] /= points.size();
  point[2] /= points.size();

  std::cout << point[0] << ',' << point[1] << ',' << point[2] << '\n';

  return EXIT_SUCCESS;
}
