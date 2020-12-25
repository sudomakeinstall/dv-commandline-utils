// ITK
#include <itkPointSet.h>

// Custom
#include <dvLabeledITKPointSetReader.h>
#include <dvLabeledITKPointSetToPointSetMap.h>

using TPointSet = itk::PointSet<double, 3>;

int main(int argc, char**argv) {

  if (argc != 2) {
    std::cerr << "Please provide path to test directory." << std::endl;
    return EXIT_FAILURE;
  }

  const std::string TESTDIR(argv[1]);

  const auto points = dv::LabeledITKPointSetReader<TPointSet>(TESTDIR+"labeled-point-set.txt");

  const auto pointset_map = dv::LabeledITKPointSetToPointSetMap<TPointSet>(points);

  for (const auto& i : pointset_map) {
    std::cout << i.first << " : " << i.second->GetPoints()->Size() << std::endl;
  }

  return EXIT_SUCCESS;

}
