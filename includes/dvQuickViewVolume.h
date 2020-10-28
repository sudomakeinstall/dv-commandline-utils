#ifndef dv_QuickViewVolume_h
#define dv_QuickViewVolume_h

#include <itkQuadEdgeMesh.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <dvITKTriangleMeshToVTKPolyData.h>
#include <dvQuickViewPolyData.h>

namespace dv {

template<typename TImage>
void
QuickViewVolume(typename TImage::Pointer volume) {

  using TMesh = itk::QuadEdgeMesh< double, 3 >;
  using TCuberille = itk::CuberilleImageToMeshFilter<TImage, TMesh>;

  const auto cuberille = TCuberille::New();
  cuberille->SetInput( volume );
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();
  cuberille->Update();

  const auto poly_data = dv::ITKTriangleMeshToVTKPolyData< TMesh >( cuberille->GetOutput() );
  dv::QuickViewPolyData( poly_data );

}

}

#endif
