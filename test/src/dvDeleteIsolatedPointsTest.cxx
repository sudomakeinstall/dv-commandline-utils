#include <dvDeleteIsolatedPoints.h>
#include <dvSqueezePointsIds.h>
#include <itkDefaultDynamicMeshTraits.h>
#include <itkMesh.h>
#include <itkMeshFileWriter.h>
#include <itkTriangleCell.h>

const unsigned int Dimension = 2;
using TPixel = float;
using TMeshTraits = itk::DefaultDynamicMeshTraits<TPixel>;
using TMesh = itk::Mesh<TPixel, Dimension, TMeshTraits>;
using TPoint = TMesh::PointType;
using TCell = TMesh::CellType;
using TTriangle = itk::TriangleCell<TCell>;
using TMeshWriter = itk::MeshFileWriter<TMesh>;

int
main(int, char**)
{

  // 1------3
  //   \    |
  //      \ |
  // 0      2

  const auto mesh = TMesh::New();
  mesh->SetPoint(0, TPoint{ { 0.0, 0.0 } });
  mesh->SetPoint(1, TPoint{ { 0.0, 1.0 } });
  mesh->SetPoint(2, TPoint{ { 1.0, 0.0 } });
  mesh->SetPoint(3, TPoint{ { 1.0, 1.0 } });

  TCell::CellAutoPointer cellpointer;
  cellpointer.TakeOwnership(new TTriangle);
  cellpointer->SetPointId(0, 1);
  cellpointer->SetPointId(1, 2);
  cellpointer->SetPointId(2, 3);
  mesh->SetCell(0, cellpointer);

  itkAssertOrThrowMacro(4 == mesh->GetNumberOfPoints(),
                        "Wrong number of points.");
  itkAssertOrThrowMacro(1 == mesh->GetNumberOfCells(),
                        "Wrong number of cells.");

  dv::DeleteIsolatedPoints<TMesh>(mesh);

  itkAssertOrThrowMacro(3 == mesh->GetNumberOfPoints(),
                        "Wrong number of points.");
  itkAssertOrThrowMacro(1 == mesh->GetNumberOfCells(),
                        "Wrong number of cells.");

  dv::SqueezePointsIds<TMesh>(mesh);

  const auto writer = TMeshWriter::New();
  writer->SetInput(mesh);
  writer->SetFileName("mesh-unsqueezed.obj");
  writer->Update();

  return EXIT_SUCCESS;
}
