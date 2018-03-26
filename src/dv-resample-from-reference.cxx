// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvResampleFromReference.h>

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",           "Print usage information.")
    ("input-image",     po::value<std::string>()->required(), "Filename of the input image.")
    ("reference-image", po::value<std::string>()->required(), "Filename of the reference image.")
    ("output-image",    po::value<std::string>()->required(), "Filename of the output image.")
    ("output-size",     po::value<unsigned int>(), "Output image size in pixels.")
    ("output-spacing",  po::value<double>(), "Output pixel size in mm.")
    ("outside-value",  po::value<double>()->default_value(0.0), "Outside value.")
    ("interpolator", po::value<unsigned int>()->default_value(1), "Order of BSpline interpolation (0 to 5 allowed).")
;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help"))
    {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
    }

  po::notify(vm);

  const auto IImage(vm["input-image"].as<std::string>());
  const auto RImage(vm["reference-image"].as<std::string>());
  const auto OImage(vm["output-image"].as<std::string>());
  const unsigned int interpolator = vm["interpolator"].as<unsigned int>();

  const bool output_size_exists = vm.count("output-size");
  const auto OutputSize = output_size_exists ? vm["output-size"].as<unsigned int>() : -1;

  const bool output_spacing_exists = vm.count("output-spacing");
  const auto OutputSpacing = output_spacing_exists ? vm["output-spacing"].as<double>() : -1;

  const auto OutsideValue = vm["outside-value"].as<double>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::ResampleFromReference<3, unsigned char>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::CHAR:
      dv::ResampleFromReference<3, char>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::USHORT:
      dv::ResampleFromReference<3, unsigned short>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::SHORT:
      dv::ResampleFromReference<3, short>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::UINT:
      dv::ResampleFromReference<3, unsigned int>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::INT:
      dv::ResampleFromReference<3, int>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::ULONG:
      dv::ResampleFromReference<3, unsigned long>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::LONG:
      dv::ResampleFromReference<3, long>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::FLOAT:
      dv::ResampleFromReference<3, float>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::ResampleFromReference<3, double>(
        IImage,
        RImage,
        OImage,
        OutsideValue,
        output_size_exists,
        OutputSize,
        output_spacing_exists,
        OutputSpacing, interpolator
                                                 );
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}
