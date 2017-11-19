// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkImage.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef unsigned char  PixelType;
typedef itk::Image< PixelType, Dimension >   ImageType;

typedef itk::Mesh<float,Dimension>                         MeshType;
typedef itk::BinaryMask3DMeshSource< ImageType, MeshType >   MeshSourceType;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileWriter< TMesh >        TWriter;

int main( int argc, char* argv[] )
{

  if ( 4 != argc )
    {
    std::cerr << "Usage:\n"<< std::endl;
    std::cerr << argv[0] << " <InputImage> <OutputMesh> <LabelValue>" << std::endl;
    return EXIT_FAILURE;
    }

  const std::string inputFileName(argv[1]);
  const std::string outputFileName(argv[2]);

  //
  // Reader
  //

  typedef itk::ImageFileReader< ImageType >    ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName );

  try
      {
      reader->Update();
      }
  catch( itk::ExceptionObject & exp )
      {
      std::cerr << "Exception thrown while reading the input file " << std::endl;
      std::cerr << exp << std::endl;
      return EXIT_FAILURE;
      }

//
//Meshing
//
MeshSourceType::Pointer meshSource = MeshSourceType::New();
const PixelType objectValue = static_cast<PixelType>( std::atof( argv[3] ) );
meshSource->SetObjectValue( objectValue );
meshSource->SetInput( reader->GetOutput() );

try
    {
    meshSource->Update();
    }
  catch( itk::ExceptionObject & exp )
    {
    std::cerr << "Exception thrown during meshSource->Update() " << std::endl;
    std::cerr << exp << std::endl;
    return EXIT_FAILURE;
    }

    std::cout << "Nodes = " << meshSource->GetNumberOfNodes() << std::endl;
    std::cout << "Cells = " << meshSource->GetNumberOfCells() << std::endl;

  //
  // Writer
  //

  const auto writer = TWriter::New();
  writer->SetInput( meshSource->GetOutput() );
  writer->SetFileName( outputFileName );

  const auto o_ext = outputFileName.substr(outputFileName.size() - 3, 3);
  if (o_ext == "stl" || o_ext == "STL")
    {
    writer->SetMeshIO( itk::STLMeshIO::New() );
    }

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cerr << "There was a problem writing the file." << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
