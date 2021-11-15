// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvMaskImage.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")
  ("input-image", po::value<std::string>()->required(), "Filename of the input image.")
  ("mask-image", po::value<std::string>()->required(), "Filename of the mask image.")
  ("output-image", po::value<std::string>()->required(), "Filename of the output image.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage = vm["input-image"].as<std::string>();
  const std::string MImage = vm["mask-image"].as<std::string>();
  const std::string OImage = vm["output-image"].as<std::string>();

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::UCHAR:
      dv::MaskImage<3, unsigned char>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::CHAR:
      dv::MaskImage<3, char>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::USHORT:
      dv::MaskImage<3, unsigned short>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::SHORT:
      dv::MaskImage<3, short>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::UINT:
      dv::MaskImage<3, unsigned int>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::INT:
      dv::MaskImage<3, int>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::ULONG:
      dv::MaskImage<3, unsigned long>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::LONG:
      dv::MaskImage<3, long>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::FLOAT:
      dv::MaskImage<3, float>(IImage, MImage, OImage);
      break;
    case itk::ImageIOBase::DOUBLE:
      dv::MaskImage<3, double>(IImage, MImage, OImage);
      break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
