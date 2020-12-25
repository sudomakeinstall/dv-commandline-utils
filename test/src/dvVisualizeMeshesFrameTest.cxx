#include <dvVisualizeMeshesFrame.h>
#include <vtkRenderer.h>

int main() {

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  const auto frame = dv::VisualizeMeshesFrame(renderer);

  return EXIT_SUCCESS;

}
