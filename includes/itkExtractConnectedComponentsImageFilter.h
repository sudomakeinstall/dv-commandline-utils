#ifndef itkExtractConnectedComponentsImageFilter_h
#define itkExtractConnectedComponentsImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk
{

/** \class ExtractConnectedComponentsImageFilter
 *
 * \brief Filters a image by iterating over its pixels.
 *
 * Filters a image by iterating over its pixels in a multi-threaded way
 * and {to be completed by the developer}.
 *
 * \ingroup DVUtils
 *
 */
template <typename TInputImage, typename TOutputImage = TInputImage>
class ExtractConnectedComponentsImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(ExtractConnectedComponentsImageFilter);

  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using InputPixelType = typename InputImageType::PixelType;
  using OutputPixelType = typename OutputImageType::PixelType;

  /** Standard class typedefs. */
  using Self = ExtractConnectedComponentsImageFilter<InputImageType, OutputImageType>;
  using Superclass = ImageToImageFilter<InputImageType, OutputImageType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Run-time type information. */
  itkTypeMacro(ExtractConnectedComponentsImageFilter, ImageToImageFilter);

  /** Standard New macro. */
  itkNewMacro(Self);

  /** Seg/Get N. */
  itkSetMacro(N, unsigned int);
  itkGetMacro(N, unsigned int);

  /** Seg/Get Background. */
  itkSetMacro(Background, OutputPixelType);
  itkGetMacro(Background, OutputPixelType);

protected:
  ExtractConnectedComponentsImageFilter();
  ~ExtractConnectedComponentsImageFilter() override = default;

  void PrintSelf(std::ostream & os, Indent indent) const override;

  using OutputRegionType = typename OutputImageType::RegionType;

  void GenerateData() override;

private:
#ifdef ITK_USE_CONCEPT_CHECKING
  // Add concept checking such as
  // itkConceptMacro( FloatingPointPixel, ( itk::Concept::IsFloatingPoint< typename InputImageType::PixelType > ) );
#endif

  unsigned int m_N;
  OutputPixelType m_Background;
};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkExtractConnectedComponentsImageFilter.hxx"
#endif

#endif // itkExtractConnectedComponentsImageFilter
