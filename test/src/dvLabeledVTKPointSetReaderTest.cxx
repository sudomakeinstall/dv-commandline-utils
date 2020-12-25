#include <dvLabeledVTKPointSetReader.h>

//Visualization
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>

int main(int argc, char**argv) {

  if (argc != 2) {
    std::cerr << "Please provide path to test directory." << std::endl;
    return EXIT_FAILURE;
  }

  const std::string TESTDIR(argv[1]);

  const auto polydata = dv::LabeledVTKPointSetReader(TESTDIR+"labeled-point-set.txt");

  // Visualization
  const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputData(polydata);

  const auto actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetPointSize(10);

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  const auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(1.0, 1.0, 1.0);
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;

}
