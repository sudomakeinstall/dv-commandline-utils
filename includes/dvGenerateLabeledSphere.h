#ifndef dv_GenerateLabeledSphere_h
#define dv_GenerateLabeledSphere_h

// ITK
#include <itkRegularSphereMeshSource.h>
#include <itkTriangleHelper.h>

namespace dv {

template<typename TMesh>
auto
GenerateLabeledSphere() -> typename TMesh::Pointer {

  // Typedefs
  using TSource = itk::RegularSphereMeshSource< TMesh >;
  using TTriangleHelper = itk::TriangleHelper< typename TMesh::PointType >;

  // Generate some input mesh data
  const auto sphere = TSource::New();
  sphere->SetResolution( 4 );
  sphere->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( sphere->GetOutput() );
  mesh->DisconnectPipeline();

  // Assign cell data (different for each octant).
  for (auto it = mesh->GetCells()->Begin();
    it != mesh->GetCells()->End();
    ++it) {
    const auto cell = it.Value();

    const auto centroid = TTriangleHelper::ComputeGravityCenter(
      mesh->GetPoint(cell->GetPointIds()[0]),
      mesh->GetPoint(cell->GetPointIds()[1]),
      mesh->GetPoint(cell->GetPointIds()[2])
    );

    unsigned int data = 0;
    if (centroid[0] < 0 && centroid[1] < 0 && centroid[2] < 0) {
      data = 1;
    } else if (centroid[0] < 0 && centroid[1] < 0 && centroid[2] >= 0) {
      data = 2;
    } else if (centroid[0] < 0 && centroid[1] >= 0 && centroid[2] < 0) {
      data = 3;
    } else if (centroid[0] < 0 && centroid[1] >= 0 && centroid[2] >= 0) {
      data = 4;
    } else if (centroid[0] >= 0 && centroid[1] < 0 && centroid[2] < 0) {
      data = 5;
    } else if (centroid[0] >= 0 && centroid[1] < 0 && centroid[2] >= 0) {
      data = 6;
    } else if (centroid[0] >= 0 && centroid[1] >= 0 && centroid[2] < 0) {
      data = 7;
    } else if (centroid[0] >= 0 && centroid[1] >= 0 && centroid[2] >= 0) {
      data = 8;
    }

    mesh->SetCellData( it.Index(), data );
  }

  // Assert one CellData entry for each Cell
  const auto i_cell = mesh->GetNumberOfCells();
  const auto i_data = mesh->GetCellData()->Size();
  itkAssertOrThrowMacro(i_cell == i_data,
    "Incorrect number of entries in input cell data array.");

  return mesh;

}

}

#endif
