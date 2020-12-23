// ITK
#include <itkTestingMacros.h>
#include <itkImage.h>
#include <itkConstantPadImageFilter.h>
#include <itkQuadEdgeMesh.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>
#include <itkConnectedComponentImageFilter.h>

// Custom
#include <dvITKMeshToVTKPolyData.h>
#include <dvQuickViewMultiplePolyData.h>

int main() {

  using TPixel = unsigned char;
  using TCoordinate = float;
  const unsigned int Dimension = 3;
  using TImage = itk::Image<TPixel, Dimension>;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TConnected = itk::ConnectedComponentImageFilter<TImage, TImage>;
  using TExtract = itk::CuberilleImageToMeshFilter< TImage, TMesh >;
  using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter< TMesh >;

  for (size_t mask = 255; mask < std::pow(2, 8); ++mask) {

    std::bitset<8> bitmask(mask);

    std::cout << mask << ' ' << bitmask << std::endl;

    const auto image = TImage::New();
    TImage::SizeType size;
    size.Fill( 2 );

    TImage::IndexType origin;
    origin.Fill( 0 );

    TImage::RegionType region(origin, size);

    image->SetRegions( region );
    image->Allocate();
    image->FillBuffer( 0 );

    for (size_t index = 0; index < std::pow(2, 3); ++index) {
      std::bitset<3> bitindex(index);
      image->SetPixel( {{bitindex[0], bitindex[1], bitindex[2]}}, bitmask[index] );
    }

    typename TImage::SizeType padding;
    padding.Fill(1);

    const auto pad = TPad::New();
    pad->SetInput(image);
    pad->SetPadUpperBound(padding);
    pad->SetPadLowerBound(padding);
    pad->SetConstant(static_cast<TPixel>(0));

    const auto connected = TConnected::New();
    connected->SetInput( pad->GetOutput() );

    const auto extract = TExtract::New();
    extract->SetInput( connected->GetOutput() );
    extract->GenerateTriangleFacesOn();
    extract->ProjectVerticesToIsoSurfaceOff();
    extract->SavePixelAsCellDataOn();
    ITK_TRY_EXPECT_NO_EXCEPTION(extract->Update());

    const auto loop0 = TLoop::New();
    loop0->SetInput( extract->GetOutput() );
    const auto loop1 = TLoop::New();
    loop1->SetInput( loop0->GetOutput() );
    const auto loop2 = TLoop::New();
    loop2->SetInput( loop1->GetOutput() );
    const auto loop3 = TLoop::New();
    loop3->SetInput( loop2->GetOutput() );
    ITK_TRY_EXPECT_NO_EXCEPTION(loop3->Update());

    {
    const auto poly_data_i = dv::ITKMeshToVTKPolyData< TMesh >( extract->GetOutput() );
    const auto poly_data_o = dv::ITKMeshToVTKPolyData< TMesh >( loop3->GetOutput() );
    std::vector<vtkPolyData*> poly_data_vector;
    poly_data_vector.emplace_back(poly_data_i);
    poly_data_vector.emplace_back(poly_data_o);
    dv::QuickViewMultiplePolyData( poly_data_vector );
    }

  }

  return EXIT_SUCCESS;

}
