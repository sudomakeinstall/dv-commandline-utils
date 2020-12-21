// ITK
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileReader.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshEulerOperatorDeleteCenterVertexFunction.h>

//// Custom
//#include <dvITKMeshToVTKPolyData.h>
//#include <dvQuickViewSideBySidePolyData.h>
//
const unsigned int Dimension = 3;
using TCoordinate = float;
using TMesh = itk::QuadEdgeMesh< TCoordinate, Dimension >;
using TReader = itk::MeshFileReader< TMesh >;
using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<TMesh, TMesh>;
using TDeleteVertex = itk::QuadEdgeMeshEulerOperatorDeleteCenterVertexFunction< TMesh, typename TMesh::QEType >;

int main(int argc, char** argv) {

  if (argc < 2) { std::cerr << "Please supply data dir." << std::endl; }

  const std::string test_data_dir(argv[1]);
  const std::string mesh_file = test_data_dir + "valence-three.obj";

  const auto reader = TReader::New();
  reader->SetFileName( mesh_file );
  reader->Update();

  const auto mesh = TMesh::New();
  mesh->Graft( reader->GetOutput() );

  std::cout << mesh->GetNumberOfPoints() << std::endl;
//  const auto i_polydata = dv::ITKMeshToVTKPolyData<TMesh>( mesh );
//
  for (auto it = mesh->GetPoints()->Begin(); it != mesh->GetPoints()->End(); ++it) {
    if (3 != it.Value().GetValence()) { continue; }
    auto qe = mesh->FindEdge( it.Index() );
    if (3 != mesh->GetPoint(qe->GetDestination()).GetValence()) {
      qe = qe->GetSym();
    }
    const auto delete_vertex = TDeleteVertex::New();
    delete_vertex->SetInput( mesh );
    delete_vertex->Evaluate( qe );
    mesh->DeletePoint(delete_vertex->GetOldPointID());
    break;
  }

  std::cout << mesh->GetNumberOfPoints() << std::endl;
  mesh->SqueezePointsIds();

  const auto loop = TLoop::New();
  loop->SetInput( mesh );
  loop->Update();
//
//  const auto o_polydata = dv::ITKMeshToVTKPolyData<TMesh>( mesh );
//
//  dv::QuickViewSideBySidePolyData(i_polydata, o_polydata);

  return EXIT_SUCCESS;

}
