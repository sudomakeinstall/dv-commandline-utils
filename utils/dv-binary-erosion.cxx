// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvBinaryErosion.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")
    ("input-image", po::value<std::string>()->required(), "Filename of input image.")
    ("output-image", po::value<std::string>()->required(), "Filename of output image.")
    ("radius", po::value<unsigned int>()->default_value(3), "Radius of structuring element.")
    ("foreground", po::value<unsigned int>()->default_value(1), "Foreground value.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();
  const unsigned int radius = vm["radius"].as<unsigned int>();
  const unsigned int foreground = vm["foreground"].as<unsigned int>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::BinaryErosion<3, unsigned char>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::CHAR:
      dv::BinaryErosion<3, char>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::USHORT:
      dv::BinaryErosion<3, unsigned short>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::SHORT:
      dv::BinaryErosion<3, short>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::UINT:
      dv::BinaryErosion<3, unsigned int>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::INT:
      dv::BinaryErosion<3, int>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::ULONG:
      dv::BinaryErosion<3, unsigned long>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::LONG:
      dv::BinaryErosion<3, long>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::BinaryErosion<3, float>(IImage, OImage, radius, foreground);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::BinaryErosion<3, double>(IImage, OImage, radius, foreground);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
