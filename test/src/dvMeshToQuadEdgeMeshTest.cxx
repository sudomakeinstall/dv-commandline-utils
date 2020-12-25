#include <dvMeshToQuadEdgeMesh.h>
#include <itkMesh.h>
#include <itkQuadEdgeMesh.h>
#include <itkRegularSphereMeshSource.h>

const unsigned int Dimension = 3;
using TPixel = float;
using TMesh = itk::Mesh<TPixel, Dimension>;
using TQEMesh = itk::QuadEdgeMesh<TPixel, Dimension>;
using TSphere = itk::RegularSphereMeshSource<TMesh>;

int
main()
{

  const auto sphere = TSphere::New();
  sphere->Update();

  const auto mesh = TMesh::New();
  mesh->Graft(sphere->GetOutput());
  mesh->DisconnectPipeline();

  const auto qemesh = TQEMesh::New();

  dv::MeshToQuadEdgeMesh<TMesh, TQEMesh>(mesh, qemesh);

  std::cout << mesh->GetNumberOfPoints() << std::endl;
  std::cout << mesh->GetNumberOfCells() << std::endl;

  std::cout << qemesh->GetNumberOfPoints() << std::endl;
  std::cout << qemesh->GetNumberOfCells() << std::endl;

  return EXIT_SUCCESS;
}
