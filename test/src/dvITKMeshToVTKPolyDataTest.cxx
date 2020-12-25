#include <dvITKMeshToVTKPolyData.h>
#include <itkMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>

using TMesh = itk::Mesh<float, 3>;
using TSource = itk::RegularSphereMeshSource< TMesh >;

int main() {

  const auto source = TSource::New();
  source->Update();
  const auto polydata = dv::ITKMeshToVTKPolyData<TMesh>( source->GetOutput() );
  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata);
  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  const auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->AddActor(actor);
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;

}
