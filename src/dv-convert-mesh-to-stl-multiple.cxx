// Boost
#include <boost/program_options.hpp>

// ITK
#include <itkMesh.h>
#include <itkImageFileReader.h>
#include <itkMeshFileWriter.h>
#include <itkSTLMeshIO.h>
#include <itkBinaryMask3DMeshSource.h>
#include <itkImage.h>
#include <itkAddImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>

const unsigned int Dimension = 3;
typedef float      TCoordinate;

typedef unsigned char  PixelType;
typedef itk::Image< PixelType, Dimension >   ImageType;

typedef itk::Mesh<float,Dimension>                         MeshType;
typedef itk::BinaryMask3DMeshSource< ImageType, MeshType >   MeshSourceType;

typedef itk::Mesh< TCoordinate, Dimension > TMesh;
typedef itk::MeshFileWriter< TMesh >        TWriter;

typedef itk::BinaryThresholdImageFilter<ImageType, ImageType> TFilter;
typedef itk::AddImageFilter <ImageType, ImageType > AddImageFilterType;

int main( int argc, char* argv[] )
{

  if ( 4 > argc )
  {
    std::cerr << "Usage:\n"<< std::endl;
    std::cerr << argv[0] << " <InputImage> <OutputMesh> <LabelValue1> <LabelValue2> .... <LabelValueN>" << std::endl;
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
  // Adding Filtering - Add the first two labels
  //
  //std::cerr << "argv[3] value " << argv[3] <<std::endl;
  const auto filter1 = TFilter::New();
  filter1->SetInput(reader->GetOutput());
  filter1->SetInsideValue( 1 );
  filter1->SetOutsideValue( 0 );
  filter1->SetLowerThreshold( std::atof( argv[3] ) );
  filter1->SetUpperThreshold( std::atof( argv[3] ) );

  //std::cerr << "argv[4] value " << argv[4] <<std::endl;
  const auto filter2 = TFilter::New();
  filter2->SetInput(reader->GetOutput());
  filter2->SetInsideValue( 1 );
  filter2->SetOutsideValue( 0 );
  filter2->SetLowerThreshold( std::atof( argv[4] ) );
  filter2->SetUpperThreshold( std::atof( argv[4] ) );

  AddImageFilterType::Pointer addFilter  = AddImageFilterType::New ();
  addFilter->SetInput1(filter1->GetOutput());
  addFilter->SetInput2(filter2->GetOutput());

  addFilter->Update();

  // Add any remaining labels
  for (int i = 5; i < argc; ++i)
  {
    const auto filter3 = TFilter::New();
    filter3->SetInput(reader->GetOutput());
    filter3->SetInsideValue( 1 );
    filter3->SetOutsideValue( 0 );
    filter3->SetLowerThreshold( std::atof( argv[i] ) );
    filter3->SetUpperThreshold( std::atof( argv[i] ) );

    addFilter->SetInput1(addFilter->GetOutput());
    addFilter->SetInput2(filter3->GetOutput());

    addFilter->Update();
  }

  // Turn image back into binary
  const auto filter4 = TFilter::New();
  filter4->SetInput(addFilter->GetOutput());
  filter4->SetInsideValue( 1 );
  filter4->SetOutsideValue( 0 );
  filter4->SetLowerThreshold( 1 );
  filter4->SetUpperThreshold( 1000 );

  //
  //Meshing
  //
  MeshSourceType::Pointer meshSource = MeshSourceType::New();
  const PixelType objectValue = static_cast<PixelType>( 1 );
  meshSource->SetObjectValue( objectValue );
  meshSource->SetInput( filter4->GetOutput() );

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
