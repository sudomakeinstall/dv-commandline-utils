#ifndef dv_QuickViewVolume_h
#define dv_QuickViewVolume_h

#include <itkQuadEdgeMesh.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <dvITKMeshToVTKPolyData.h>
#include <dvQuickViewPolyData.h>

namespace dv {

template<typename TImage>
void
QuickViewVolume(typename TImage::Pointer volume) {

  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TMesh = itk::QuadEdgeMesh< double, 3 >;
  using TCuberille = itk::CuberilleImageToMeshFilter<TImage, TMesh>;

  typename TImage::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(volume);
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<typename TImage::PixelType>(0));

  const auto cuberille = TCuberille::New();
  cuberille->SetInput( pad->GetOutput() );
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();
  cuberille->Update();

  const auto poly_data = dv::ITKMeshToVTKPolyData< TMesh >( cuberille->GetOutput() );
  dv::QuickViewPolyData( poly_data );

}

}

#endif
