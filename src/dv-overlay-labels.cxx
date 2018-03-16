// Boost
#include <boost/program_options.hpp>

namespace po = boost::program_options;

// Custom
#include <dvReadImageIOBase.h>
#include <dvOverlayLabels.h>

//ITK Includes
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkLabelObject.h>
#include <itkLabelMap.h>
#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelMapOverlayImageFilter.h>

int
main( int argc, char* argv[] )
{

  // Declare the supported options.
  po::options_description description("Allowed options");
  description.add_options()
    ("help",           "Print usage information.")
    ("input-image",     po::value<std::string>()->required(), "Filename of the input image.")
    ("label-image", po::value<std::string>()->required(), "Filename of the label image.")
    ("output-image",    po::value<std::string>()->required(), "Filename of the output image.")
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
  const auto LImage(vm["label-image"].as<std::string>());
  const auto OImage(vm["output-image"].as<std::string>());

  switch (dv::ReadImageIOBase(IImage)->GetComponentType())
    {
    case itk::ImageIOBase::UCHAR:
      dv::OverlayLabels<3, unsigned char>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::CHAR:
      dv::OverlayLabels<3, char>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::USHORT:
      dv::OverlayLabels<3, unsigned short>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::SHORT:
      dv::OverlayLabels<3, short>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::UINT:
      dv::OverlayLabels<3, unsigned int>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::INT:
      dv::OverlayLabels<3, int>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::ULONG:
      dv::OverlayLabels<3, unsigned long>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    case itk::ImageIOBase::LONG:
      dv::OverlayLabels<3, long>(
        IImage,
        LImage,
        OImage
                                                 );
      break;
    // case itk::ImageIOBase::FLOAT:
    //   dv::OverlayLabels<3, float>(
    //     IImage,
    //     LImage,
    //     OImage
    //                                              );
    //   break;
    // case itk::ImageIOBase::DOUBLE:
    //   dv::OverlayLabels<3, double>(
    //     IImage,
    //     LImage,
    //     OImage
    //                                              );
    //   break;
    default:
      std::cerr << "ERROR: Unrecognized pixel type." << std::endl;
      return EXIT_FAILURE;
      break;
    }

  return EXIT_SUCCESS;

}
