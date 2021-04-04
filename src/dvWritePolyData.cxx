#include <dvWritePolyData.h>

namespace dv
{
void WritePolyData(vtkPolyData* data, const std::string &fileName)
{
  std::string extension = vtksys::SystemTools::GetFilenameExtension(std::string(fileName));
  if (extension == ".ply")
  {
    const auto writer = vtkSmartPointer<vtkPLYWriter>::New();
    writer->SetFileName (fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else if (extension == ".vtp")
  {
    const auto writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else if (extension == ".obj")
  {
    const auto writer = vtkSmartPointer<vtkOBJWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else if (extension == ".stl")
  {
    const auto writer = vtkSmartPointer<vtkSTLWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else if (extension == ".vtk")
  {
    const auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName(fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else if (extension == ".g")
  {
    const auto writer = vtkSmartPointer<vtkBYUWriter>::New();
    writer->SetGeometryFileName(fileName.c_str());
    writer->SetInputData( data );
    writer->Update();
  }
  else
  {
    std::cerr << "Unrecognized extension--returning." << std::endl;
  }
}
}

