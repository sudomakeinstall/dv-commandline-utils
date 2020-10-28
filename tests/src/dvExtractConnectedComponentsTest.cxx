#include <itkTestingMacros.h>
#include <itkExtractConnectedComponentsImageFilter.h>
#include <dvQuickViewVolume.h>

int main() {
  const unsigned int Dimension = 3;
  using TPixel = unsigned char;
  using TImage = itk::Image<TPixel, Dimension>;
  using TExtract = itk::ExtractConnectedComponentsImageFilter<TImage>;

  // 1 1 2 2    1 1 2 2
  // 1 2 1 2 => 1 0 0 2
  // 2 1 0 2    0 0 0 2
  const auto image = TImage::New();
  TImage::IndexType index{0, 0, 0};
  TImage::SizeType size{3, 4, 1};
  TImage::RegionType region{index, size};

  image->SetRegions( region );
  image->Allocate();
  image->FillBuffer(0);

  image->SetPixel({0, 0, 0}, 1);
  image->SetPixel({1, 0, 0}, 1);
  image->SetPixel({2, 0, 0}, 2);

  image->SetPixel({0, 1, 0}, 1);
  image->SetPixel({1, 1, 0}, 2);
  image->SetPixel({2, 1, 0}, 1);

  image->SetPixel({0, 2, 0}, 2);
  image->SetPixel({1, 2, 0}, 1);
  image->SetPixel({2, 2, 0}, 0);

  image->SetPixel({0, 3, 0}, 2);
  image->SetPixel({1, 3, 0}, 2);
  image->SetPixel({2, 3, 0}, 2);

  dv::QuickViewVolume<TImage>( image );

  const auto extract = TExtract::New();
  extract->SetInput( image );
  extract->SetN(1);

  ITK_TRY_EXPECT_NO_EXCEPTION(extract->Update());

//  ITK_TEST_EXPECT_EQUAL(extract->GetOutput()->GetPixel({0, 0, 0}), 0);
//  ITK_TEST_EXPECT_EQUAL(extract->GetOutput()->GetPixel({1, 0, 0}), 0);

  dv::QuickViewVolume<TImage>( extract->GetOutput() );

  return EXIT_SUCCESS;
}
