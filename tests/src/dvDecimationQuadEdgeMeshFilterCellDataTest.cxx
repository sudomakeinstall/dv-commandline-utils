// ITK
#include <itkQuadEdgeMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>

// Custom
#include <dvITKMeshToVTKPolyData.h>
#include <dvAssignCellDataByOctant.h>
#include <dvQuickViewSideBySidePolyData.h>

void
DecimationQuadEdgeMeshFilterCellDataTestHelper() {

  // Typedefs
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TQEMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
  using TSource = itk::RegularSphereMeshSource< TQEMesh >;
  using TCriterion = itk::NumberOfFacesCriterion<TQEMesh>;
  using TDecimation = itk::
    SquaredEdgeLengthDecimationQuadEdgeMeshFilter<TQEMesh, TQEMesh, TCriterion>;

  // Generate some input mesh data
  const auto sphere = TSource::New();
  sphere->SetResolution( 5 );
  sphere->Update();

  const auto i_mesh = TQEMesh::New();
  i_mesh->Graft( sphere->GetOutput() );
  i_mesh->DisconnectPipeline();

  dv::AssignCellDataByOctant< TQEMesh >( i_mesh );

  // Assert one CellData entry for each Cell
  const auto i_cell = i_mesh->GetNumberOfCells();
  const auto i_data = i_mesh->GetCellData()->Size();
  itkAssertOrThrowMacro(i_cell == i_data,
    "Incorrect number of entries in input cell data array.");

  const auto criterion = TCriterion::New();
  const auto decimate = TDecimation::New();
  criterion->SetNumberOfElements(1000);

  decimate->SetInput(sphere->GetOutput());
  decimate->SetCriterion(criterion);
  decimate->Update();

  const auto o_mesh = TQEMesh::New();
  o_mesh->Graft( decimate->GetOutput() );
  o_mesh->DisconnectPipeline();

  // Assert one CellData entry for each Cell
  const auto o_cell = o_mesh->GetNumberOfCells();
  const auto o_data = o_mesh->GetCellData()->Size();
  itkAssertOrThrowMacro(o_cell == o_data,
    "Incorrect number of entries in output cell data array.");

  // Visualize
  const auto i_polydata = dv::ITKMeshToVTKPolyData<TQEMesh>( i_mesh );
  const auto o_polydata = dv::ITKMeshToVTKPolyData<TQEMesh>( o_mesh );

  dv::QuickViewSideBySidePolyData( i_polydata, o_polydata );

}

int main() {

  DecimationQuadEdgeMeshFilterCellDataTestHelper();

  return EXIT_SUCCESS;

}
