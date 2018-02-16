#ifndef dv_ResampleFromReference_h
#define dv_ResampleFromReference_h

#include <itkImage.h>
#include <itkIdentityTransform.h>
#include <itkResampleImageFilter.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>

namespace dv
{

template<unsigned int Dimension, typename PixelType>
void
ResampleFromReference(
  const std::string iImageFileName,
  const std::string rImageFileName,
  const std::string oImageFileName,
  const double outsideValue,
  const unsigned int outputSize,
  const double outputSpacing
)
{

  using ImageType     = itk::Image< PixelType, Dimension >;
  using ReaderType    = itk::ImageFileReader< ImageType >;
  using WriterType    = itk::ImageFileWriter< ImageType >;
  using TransformType = itk::IdentityTransform< double, Dimension >;
  using ResampleType  = itk::ResampleImageFilter< ImageType, ImageType >;

  const auto iReader = ReaderType::New();
  const auto rReader = ReaderType::New();
  const auto resample = ResampleType::New();
  const auto writer = WriterType::New();

  iReader->SetFileName( iImageFileName );
  rReader->SetFileName( rImageFileName );

  resample->SetInput( iReader->GetOutput() );
  resample->SetTransform( TransformType::New() );
  resample->SetDefaultPixelValue( outsideValue );

  rReader->Update();

  // FC adding in new values
  // Get intiial values
  const auto RefDimension = rReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  auto RefDirection = rReader->GetOutput()->GetDirection();
  const auto RefSpacing = rReader->GetOutput()->GetSpacing();
  const auto RefOrigin = rReader->GetOutput()->GetOrigin();


  unsigned int Nsize1 = static_cast<double>( RefDimension[0] );
  unsigned int Nsize2 = static_cast<double>( RefDimension[1] );
  if (!(outputSize == -1))
  {
    // This means we overwrite the size v
    Nsize1 = static_cast<unsigned int>( outputSize );
    Nsize2 = static_cast<unsigned int>( outputSize );
  }

  // First load in the reference spacing
  double Nspac1 = static_cast<double>( RefSpacing[0] );
  double Nspac2 = static_cast<double>( RefSpacing[1] );
  if (!(outputSpacing == -1))
  {
    // This means we overwrite the spacing with user defined values
    Nspac1 = static_cast<double>( outputSpacing );
    Nspac2 = static_cast<double>( outputSpacing );
  }

  //std::cout << "Ref Dim:" << RefDimension << std::endl;
  //std::cout << "Ref Dir:" << RefDirection << std::endl;
  //std::cout << "Ref Spac:" << RefSpacing << std::endl;
  //std::cout << "Ref Orig:" << RefOrigin << std::endl;

  // Normalize Dir Vector (just in case)

  // Calcualte new origin so center remains the same

  itk::Vector<double,3> CenterVect;
  CenterVect[0] = ( static_cast<double> (RefDimension[0]) / 2.0 ) * static_cast<double> ( RefSpacing[0]);
  CenterVect[1] = ( static_cast<double> (RefDimension[1]) / 2.0 ) * static_cast<double> ( RefSpacing[1]);
  CenterVect[2] = ( 0.0 ) * static_cast<double> ( RefSpacing[2]);
  //std::cout << "CenterVect: [" << CenterVect[0] << " , " << CenterVect[1] << " , " <<CenterVect[2] << " ] " <<std::endl;

  itk::Vector<double,3> OutVect;
  OutVect[0] = ( static_cast<double> ( Nsize1 ) / 2.0 ) * static_cast<double> ( Nspac1 );
  OutVect[1] = ( static_cast<double> ( Nsize2 ) / 2.0 ) * static_cast<double> ( Nspac2 );
  OutVect[2] = ( 0.0 ) * static_cast<double> ( RefSpacing[2]);
  //std::cout << "OutVect: [" << OutVect[0] << " , " << OutVect[1] << " , " <<OutVect[2] << " ] " <<std::endl;

  auto CenterDiff = CenterVect - OutVect;
  //std::cout << "CenterDiff: [" << CenterDiff[0] << " , " << CenterDiff[1] << " , " <<CenterDiff[2] << " ] " <<std::endl;

  auto OriginShift = RefDirection * CenterDiff;
  //std::cout << "OriginShift: [" << OriginShift[0] << " , " << OriginShift[1] << " , " <<OriginShift[2] << " ] " <<std::endl;

  auto NewOrigin = RefOrigin + OriginShift;
  std::cout << "NewOrig:" << NewOrigin << std::endl;
  resample->SetOutputOrigin( NewOrigin );

  auto CentShift = RefDirection * CenterVect;
  auto CentPoint = RefOrigin + CentShift;

  iReader->Update();
  auto inOrigin = iReader->GetOutput()->GetOrigin();
  auto inDimension = iReader->GetOutput()->GetLargestPossibleRegion().GetSize();
  auto inSpacing = iReader->GetOutput()->GetSpacing();
  auto inDirection = iReader->GetOutput()->GetDirection();

  //std::cout << "In Dim:" << inDimension << std::endl;
  //std::cout << "In Dir:" << inDirection << std::endl;
  //std::cout << "In Spac:" << inSpacing << std::endl;
  //std::cout << "In Orig:" << inOrigin << std::endl;

  itk::Vector<double,3> EndVect;
  EndVect[0] = ( static_cast<double> (inDimension[0]) / 1.0 ) * static_cast<double> ( inSpacing[0]);
  EndVect[1] = ( static_cast<double> (inDimension[1]) / 1.0 ) * static_cast<double> ( inSpacing[1]);
  EndVect[2] = ( static_cast<double> (inDimension[2]) / 1.0 ) * static_cast<double> ( inSpacing[2]);

  auto EndShift = inDirection * EndVect;
  auto EndPoint = inOrigin + EndShift;

  //std::cout << "X ranges from: " << inOrigin[0] << " to " << EndPoint[0] << " Img Center: " << CentPoint[0] << std::endl;
  //std::cout << "Y ranges from: " << inOrigin[1] << " to " << EndPoint[1] << " Img Center: " << CentPoint[1] << std::endl;
  //std::cout << "Z ranges from: " << inOrigin[2] << " to " << EndPoint[2] << " Img Center: " << CentPoint[2] << std::endl;

  resample->SetOutputDirection( rReader->GetOutput()->GetDirection() );

  double v_OutputSpacing[3];
  v_OutputSpacing[0] = Nspac1;
  v_OutputSpacing[1] = Nspac2;
  v_OutputSpacing[2] = 1;
  resample->SetOutputSpacing( v_OutputSpacing );

  itk::Size<3> v_OutputSize = { {Nsize1, Nsize2,1} };
  resample->SetSize( v_OutputSize );

  resample->Update();

  writer->SetInput( resample->GetOutput() );
  writer->SetFileName( oImageFileName );
  writer->Update();

}

}

#endif
