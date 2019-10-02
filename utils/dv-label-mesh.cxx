#include <itkVector.h>
#include <itkQuadEdgeMesh.h>
#include <itkMeshFileReader.h>
#include <itkQuadEdgeMeshExtendedTraits.h>
#include <itkNormalQuadEdgeMeshFilter.h>

#include <itkImage.h>
#include <itkImageFileReader.h>

constexpr unsigned int Dimension = 3;

using TCoord = double;
using TInputMesh = itk::QuadEdgeMesh< TCoord, Dimension >;
using TVector = itk::Vector< TCoord, Dimension >;
using Traits = itk::QuadEdgeMeshExtendedTraits <
  TVector,
  Dimension,
  2,
  TCoord,
  TCoord,
  TVector,
  bool,
  bool >;
using TMeshReader = itk::MeshFileReader< TInputMesh >;
using TOutputMesh = itk::QuadEdgeMesh< TVector, Dimension, Traits >;
using TNormal = itk::NormalQuadEdgeMeshFilter< TInputMesh, TOutputMesh >;

using TImageReader = itk::Image< Dimension, double >;

int main( int argc, char** argv )
{

  if ( argc < 2 )
    {
    std::cerr << "Usage:" <<std::endl;
    std::cerr << argv[0] << "<InputFileName>" << std::endl;
    return EXIT_FAILURE;
    }

  const auto reader = TMeshReader::New( );
  reader->SetFileName( argv[1] );

  const auto normals = TNormal::New( );
  normals->SetInput( reader->GetOutput() );
  normals->Update( );

  const auto output = normals->GetOutput( );

  auto cells = output->GetCells();
  auto c_it = cells->Begin();

  auto data = output->GetCellData();
  auto d_it = data->Begin();

  while ( c_it != cells->End() )
    {

    TOutputMesh::PointType points[3];

    for (size_t i = 0; i < c_it.Value()->GetNumberOfPoints(); ++i)
      {
      points[i] = reader->GetOutput()->GetPoint( c_it.Value()->GetPointIds()[i] );
      }

    TOutputMesh::PointType center;

    center.SetToBarycentricCombination( points[0], points[1], points[2], 1.0, 1.0 );

    std::cout << "Center: " << center << std::endl;
    std::cout << "Normal: " << d_it.Value() << std::endl;
    std::cout << "Normal: " << d_it.Value() * -0.5 << std::endl << std::endl;

    ++c_it;
    ++d_it;
    }

  return EXIT_SUCCESS;

}
