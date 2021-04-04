#include <vtkCamera.h>
#include <vtkSmartPointer.h>

#include <dvCameraState.h>
#include <dvRapidJSONHelper.h>

namespace dv {

void
CameraState ::CaptureState(vtkCamera* const camera)
{
  camera->GetPosition(this->Position);
  camera->GetFocalPoint(this->FocalPoint);
  camera->GetViewUp(this->ViewUp);
  this->ViewAngle = camera->GetViewAngle(); // No effect when in parallel mode
  this->ParallelScale = camera->GetParallelScale();
  this->ParallelProjection = camera->GetParallelProjection();
}

void
CameraState ::RestoreState(vtkCamera* const camera) const
{
  camera->SetPosition(this->Position);
  camera->SetFocalPoint(this->FocalPoint);
  camera->SetViewUp(this->ViewUp);
  camera->SetViewAngle(this->ViewAngle); // No effect when in parallel mode
  camera->SetParallelScale(this->ParallelScale);
  camera->SetParallelProjection(this->ParallelProjection);
}

void
CameraState ::SerializeJSON(
  rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
  writer.Key("camera.ViewAngle");
  writer.Double(this->ViewAngle);

  writer.Key("camera.ParallelScale");
  writer.Double(this->ParallelScale);

  writer.Key("camera.ParallelProjection");
  writer.Int(this->ParallelProjection);

  writer.Key("camera.Position");
  writer.StartArray();
  for (size_t i = 0; i < 3; ++i)
    writer.Double(this->Position[i]);
  writer.EndArray();

  writer.Key("camera.FocalPoint");
  writer.StartArray();
  for (size_t i = 0; i < 3; ++i)
    writer.Double(this->FocalPoint[i]);
  writer.EndArray();

  writer.Key("camera.ViewUp");
  writer.StartArray();
  for (size_t i = 0; i < 3; ++i)
    writer.Double(this->ViewUp[i]);
  writer.EndArray();
}

void
CameraState::DeserializeJSON(const rapidjson::Document& d)
{
  check_and_set_double(d, this->ViewAngle, "camera.ViewAngle");
  check_and_set_double(d, this->ParallelScale, "camera.ParallelScale");

  check_and_set_int(d, this->ParallelProjection, "camera.ParallelProjection");

  check_and_set_double_array(d, this->Position, "camera.Position");
  check_and_set_double_array(d, this->FocalPoint, "camera.FocalPoint");
  check_and_set_double_array(d, this->ViewUp, "camera.ViewUp");
}

} // end namespace vtk
