// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include <itkImageFileReader.h>
#include <itkClampImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkGradientAnisotropicDiffusionImageFilter.h>
#include <itkMultiScaleHessianBasedMeasureImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>

#include <itkImageFileWriter.h>

// Typedefs
constexpr unsigned int Dimension = 3;

using TPixel = double;
using TImage = itk::Image<TPixel, Dimension>;
using TReader = itk::ImageFileReader<TImage>;
using TClamp = itk::ClampImageFilter<TImage, TImage>;
using TRescale = itk::RescaleIntensityImageFilter<TImage>;
using TDiffusion = itk::GradientAnisotropicDiffusionImageFilter<TImage, TImage>;
using THessianPixel = itk::SymmetricSecondRankTensor<TPixel, Dimension>;
using THessianImage = itk::Image<THessianPixel, Dimension>;
using TVesselnessFilter = itk::Hessian3DToVesselnessMeasureImageFilter<TPixel>;
using TMultiScaleEnhancement =
  itk::MultiScaleHessianBasedMeasureImageFilter<TImage, THessianImage, TImage>;
using TWriter = itk::ImageFileWriter<TImage>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("clamp-lower", po::value<double>()->default_value(-700.0), "Clamp lower.")
    ("clamp-upper", po::value<double>()->default_value(1500.0), "Clamp upper.")
    ("iterations", po::value<unsigned int>()->default_value(10), "Iterations.")
    ("time-step", po::value<double>()->default_value(0.0625), "Time step.")
    ("conductance", po::value<double>()->default_value(1.25), "Conductance.")
    ("alpha1", po::value<double>()->default_value(0.5), "Alpha1.")
    ("alpha2", po::value<double>()->default_value(2.5), "Alpha2.")
    ("sigma-min", po::value<double>()->default_value(0.2), "Sigma minimum.")
    ("sigma-max", po::value<double>()->default_value(3.0), "Sigma maximum.")
    ("sigma-steps", po::value<unsigned int>()->default_value(8), "Sigma steps.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const double clamp_lower = vm["clamp-lower"].as<double>();
  const double clamp_upper = vm["clamp-upper"].as<double>();
  const unsigned int iterations = vm["iterations"].as<unsigned int>();
  const double time_step = vm["time-step"].as<double>();
  const double conductance = vm["conductance"].as<double>();
  const double alpha1 = vm["alpha1"].as<double>();
  const double alpha2 = vm["alpha2"].as<double>();
  const double sigma_min = vm["sigma-min"].as<double>();
  const double sigma_max = vm["sigma-max"].as<double>();
  const unsigned int sigma_steps = vm["sigma-steps"].as<unsigned int>();

  const auto reader = TReader::New();
  reader->SetFileName(IImage);

  const auto clamp = TClamp::New();
  clamp->SetInput( reader->GetOutput() );
  clamp->SetBounds(clamp_lower, clamp_upper);

  const auto rescale = TRescale::New();
  rescale->SetInput( clamp->GetOutput() );
  rescale->SetOutputMaximum( 1.0 );
  rescale->SetOutputMinimum( 0.0 );

  const auto diffusion = TDiffusion::New();
  diffusion->SetInput( rescale->GetOutput() );
  diffusion->SetNumberOfIterations( iterations );
  diffusion->SetTimeStep( time_step );
  diffusion->SetConductanceParameter( conductance );

  const auto vesselness = TVesselnessFilter::New();
  vesselness->SetAlpha1(alpha1);
  vesselness->SetAlpha2(alpha2);
 
  const auto multiScaleEnhancementFilter = TMultiScaleEnhancement::New();
  multiScaleEnhancementFilter->SetInput( reader->GetOutput() );
  multiScaleEnhancementFilter->SetHessianToMeasureFilter(vesselness);
  multiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
  multiScaleEnhancementFilter->SetSigmaMinimum(sigma_min);
  multiScaleEnhancementFilter->SetSigmaMaximum(sigma_max);
  multiScaleEnhancementFilter->SetNumberOfSigmaSteps(sigma_steps);

  const auto writer = TWriter::New();
  writer->SetFileName(OImage);
  writer->SetInput(multiScaleEnhancementFilter->GetOutput());

  try {
    writer->Update();
  }
  catch (itk::ExceptionObject & error) {
    std::cerr << "Error: " << error << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}
