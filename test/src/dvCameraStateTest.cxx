#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <dvCameraState.h>
#include <vtkAxesActor.h>

int main()
{

  const auto renderer = vtkRenderer::New();
  const auto window = vtkRenderWindow::New();
  const auto axes = vtkAxesActor::New();
  window->AddRenderer(renderer);
  const auto interactor = vtkRenderWindowInteractor::New();
  interactor->SetRenderWindow(window);
  renderer->AddActor(axes);
  interactor->Start();

  auto state = dv::CameraState();
  state.CaptureState(renderer->GetActiveCamera());

  assert(renderer->GetActiveCamera()->GetViewAngle()          == state.ViewAngle);
  assert(renderer->GetActiveCamera()->GetParallelScale()      == state.ParallelScale);
  assert(renderer->GetActiveCamera()->GetParallelProjection() == state.ParallelProjection);

  for (size_t i = 0; i < 3; ++i)
    {
    assert(renderer->GetActiveCamera()->GetPosition()[i] == state.Position[i]);
    assert(renderer->GetActiveCamera()->GetFocalPoint()[i] == state.FocalPoint[i]);
    assert(renderer->GetActiveCamera()->GetViewUp()[i] == state.ViewUp[i]);
    }

  return EXIT_SUCCESS;

}
