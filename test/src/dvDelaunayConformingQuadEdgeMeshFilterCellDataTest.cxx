// ITK
#include <itkTestingMacros.h>
#include <itkQuadEdgeMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkTriangleHelper.h>
#include <itkMeshFileReader.h>

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

void
DelaunayCellDataTestHelper() {

  // Typedefs
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TQEMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
  using TReader = itk::MeshFileReader< TQEMesh >;
//  using TSource = itk::RegularSphereMeshSource< TQEMesh >;
  using TTriangleHelper = itk::TriangleHelper< typename TQEMesh::PointType >;
  using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<TQEMesh>;

  // Generate some input mesh data
  const auto reader = TReader::New();
  reader->SetFileName( "/Users/davisvigneault/Developer/repositories/dv-commandline-utils/src/tests/testdata/suzanne-triangles.obj" );
  reader->Update();
//  const auto sphere = TSource::New();
//  sphere->Update();

  const auto i_mesh = TQEMesh::New();
  i_mesh->Graft( reader->GetOutput() );
  i_mesh->DisconnectPipeline();

  // Assign cell data (different for each octant).
  for (auto it = i_mesh->GetCells()->Begin();
    it != i_mesh->GetCells()->End();
    ++it) {
    const auto cell = it.Value();

    const auto centroid = TTriangleHelper::ComputeGravityCenter(
      i_mesh->GetPoint(cell->GetPointIds()[0]),
      i_mesh->GetPoint(cell->GetPointIds()[1]),
      i_mesh->GetPoint(cell->GetPointIds()[2])
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
    } else {
      data = 8;
    }
    itkAssertOrThrowMacro(data != 0, "Not possible.");
    i_mesh->SetCellData( it.Index(), data );
  }

  // Assert one CellData entry for each Cell
  const auto i_cell = i_mesh->GetNumberOfCells();
  const auto i_data = i_mesh->GetCellData()->Size();
  itkAssertOrThrowMacro(i_cell == i_data,
    "Incorrect number of entries in input cell data array.");

  const auto delaunay = TDelaunay::New();
  delaunay->SetInput( i_mesh );
  delaunay->Update();

  const auto o_mesh = TQEMesh::New();
  o_mesh->Graft( delaunay->GetOutput() );
  o_mesh->DisconnectPipeline();

  // Assign cell data (different for each octant).
  for (auto it = o_mesh->GetCells()->Begin();
    it != o_mesh->GetCells()->End();
    ++it) {
    if (!o_mesh->GetCellData()->IndexExists(it.Index())) {
      std::cout << it.Index() << std::endl;
    }
  }

  // Assert one CellData entry for each Cell
  const auto o_cell = o_mesh->GetNumberOfCells();
  const auto o_data = o_mesh->GetCellData()->Size();
  if (o_cell != o_data) {
    itkGenericExceptionMacro("o_cell != o_data");
  }

  // Visualize
  const auto i_polydata = dv::ITKMeshToVTKPolyData<TQEMesh>( i_mesh );
  const auto i_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  i_mapper->SetInputData( i_polydata );
  const auto i_actor = vtkSmartPointer<vtkActor>::New();
  i_actor->SetMapper(i_mapper);

  const auto o_polydata = dv::ITKMeshToVTKPolyData<TQEMesh>( o_mesh );
  const auto o_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  o_mapper->SetInputData( o_polydata );
  const auto o_actor = vtkSmartPointer<vtkActor>::New();
  o_actor->SetMapper(o_mapper);

  // Create a lookup table to map cell data to colors
  const auto lut = vtkSmartPointer<vtkLookupTable>::New();
  lut->SetNumberOfTableValues(9);
  lut->Build();

  // Fill in a few known colors, the rest will be generated if needed
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  lut->SetTableValue(0, colors->GetColor4d("Black").GetData());
  lut->SetTableValue(1, colors->GetColor4d("Red").GetData());
  lut->SetTableValue(2, colors->GetColor4d("Orange").GetData());
  lut->SetTableValue(3, colors->GetColor4d("Yellow").GetData());
  lut->SetTableValue(4, colors->GetColor4d("Green").GetData());
  lut->SetTableValue(5, colors->GetColor4d("Blue").GetData());
  lut->SetTableValue(6, colors->GetColor4d("Purple").GetData());
  lut->SetTableValue(7, colors->GetColor4d("Cyan").GetData());
  lut->SetTableValue(8, colors->GetColor4d("Magenta").GetData());

  i_mapper->SetScalarRange(0, 8);
  i_mapper->SetLookupTable(lut);
  o_mapper->SetScalarRange(0, 8);
  o_mapper->SetLookupTable(lut);

  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(600, 300);
  
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);
  
  double i_viewport[4] = {0.0, 0.0, 0.5, 1.0};
  double o_viewport[4] = {0.5, 0.0, 1.0, 1.0};
  
  const auto i_renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(i_renderer);
  i_renderer->SetViewport(i_viewport);
  i_renderer->SetBackground(1, 1, 1);  

  const auto o_renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow->AddRenderer(o_renderer);
  o_renderer->SetViewport(o_viewport);
  o_renderer->SetBackground(1, 1, 1);  
      
  i_renderer->AddActor(i_actor);
  o_renderer->AddActor(o_actor);

  o_renderer->SetActiveCamera( i_renderer->GetActiveCamera() );

  i_renderer->ResetCamera();
  o_renderer->ResetCamera();

  renderWindow->Render();
  interactor->Start();

}

int main() {

  DelaunayCellDataTestHelper();

  return EXIT_SUCCESS;

}
