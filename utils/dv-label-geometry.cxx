// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvLabelGeometry.h>
#include <dvReadImageIOBase.h>

int
main(int argc, char** argv)
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()("help", "Print usage information.")(
    "input-image",
    po::value<std::string>()->required(),
    "Filename of the input image.");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, description), vm);

  if (vm.count("help") || 1 == argc) {
    std::cout << description << '\n';
    return EXIT_SUCCESS;
  }

  po::notify(vm);

  const std::string IImage(vm["input-image"].as<std::string>());

  switch (dv::ReadImageIOBase(IImage)->GetComponentType()) {
    case itk::ImageIOBase::DOUBLE: {
      dv::LabelGeometry<3, double>(IImage);
      break;
    }
    case itk::ImageIOBase::FLOAT: {
      dv::LabelGeometry<3, float>(IImage);
      break;
    }
    case itk::ImageIOBase::UCHAR: {
      dv::LabelGeometry<3, unsigned char>(IImage);
      break;
    }
    case itk::ImageIOBase::CHAR: {
      dv::LabelGeometry<3, char>(IImage);
      break;
    }
    case itk::ImageIOBase::USHORT: {
      dv::LabelGeometry<3, unsigned short>(IImage);
      break;
    }
    case itk::ImageIOBase::SHORT: {
      dv::LabelGeometry<3, short>(IImage);
      break;
    }
    case itk::ImageIOBase::UINT: {
      dv::LabelGeometry<3, unsigned int>(IImage);
      break;
    }
    case itk::ImageIOBase::INT: {
      dv::LabelGeometry<3, int>(IImage);
      break;
    }
    case itk::ImageIOBase::ULONG: {
      dv::LabelGeometry<3, unsigned long>(IImage);
      break;
    }
    case itk::ImageIOBase::LONG: {
      dv::LabelGeometry<3, long>(IImage);
      break;
    }
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
  }

  return EXIT_SUCCESS;
}
