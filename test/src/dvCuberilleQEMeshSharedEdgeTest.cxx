#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkMeshFileWriter.h>

const unsigned int Dimension = 3;
using TPixel = unsigned char;
using TImage = itk::Image< TPixel, Dimension >;
//using TMesh = itk::Mesh<double, 3>;
using TMesh = itk::QuadEdgeMesh<double, 3>;
using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;
using TMeshWriter = itk::MeshFileWriter< TMesh >;

int main(int argc, char** argv)
{

  const auto image = TImage::New();
  TImage::RegionType region({{0,0,0}}, {{5,4,4}});
  image->SetBufferedRegion(region);
  image->Allocate();
  image->FillBuffer(0);

  // Z = 1:
  //   0 1 2 3 4
  // 0
  // 1   A B C
  // 2   D   E
  // 3

  image->SetPixel({{1, 1, 1}}, 1); // A
  image->SetPixel({{2, 1, 1}}, 1); // B
  image->SetPixel({{3, 1, 1}}, 1); // C
  image->SetPixel({{1, 2, 1}}, 1); // D
  image->SetPixel({{3, 2, 1}}, 1); // E

  // Z = 2:
  //    0 1 2 3 4
  //  0
  //  1          
  //  2   F G H
  //  3

  image->SetPixel({{1, 2, 2}}, 1); // F
  image->SetPixel({{2, 2, 2}}, 1); // G
  image->SetPixel({{3, 2, 2}}, 1); // H

  const auto extract = TExtract::New();
  extract->SetInput( image );
  extract->SavePixelAsCellDataOn();
  extract->GenerateTriangleFacesOff();
  extract->ProjectVerticesToIsoSurfaceOff();

  const auto m_writer = TMeshWriter::New();
  m_writer->SetInput( extract->GetOutput() );
  m_writer->SetFileName( "mesh.obj" );
  m_writer->Update();

  const auto n_cell = extract->GetOutput()->GetNumberOfCells();
  const auto n_data = extract->GetOutput()->GetCellData()->Size();

  if (n_cell != n_data) {
    std::cout << "This fails if using itk::QuadEdgeMesh." << std::endl;
    std::cout << "n_cell: " << n_cell << std::endl;
    std::cout << "n_data: " << n_data << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "This succeeds if using itk::Mesh." << std::endl;

  return EXIT_SUCCESS;

}
