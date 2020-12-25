// ITK
#include <itkQuadEdgeMesh.h>

// VTK
#include <vtkPolyDataReader.h>

// DV
#include <dvVTKPolyDataToITKTriangleMesh.h>
#include <dvQuickViewPolyData.h>

int main()
{

  vtkPolyDataReader * reader = vtkPolyDataReader::New();
  reader->SetFileName( "/Users/davisvigneault/Desktop/SiSSR-data/output/initial_models/initial_model.vtk" );
  reader->Update();

  const auto polydata = reader->GetOutput();

//  const unsigned int PointDimension   = 3;
//  const unsigned int MaxCellDimension = 2;
//
//  typedef itk::DefaultStaticMeshTraits< 
//                      float, 
//                      PointDimension,
//                      MaxCellDimension, 
//                      float, 
//                      float  >       MeshTraits;
//
//
//  typedef itk::Mesh<
//                      float, 
//                      PointDimension, 
//                      MeshTraits              >     MeshType;

  using MeshType = itk::QuadEdgeMesh< float, 3 >;

  const auto mesh = dv::VTKPolyDataToITKTriangleMesh<MeshType>( polydata );

  std::cout << "Mesh  " << std::endl;
  std::cout << "Number of Points =   " << mesh->GetNumberOfPoints() << std::endl;
  std::cout << "Number of Cells  =   " << mesh->GetNumberOfCells()  << std::endl;
  std::cout << "Cell Data = "          << mesh->GetCellData()->Size() << std::endl;
  std::cout << "Point Data = "         << mesh->GetPointData()->Size() << std::endl;

  for (auto it = mesh->GetCellData()->Begin(); it != mesh->GetCellData()->End(); ++it) {
    itkAssertOrThrowMacro(it.Value() != 0, "label == 0");
  }
  dv::QuickViewPolyData(polydata);

  return EXIT_SUCCESS;

}
