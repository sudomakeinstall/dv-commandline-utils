//
//// Boost
//#include <boost/program_options.hpp>
//
//namespace po = boost::program_options;
//
////VTK
//#include <vtkSmartPointer.h>
//#include <vtkOBJImporter.h>
//#include <vtkRenderer.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkPolyDataNormals.h>
//#include <vtkCleanPolyData.h>
//#include <vtkPolyData.h>
//#include <vtkTexture.h>
//#include <vtkNamedColors.h>
//#include <vtkTriangleFilter.h>
//#include <vtkQuadricDecimation.h>
//#include <vtkOBJExporter.h>
//
//int main (int argc, char *argv[])
//{
//
//  // Declare the supported options.
//  po::options_description description("Allowed options");
//  description.add_options()
//    ("help", "Print usage information.")
//    ("input-mesh",      po::value<std::string>()->required(),    "Filename of the input mesh.")
//    ("material-file",   po::value<std::string>()->required(),    "Filename of the material file.")
//    ("output-mesh",     po::value<std::string>()->required(),    "Filename of the output image.")
//    ("target",          po::value<double>()->default_value(0.1),   "Target reduction (0,1].")
//    ("change-topology", po::value<bool>()->default_value(false), "Allow change in topology?")
//    ("verbose",         po::value<bool>()->default_value(false), "Verbosity.")
//  ;
//
//  po::variables_map vm;
//  po::store(po::parse_command_line(argc, argv, description), vm);
//
//  if (vm.count("help") || 1 == argc)
//    {
//    std::cout << description << '\n';
//    return EXIT_SUCCESS;
//    }
//
//  po::notify(vm);
//
//  const std::string inputMeshName(vm["input-mesh"].as<std::string>());
//  const std::string materialFileName(vm["material-file"].as<std::string>());
//  const std::string outputMeshName(vm["output-mesh"].as<std::string>());
//  const unsigned int target = vm["target"].as<double>();
//  const bool verbose = vm["verbose"].as<bool>();
//  const bool topology = vm["change-topology"].as<bool>();
//
//  vtkNew<vtkOBJImporter> importer;
//  importer->SetFileName(inputMeshName.c_str());
//  importer->SetFileNameMTL(materialFileName.c_str());
//
//  vtkNew<vtkRenderer> renderer;
//  vtkNew<vtkRenderWindow> renWin;
//  vtkNew<vtkRenderWindowInteractor> iren;
//
//  renWin->AddRenderer(renderer);
//  renderer->UseHiddenLineRemovalOn();
//  renWin->AddRenderer(renderer);
//
////  iren->SetRenderWindow(renWin);
////  importer->SetRenderWindow(renWin);
////  importer->Update();
////
////////  vtkNew<vtkActorCollection> actors;
//////  vtkSmartPointer<vtkActorCollection> actors =
//////    vtkSmartPointer<vtkActorCollection>::New();
//////  actors = renderer->GetActors();
//////  actors->InitTraversal();
////////  std::cout << "There are " << actors->GetNumberOfItems() << " actors" << std::endl;
//////
//////  for (vtkIdType a = 0; a < actors->GetNumberOfItems(); ++a)
//////  {
//////    std::cout << importer->GetOutputDescription(a) << std::endl;
//////
//////    vtkActor * actor = actors->GetNextActor();
//////
//////    vtkPolyData *pd = dynamic_cast<vtkPolyData*>(actor->GetMapper()->GetInput());
//////
//////    vtkNew<vtkTriangleFilter> triangulate;
//////    triangulate->SetInputData(pd);
//////
//////    vtkNew<vtkQuadricDecimation> decimate;
//////    decimate->SetInputConnection(triangulate->GetOutputPort());
//////    decimate->SetTargetReduction( target );
//////    decimate->Update();
//////    vtkPolyDataMapper *mapper = dynamic_cast<vtkPolyDataMapper*>(actor->GetMapper());
//////    mapper->SetInputData(decimate->GetOutput());
//////
//////  }
////  renWin->Render();
////  iren->Start();
//////
//////  vtkNew<vtkOBJExporter> exporter;
//////  exporter->SetRenderWindow( renWin );
//////  exporter->SetFilePrefix( outputMeshName.c_str() );
//////  exporter->Write();
//
//  return EXIT_SUCCESS;
//}





#include <vtkSmartPointer.h>
#include <vtkOBJImporter.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyData.h>
#include <vtkTexture.h>
#include <vtkNamedColors.h>

int main (int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cout << "Usage: " << argv[0]
              << " objfile mtlfile texturepath"
              << std::endl;
    return EXIT_FAILURE;
  }
  vtkSmartPointer<vtkOBJImporter> importer =
    vtkSmartPointer<vtkOBJImporter>::New();
  importer->SetFileName(argv[1]);
  importer->SetFileNameMTL(argv[2]);
  importer->SetTexturePath(argv[3]);

  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();

  renderer->SetBackground2 (colors->GetColor3d("Silver").GetData());
  renderer->SetBackground (colors->GetColor3d("Gold").GetData());
  renderer->GradientBackgroundOn();
  renWin->AddRenderer(renderer);
  renderer->UseHiddenLineRemovalOn();
  renWin->AddRenderer(renderer);
  renWin->SetSize(640, 480);

  iren->SetRenderWindow(renWin);
  importer->SetRenderWindow(renWin);
  importer->Update();

  vtkSmartPointer<vtkActorCollection> actors =
    vtkSmartPointer<vtkActorCollection>::New();
  actors = renderer->GetActors();
  actors->InitTraversal();
  std::cout << "There are " << actors->GetNumberOfItems() << " actors" << std::endl;

  for (vtkIdType a = 0; a < actors->GetNumberOfItems(); ++a)
  {
    std::cout << importer->GetOutputDescription(a) << std::endl;

    vtkActor * actor = actors->GetNextActor();

    // OBJImporter turns texture interpolation off
    if (actor->GetTexture())
    {
      std::cout << "Hastexture\n";
      actor->GetTexture()->InterpolateOn();
    }

    vtkPolyData *pd = dynamic_cast<vtkPolyData*>(actor->GetMapper()->GetInput());
    vtkSmartPointer<vtkCleanPolyData> clean =
      vtkSmartPointer<vtkCleanPolyData>::New();
    clean->SetInputData(pd);
    clean->Update();

    vtkSmartPointer<vtkPolyDataNormals> normals =
      vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputData(pd);
    normals->SplittingOff();
    normals->ConsistencyOn();
    normals->Update();
    vtkPolyDataMapper *mapper = dynamic_cast<vtkPolyDataMapper*>(actor->GetMapper());
    mapper->SetInputData(normals->GetOutput());
    mapper->SetInputData(pd);
  }
  renWin->Render();
  iren->Start();

  return EXIT_SUCCESS;
}
