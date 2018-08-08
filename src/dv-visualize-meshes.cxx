// Boost
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;

// RapidJSON
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

// VTK
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkNIFTIImageReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkProperty.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCallbackCommand.h>
#include <vtkExtractVOI.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

// Custom
#include <dvNumberOfSequentialFiles.h>
#include <dvCycle.h>
#include <dvStringOperations.h>
#include <dvProgress.h>
#include <dvCameraState.h>

// Define interaction style
namespace dv
{
class KeyPressInteractorStyle
: public vtkInteractorStyleTrackballCamera
{

  public:

    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
 
    void OnKeyPress() override
      {
      vtkRenderWindowInteractor *rwi = this->Interactor;
      const std::string key = rwi->GetKeySym();
 
      // Increment
      if (this->IncrementKeys.find(key) != this->IncrementKeys.cend())
        {
        this->Increment();
        }
      // Decrement
      else if (this->DecrementKeys.find(key) != this->DecrementKeys.cend())
        {
        this->Decrement();
        }
      // Take Screenshots
      else if (this->ScreenshotKeys.find(key) != this->ScreenshotKeys.cend())
        {
        this->CaptureScreenshots();
        }
      // Restore Camera State
      else if (this->RestoreCameraStateKeys.find(key) != this->RestoreCameraStateKeys.cend())
        {
        this->RestoreCameraState();
        }
 
      // Forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
      }

  void Increment()
    {
    this->index.Increment();
    this->UpdateReader();
    }

  void Decrement()
    {
    this->index.Decrement();
    this->UpdateReader();
    }

  void UpdateReader()
    {
    const auto fn = this->directory + std::to_string(this->index.GetCurrent()) + ".nii.gz";
    this->reader->SetFileName( fn.c_str() );
    this->reader->Update();
    for (const auto &c : cubes) c->Update();
    this->GetCurrentRenderer()->GetRenderWindow()->Render();
    }

  void CaptureScreenshots()
    {
    if (!this->screenshot_dir_exists)
      {
      std::cerr << "No screenshot directory was set." << std::endl;
      return;
      }
    const std::string time = std::to_string(std::time(nullptr));
    const std::string folder = this->screenshot_dir + time;
    if (!boost::filesystem::create_directories(boost::filesystem::path(folder)))
      {
      std::cerr << "The directory " << folder << " could not be created." << std::endl;
      return;
      }
    std::cout << "Saving screenshots to " << folder << "..." << std::endl;

    this->camera.CaptureState(this->GetCurrentRenderer()->GetActiveCamera());

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    writer.StartObject();

    camera.SerializeJSON(writer);

    writer.EndObject();

    std::string cameraStateFileName = this->screenshot_dir + time + "/camera-state.json";
    std::ofstream fileStream;
    fileStream.open(cameraStateFileName);
    fileStream << sb.GetString();
    fileStream.close();

    auto progress = dv::Progress( this->index.GetRange() - this->index.GetStart() );

    const auto current = this->index.GetCurrent();

    do
      {
      const auto screenshot = vtkSmartPointer<vtkWindowToImageFilter>::New();
      screenshot->SetInput( this->GetCurrentRenderer()->GetRenderWindow() );
      screenshot->SetInputBufferTypeToRGBA();
      screenshot->SetFixBoundary(true);
      screenshot->Update();

      const auto writer = vtkSmartPointer<vtkPNGWriter>::New();
      const auto path = folder + "/" + std::to_string(this->index.GetCurrent()) + ".png";
      writer->SetFileName( path.c_str() );
      writer->SetInputConnection( screenshot->GetOutputPort() );
      writer->Write();

      progress.UnitCompleted();
      this->Increment();
      }
    while (current != this->index.GetCurrent());

    }

  void RestoreCameraState()
    {

    if (!this->camera_state_exists)
      {
      std::cerr << "No camera state was provided." << std::endl;
      return;
      }

    if (!boost::filesystem::exists(this->camera_state))
      {
      std::cerr << "A camera state was provided, but the file does not exist." << std::endl;
      }

    std::ifstream file_stream;
    file_stream.open(this->camera_state);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    file_stream.close();

    rapidjson::Document d;
    d.Parse(buffer.str().c_str());

    this->camera.DeserializeJSON(d);
    this->camera.RestoreState(this->GetCurrentRenderer()->GetActiveCamera());

    }

  dv::Cycle<unsigned int> index{1};
  std::string directory;

  bool screenshot_dir_exists;
  std::string screenshot_dir;

  bool camera_state_exists;
  std::string camera_state;

  vtkSmartPointer<vtkNIFTIImageReader> reader;
  std::vector<vtkSmartPointer<vtkDiscreteMarchingCubes>> cubes;
  std::set<std::string> IncrementKeys{"Down", "Right", "j", "l"};
  std::set<std::string> DecrementKeys{"Up", "Left", "h", "k"};
  std::set<std::string> ScreenshotKeys{"s", "p"};
  std::set<std::string> RestoreCameraStateKeys{"r"};

  dv::CameraState camera;

};
}
vtkStandardNewMacro(dv::KeyPressInteractorStyle);

int
main( int argc, char ** argv )
{
 
  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-directory", po::value<std::string>()->required(), "Directory containing segmentation images named *.nii.gz.")
    ("labels", po::value<std::vector<unsigned int>>()->multitoken()->required(), "Space-separated list of lables to visualize, e.g.: 0 1 2 3")
    ("screenshot-directory", po::value<std::string>(), "Directory in which to save screenshots.")
    ("camera-state", po::value<std::string>(), "JSON file containing the saved camera state.")
    ("downsampling-factor", po::value<double>()->default_value(1.0), "Downsampling factor.")
    ("window-size", po::value<unsigned int>()->default_value(512), "Window size.")
    ("restore-capture-quit", "Restore camera state, capture screenshots, and quit.")
  ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const std::string dn = dv::AppendCharacterIfAbsent(vm["input-directory"].as<std::string>(), '/');
  const std::vector<unsigned int> labels = vm["labels"].as<std::vector<unsigned int>>();
  const double SampleRate = vm["downsampling-factor"].as<double>();
  const unsigned int WindowSize = vm["window-size"].as<unsigned int>();
  const unsigned int NumberOfFiles = dv::NumberOfSequentialFiles([dn](size_t n){ return dn + std::to_string(n) + ".nii.gz"; });

  const bool screenshot_dir_exists = vm.count("screenshot-directory");
  const std::string screenshot_dir = screenshot_dir_exists ? dv::AppendCharacterIfAbsent(vm["screenshot-directory"].as<std::string>(), '/') : "";

  const bool camera_state_exists = vm.count("camera-state");
  const std::string camera_state = camera_state_exists ? vm["camera-state"].as<std::string>() : "";

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();

  unsigned int frameid = 0;

  const auto reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
  reader->SetFileName( (dn + std::to_string(frameid) + ".nii.gz").c_str() );
  reader->Update();

  const auto voi = vtkSmartPointer<vtkExtractVOI>::New();
  voi->SetInputConnection( reader->GetOutputPort() );
  voi->SetSampleRate( SampleRate, SampleRate, SampleRate );
  voi->SetVOI( reader->GetOutput()->GetExtent() );

  renderer->SetBackground( 1.0, 1.0, 1.0 );
  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->AddRenderer( renderer );
  window->SetSize( WindowSize, WindowSize );
  const auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  interactor->EnableRenderOn();

  const auto style = vtkSmartPointer<dv::KeyPressInteractorStyle>::New();
  style->reader = reader;
  style->directory = dn;

  style->screenshot_dir_exists = screenshot_dir_exists;
  style->screenshot_dir = screenshot_dir;

  style->camera_state_exists = camera_state_exists;
  style->camera_state = camera_state;

  style->index = dv::Cycle<unsigned int>{NumberOfFiles};
  interactor->SetInteractorStyle( style );
  style->SetCurrentRenderer( renderer );

  interactor->SetRenderWindow( window );
  std::vector<std::array<double, 3>> colors;
  colors.emplace_back(std::array<double, 3>{  0.0/255,    0.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,  127.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  127.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,    0.0/255,  127.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,   27.0/255,  155.0/255});
  colors.emplace_back(std::array<double, 3>{155.0/255,    0.0/255,   27.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,    0.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,  255.0/255,  127.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{  0.0/255,    0.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{255.0/255,  127.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,  127.0/255,  255.0/255});
  colors.emplace_back(std::array<double, 3>{127.0/255,  127.0/255,  127.0/255});

  for (const auto &l : labels)
    {
    const auto cubes = vtkSmartPointer<vtkDiscreteMarchingCubes>::New();
    cubes->SetInputConnection( voi->GetOutputPort() );

    cubes->SetValue( 0, l );

    const auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection( cubes->GetOutputPort() );
    mapper->ScalarVisibilityOff();
    const auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( mapper );
    actor->GetProperty()->SetColor( colors.at(l).data() );
    renderer->AddActor( actor );
    style->cubes.push_back(cubes);
    }


  window->Render();

  if (vm.count("restore-capture-quit"))
    {
    style->RestoreCameraState();
    style->CaptureScreenshots();
    window->Finalize();
    }
  else
    {
    interactor->Start();
    }

  return EXIT_SUCCESS;
 

}
