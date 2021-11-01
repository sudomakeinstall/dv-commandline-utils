// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// ITK
#include "itkImageFileReader.h"
#include "itkMultiScaleHessianBasedMeasureImageFilter.h"
#include "itkHessianToObjectnessMeasureImageFilter.h"

#include "itkImageFileWriter.h"

// Typedefs
constexpr unsigned int Dimension = 3;

using PixelType = double;
using ImageType = itk::Image<PixelType, Dimension>;
using ReaderType = itk::ImageFileReader<ImageType>;
using HessianPixelType = itk::SymmetricSecondRankTensor<PixelType, Dimension>;
using HessianImageType = itk::Image<HessianPixelType, Dimension>;
using ObjectnessFilterType = itk::HessianToObjectnessMeasureImageFilter<HessianImageType, ImageType>;
using MultiScaleEnhancementFilterType =
  itk::MultiScaleHessianBasedMeasureImageFilter<ImageType, HessianImageType, ImageType>;
using WriterType = itk::ImageFileWriter<ImageType>;

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("alpha", po::value<double>()->default_value(0.5), "Alpha.")
    ("beta", po::value<double>()->default_value(1.0), "Beta.")
    ("gamma", po::value<double>()->default_value(5.0), "Gamma.")
    ("sigma-min", po::value<double>()->default_value(2.0), "Sigma minimum.")
    ("sigma-max", po::value<double>()->default_value(2.0), "Sigma maximum.")
    ("sigma-steps", po::value<unsigned int>()->default_value(1), "Sigma steps.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const double alpha = vm["alpha"].as<double>();
  const double beta = vm["beta"].as<double>();
  const double gamma = vm["gamma"].as<double>();
  const double sigma_min = vm["sigma-min"].as<double>();
  const double sigma_max = vm["sigma-max"].as<double>();
  const unsigned int sigma_steps = vm["sigma-steps"].as<unsigned int>();

  const auto reader = ReaderType::New();
  reader->SetFileName(IImage);

  const auto objectnessFilter = ObjectnessFilterType::New();
  objectnessFilter->SetBrightObject(true);
  objectnessFilter->SetScaleObjectnessMeasure(false);
  objectnessFilter->SetAlpha(alpha);
  objectnessFilter->SetBeta(beta);
  objectnessFilter->SetGamma(gamma);
 
  const auto multiScaleEnhancementFilter = MultiScaleEnhancementFilterType::New();
  multiScaleEnhancementFilter->SetInput( reader->GetOutput() );
  multiScaleEnhancementFilter->SetHessianToMeasureFilter(objectnessFilter);
  multiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
  multiScaleEnhancementFilter->SetSigmaMinimum(sigma_min);
  multiScaleEnhancementFilter->SetSigmaMaximum(sigma_max);
  multiScaleEnhancementFilter->SetNumberOfSigmaSteps(sigma_steps);

  const auto writer = WriterType::New();
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
