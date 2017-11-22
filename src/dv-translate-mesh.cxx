// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkMesh.h>
#include <itkMeshFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkTranslationTransform.h>
#include <itkTransformMeshFilter.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileReader< TMesh >        TReader;
typedef itk::MeshFileWriter< TMesh >        TWriter;
typedef itk::TranslationTransform< TCoordinate, Dimension > TTranslate;
typedef itk::TransformMeshFilter< TMesh, TMesh, TTranslate > TTransform;

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-mesh",  po::value<std::string>()->required(), "Filename of the input mesh.")
    ("output-mesh", po::value<std::string>()->required(), "Filename of the output image.")
    ("x",           po::value<double>()->required(), "Translation in the x direction.")
    ("y",           po::value<double>()->required(), "Translation in the y direction.")
    ("z",           po::value<double>()->required(), "Translation in the z direction.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string inputFileName(vm["input-mesh"].as<std::string>());
  const std::string outputFileName(vm["output-mesh"].as<std::string>());

  TCoordinate T[3] = {0.0, 0.0, 0.0};
  T[0] = vm["x"].as<double>();
  T[1] = vm["y"].as<double>();
  T[2] = vm["z"].as<double>();

  const auto reader = TReader::New();
  reader->SetFileName( inputFileName );
  reader->SetMeshIO( itk::STLMeshIO::New() );

  TTranslate::OutputVectorType displacement;
  for (std::size_t i = 0; i < 3; ++i) displacement[i] = T[i];

  const auto translate = TTranslate::New();
  translate->Translate( displacement );

  const auto transform = TTransform::New();
  transform->SetInput( reader->GetOutput() );
  transform->SetTransform( translate );

  const auto writer = TWriter::New();
  writer->SetInput( transform->GetOutput() );
  writer->SetFileName( outputFileName );
  writer->SetMeshIO( itk::STLMeshIO::New() );

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

