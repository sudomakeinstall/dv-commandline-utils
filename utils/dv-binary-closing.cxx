// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvBinaryClosing.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of input image.")("output-image",
                                po::value<std::string>()->required(),
                                "Filename of output image.")(
    "radius",
    po::value<unsigned int>()->default_value(3),
    "Radius of structuring element.");

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

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::BinaryClosing<3, unsigned char>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::CHAR:
      dv::BinaryClosing<3, char>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::USHORT:
      dv::BinaryClosing<3, unsigned short>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::SHORT:
      dv::BinaryClosing<3, short>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::UINT:
      dv::BinaryClosing<3, unsigned int>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::INT:
      dv::BinaryClosing<3, int>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::ULONG:
      dv::BinaryClosing<3, unsigned long>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::LONG:
      dv::BinaryClosing<3, long>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::BinaryClosing<3, float>(IImage, OImage, radius);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::BinaryClosing<3, double>(IImage, OImage, radius);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
