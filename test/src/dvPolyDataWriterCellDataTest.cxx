// ITK
#include <itkQuadEdgeMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkAdditiveGaussianNoiseMeshFilter.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkTriangleHelper.h>
#include <itkMeshFileWriter.h>

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataReader.h>

// Custom
#include <dvITKMeshToVTKPolyData.h>

int
main() {

  // Typedefs
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::Mesh<TCoordinate, Dimension>;
  using TSource = itk::RegularSphereMeshSource< TMesh >;
  using TTriangleHelper = itk::TriangleHelper< typename TMesh::PointType >;
  using TWriter = itk::MeshFileWriter< TMesh >;

  // Generate some input mesh data
  const auto sphere = TSource::New();
  sphere->Update();

  const auto i_mesh = TMesh::New();
  i_mesh->Graft( sphere->GetOutput() );
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
    } else if (centroid[0] >= 0 && centroid[1] >= 0 && centroid[2] >= 0) {
      data = 8;
    }

    i_mesh->SetCellData( it.Index(), data );
  }

  // Assert one CellData entry for each Cell
  const auto i_cell = i_mesh->GetNumberOfCells();
  const auto i_data = i_mesh->GetCellData()->Size();
  itkAssertOrThrowMacro(i_cell == i_data,
    "Incorrect number of entries in input cell data array.");

  const auto i_polydata = dv::ITKMeshToVTKPolyData<TMesh>( i_mesh );

  const auto writer = TWriter::New();
  writer->SetInput( i_mesh );
  writer->SetFileName( "out.vtk");
  writer->Update();

  const auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName( "out.vtk" );
  reader->Update();

  const auto i_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  i_mapper->SetInputData( i_polydata );
  const auto i_actor = vtkSmartPointer<vtkActor>::New();
  i_actor->SetMapper(i_mapper);

  const auto o_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  o_mapper->SetInputData( reader->GetOutput() );
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

  return EXIT_SUCCESS;

}
