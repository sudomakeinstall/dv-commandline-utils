// ITK
#include <itkMeshFileReader.h>
#include <itkQuadEdgeMesh.h>

// Custom
#include <dvGenerateLabeledSphere.h>
#include <dvITKMeshToVTKPolyData.h>
#include <dvQuickViewMultiplePolyData.h>
#include <dvCalculateBorderCells.h>

constexpr unsigned int Dimension = 3;
using TPixel = float;
using TMesh = itk::QuadEdgeMesh<TPixel,Dimension>;

int main(int argc, char** argv) {

  const auto i_mesh = dv::GenerateLabeledSphere<TMesh>();

  const auto edge_cells = dv::CalculateBorderCells<TMesh>(i_mesh);

  for (const auto e : edge_cells) {
    i_mesh->GetCellData()->SetElement(e, 0);
  }

  const auto polydata = dv::ITKMeshToVTKPolyData<TMesh>(i_mesh);
  std::vector<vtkPolyData*> p_vec;
  p_vec.push_back(polydata);

  dv::QuickViewMultiplePolyData(p_vec);

  return EXIT_SUCCESS;

}
