#ifndef dv_ResampleFromReference_h
#define dv_ResampleFromReference_h

#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

#include <itkNearestNeighborInterpolateImageFunction.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkBSplineInterpolateImageFunction.h>

namespace dv
{

template<unsigned int Dimension, typename TPixel>
void
ResampleFromReference(
  const std::string IImage,
  const std::string RImage,
  const std::string OImage,
  const double OutsideValue,
  const bool OutputSizeExists,
  const unsigned int OutputSize,
  const bool OutputSpacingExists,
  const double OutputSpacing,
  const unsigned int &interpolator
)
{

  using TImage     = itk::Image< TPixel, Dimension >;
  using TReader    = itk::ImageFileReader< TImage >;
  using TWriter    = itk::ImageFileWriter< TImage >;
  using TTransform = itk::IdentityTransform< double, Dimension >;
  using TResample  = itk::ResampleImageFilter< TImage, TImage >;

  using NNInterpolateType = itk::NearestNeighborInterpolateImageFunction< TImage >;
  using LNInterpolateType = itk::LinearInterpolateImageFunction< TImage >;
  using BSInterpolateType = itk::BSplineInterpolateImageFunction< TImage >;


  const auto iReader = TReader::New();
  const auto rReader = TReader::New();
  iReader->SetFileName( IImage );
  rReader->SetFileName( RImage );
  rReader->Update();

  const auto resample = TResample::New();
  resample->SetInput( iReader->GetOutput() );
  resample->SetTransform( TTransform::New() );
  resample->SetDefaultPixelValue( OutsideValue );

  // Default is to use the reference image.
  const auto ReferenceSize = rReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  const auto ReferenceSpacing = rReader->GetOutput()->GetSpacing();
  const auto ReferenceDirection = rReader->GetOutput()->GetDirection();
  const auto ReferenceOrigin = rReader->GetOutput()->GetOrigin();

  if (OutputSpacingExists && OutputSizeExists)
    {
    // Set Spacing
    const double TempOutputSpacing[3] = {OutputSpacing, OutputSpacing, 1};
    resample->SetOutputSpacing( TempOutputSpacing );

    // Set Size
    const itk::Size<Dimension> TempOutputSize = {{OutputSize, OutputSize, 1}};
    resample->SetSize( TempOutputSize );

    // Set Origin
    itk::Vector<double,3> ReferenceCenter;
    ReferenceCenter[0] = ( ReferenceSize[0] / 2.0 ) * ( ReferenceSpacing[0] );
    ReferenceCenter[1] = ( ReferenceSize[1] / 2.0 ) * ( ReferenceSpacing[1] );
    ReferenceCenter[2] = 0.0;

    itk::Vector<double,3> UserCenter;
    UserCenter[0] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[1] = OutputSize / 2.0 * OutputSpacing;
    UserCenter[2] = 0.0;

    const auto CenterDiff = ReferenceCenter - UserCenter;

    const auto OriginShift = ReferenceDirection * CenterDiff;

    const auto NewOrigin = ReferenceOrigin + OriginShift;

    resample->SetOutputOrigin( NewOrigin );
    }
  else
    {
    resample->SetOutputOrigin( ReferenceOrigin );
    resample->SetOutputSpacing( ReferenceSpacing );
    resample->SetSize( ReferenceSize );
    }

  resample->SetOutputDirection( ReferenceDirection );
  switch ( interpolator )
    {
    case 0:
      {
      const auto interp = NNInterpolateType::New();
      resample->SetInterpolator( interp );
      break;
      }
    case 1:
      {
      const auto interp = LNInterpolateType::New();
      resample->SetInterpolator( interp );
      break;
      }
    default:
      {
      const auto interp = BSInterpolateType::New();
      interp->SetSplineOrder( interpolator );
      resample->SetInterpolator( interp );
      break;
      }
    }

  const auto writer = TWriter::New();
  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( OImage );
  writer->Update();

}

}

#endif
