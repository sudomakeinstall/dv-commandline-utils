#ifndef dv_CameraState_h
#define dv_CameraState_h

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

namespace dv {
class CameraState
{

public:
  double Position[3] = { 0.0, 0.0, 0.0 };
  double FocalPoint[3] = { 0.0, 0.0, 0.0 };
  double ViewUp[3] = { 0.0, 0.0, 0.0 };
  double ViewAngle = 0.0;
  double ParallelScale = 0.0;
  int ParallelProjection = 0;

  void CaptureState(vtkCamera* const camera);
  void RestoreState(vtkCamera* const camera) const;

  void SerializeJSON(const std::string &fileName);
  void SerializeJSON(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

  void DeserializeJSON(const std::string &fileName);
  void DeserializeJSON(const rapidjson::Document& d);

}; // end class
} // end namespace vtk
#endif
