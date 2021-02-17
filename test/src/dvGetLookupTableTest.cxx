// ITK
#include <itkTestingMacros.h>
#include <itkQuadEdgeMesh.h>
#include <itkRegularSphereMeshSource.h>

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
#include <dvGetLookupTable.h>
#include <dvITKMeshToVTKPolyData.h>
#include <dvQuickViewMultiplePolyData.h>

void
DelaunayCellDataTestHelper() {

  // Typedefs
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TQEMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
  using TSource = itk::RegularSphereMeshSource< TQEMesh >;

  const auto sphere = TSource::New();
  sphere->Update();

  const auto i_mesh = TQEMesh::New();
  i_mesh->Graft( sphere->GetOutput() );
  i_mesh->DisconnectPipeline();

  const double min = 0.3;
  const double max = 1.7;
  const double N = i_mesh->GetNumberOfCells();
  const auto lut = dv::LUT::SQUEEZ(min, max);

  const auto transfer = [](const double x, const double N, const double min, const double max) {
    return x/N*(max-min)+min;
  };

  // Assign cell data (different for each octant).
  for (auto it = i_mesh->GetCells()->Begin();
    it != i_mesh->GetCells()->End();
    ++it) {
    const auto data = transfer(it.Index(), N, min, max);
    i_mesh->SetCellData( it.Index(), data );
  }

  // Visualize
  const auto i_polydata = dv::ITKMeshToVTKPolyData<TQEMesh>( i_mesh );
  std::vector<vtkPolyData*> polydata_vec;
  polydata_vec.push_back(i_polydata);

  dv::QuickViewMultiplePolyData(polydata_vec, true, dv::LUT::SQUEEZ(min, max));
//  const auto i_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//  i_mapper->SetInputData( i_polydata );
//  const auto i_actor = vtkSmartPointer<vtkActor>::New();
//  i_actor->SetMapper(i_mapper);
//  i_mapper->UseLookupTableScalarRangeOn();
//  i_mapper->SetLookupTable(lut);
//
//  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
//  
//  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  interactor->SetRenderWindow(renderWindow);
//  
//  const auto i_renderer = vtkSmartPointer<vtkRenderer>::New();
//  renderWindow->AddRenderer(i_renderer);
//  i_renderer->SetBackground(1, 1, 1);  
//
//  i_renderer->AddActor(i_actor);
//  i_renderer->ResetCamera();
//
//  renderWindow->Render();
//  interactor->Start();

}

int main() {

  DelaunayCellDataTestHelper();

  return EXIT_SUCCESS;

}
