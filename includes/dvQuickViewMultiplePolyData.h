#ifndef dv_QuickViewMultiplePolyData_h
#define dv_QuickViewMultiplePolyData_h

// STD
#include <vector>

// VTK
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkRendererCollection.h>

// DV
#include <dvGetLookupTable.h>

namespace dv
{
void
QuickViewMultiplePolyData(std::vector<vtkPolyData*> poly_data_vector, const bool synchronize_cameras = true, vtkLookupTable* lut = dv::LUT::Rainbow()) {

  size_t N = poly_data_vector.size();
  const double width = 1.0/N;

  const auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(N*300, 300);

  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->SetRenderWindow(renderWindow);

  for (size_t i = 0; i < poly_data_vector.size(); ++i) {

    const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData( poly_data_vector[i] );
    const auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    mapper->UseLookupTableScalarRangeOn();
    mapper->SetLookupTable(lut);
    double viewport[4] = {i*width, 0.0, (i+1)*width, 1.0};
    const auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetViewport(viewport);
    renderer->SetBackground(1, 1, 1);  
    renderer->AddActor(actor);
    renderWindow->AddRenderer(renderer);

  }

  if (synchronize_cameras) {
    const auto renderers = renderWindow->GetRenderers();
    const auto first_renderer = renderers->GetFirstRenderer();

    for (size_t i = 1; i < renderers->GetNumberOfItems(); ++i) {
      const auto r = static_cast<vtkRenderer*>(renderers->GetItemAsObject(i));
      r->SetActiveCamera( first_renderer->GetActiveCamera() );
      r->ResetCamera();
    }
  }

  renderWindow->Render();
  interactor->Start();

}
}

#endif
