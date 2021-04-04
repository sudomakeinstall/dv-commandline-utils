#include <dvReadPolyData.h>

namespace dv
{
vtkSmartPointer<vtkPolyData> ReadPolyData(const std::string &fileName)
{
  vtkSmartPointer<vtkPolyData> polyData;
  std::string extension = vtksys::SystemTools::GetFilenameExtension(std::string(fileName));
  if (extension == ".ply")
  {
    const auto reader = vtkSmartPointer<vtkPLYReader>::New();
    reader->SetFileName (fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtp")
  {
    const auto reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".obj")
  {
    const auto reader = vtkSmartPointer<vtkOBJReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".stl")
  {
    const auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".vtk")
  {
    const auto reader = vtkSmartPointer<vtkPolyDataReader>::New();
    reader->SetFileName(fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else if (extension == ".g")
  {
    const auto reader = vtkSmartPointer<vtkBYUReader>::New();
    reader->SetGeometryFileName(fileName.c_str());
    reader->Update();
    polyData = reader->GetOutput();
  }
  else
  {
    std::cerr << "Unrecognized extension--returning." << std::endl;
  }
  return polyData;
}
}

