
// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// VTK
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <vtkFieldData.h>
#include <vtkMeshQuality.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkSmartPointer.h>

using TCoordinate = float;

using TReader = vtkSmartPointer<vtkPolyDataReader>;
using TQuality = vtkSmartPointer<vtkMeshQuality>;
using TMetric = vtkMeshQuality::QualityMeasureTypes;

int
main(int argc, char** argv)
{

  std::map<int, std::string> quality_map;
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::AREA)] = "TriangleArea";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::RADIUS_RATIO)] = "TriangleRadiusRatio";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::ASPECT_RATIO)] = "TriangleAspectRatio";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::ASPECT_FROBENIUS)] = "TriangleAspectFrobenius";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::EDGE_RATIO)] = "TriangleEdgeRatio";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::MIN_ANGLE)] = "TriangleMinAngle";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::MAX_ANGLE)] = "TriangleMaxAngle";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::CONDITION)] = "TriangleCondition";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::SCALED_JACOBIAN)] = "TriangleScaledJacobian";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::RELATIVE_SIZE_SQUARED)] = "TriangleRelativeSizeSquared";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::SHAPE)] = "TriangleShape";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::SHAPE_AND_SIZE)] = "TriangleShapeAndSize";
  quality_map[static_cast<std::underlying_type_t<TMetric>>(TMetric::DISTORTION)] = "TriangleDistortion";

  std::string metric_description =
    "Integer corresponding to the quality metric.\n";
  for (const auto& pair : quality_map) {
    metric_description +=
      ('\t' + std::to_string(pair.first) + ' ' + pair.second + '\n');
  }

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh", po::value<std::string>()->required(), "Filename of the input mesh.")
    ("metric", po::value<int>()->default_value(static_cast<std::underlying_type_t<TMetric>>(TMetric::RADIUS_RATIO)), metric_description.c_str())
    ("summary", "Print summary.")
    ("summary-hide-header", "Hide the summary header.")
    ("cellwise", "Print cellwise data.")
    ("cellwise-hide-header", "Hide the cellwise header.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IMesh(vm["input-mesh"].as<std::string>());
  const int metric = vm["metric"].as<int>();
  const bool summary = vm.count("summary");
  const bool summary_header = !(vm.count("summary-hide-header"));
  const bool cellwise = vm.count("cellwise");
  const bool cellwise_header = !(vm.count("cellwise-hide-header"));

  const auto reader = TReader::New();
  reader->SetFileName(IMesh.c_str());
  reader->Update();

  const auto quality = TQuality::New();
  quality->SetInputConnection(reader->GetOutputPort());
  quality->SetSaveCellQuality(cellwise);
  quality->SetTriangleQualityMeasure(metric);
  quality->Update();

  if (summary) {
    const auto fd =
      quality->GetOutput()->GetFieldData()->GetArray("Mesh Triangle Quality");
    const auto mn = fd->GetComponent(0, 0);
    const auto mu = fd->GetComponent(0, 1);
    const auto mx = fd->GetComponent(0, 2);
    const auto sd = fd->GetComponent(0, 3);

    if (summary_header) {
      std::cout << "Metric,Min,Average,Max,SD\n";
    }
    std::cout << quality_map[metric] << ','
              << mn << ','
              << mu << ','
              << mx << ','
              << sd << std::endl;
  }

  if (cellwise) {
    const auto qualityArray = vtkDoubleArray::SafeDownCast(
      quality->GetOutput()->GetCellData()->GetArray("Quality"));

    if (cellwise_header) {
      std::cout << "CellID,Area\n";
    }

    for (vtkIdType i = 0; i < qualityArray->GetNumberOfTuples(); ++i) {
      double val = qualityArray->GetValue(i);
      std::cout << i << ',' << val << '\n';
    }
  }

  return EXIT_SUCCESS;

}
