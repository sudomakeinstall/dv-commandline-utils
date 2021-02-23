// VTK
#include <vtkLookupTable.h>
#include <vtkNamedColors.h>
#include <vtkColorTransferFunction.h>

// ITK
#include <dvGetLookupTable.h>

namespace dv
{

namespace LUT
{

vtkSmartPointer<vtkLookupTable>
Rainbow()
{

  // Create a lookup table to map cell data to colors
  const auto lut = vtkSmartPointer<vtkLookupTable>::New();
  lut->SetNumberOfTableValues(10);
  lut->Build();

  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  lut->SetTableValue(0, 0.0, 0.0, 0.0, 1.0);
  lut->SetTableValue(1, 1.0, 0.3, 0.3, 1.0);
  lut->SetTableValue(2, 0.3, 1.0, 0.3, 1.0);
  lut->SetTableValue(3, 0.3, 0.3, 1.0, 1.0);
  lut->SetTableValue(4, 1.0, 0.0, 1.0, 1.0);
  lut->SetTableValue(5, 1.0, 0.5, 0.0, 1.0);
  lut->SetTableValue(6, 0.0, 0.5, 1.0, 1.0);
  lut->SetTableValue(7, 1.0, 0.0, 0.5, 1.0);
  lut->SetTableValue(8, 0.0, 0.25, 0.5, 1.0);
  lut->SetTableValue(9, 0.5, 0.0, 0.25, 1.0);

  const double gray = 0.3;
  lut->SetTableRange(0, 9);
  lut->SetBelowRangeColor( gray, gray, gray, 1.0 ); // Gray
  lut->SetAboveRangeColor( gray, gray, gray, 1.0 ); // Gray
  lut->UseAboveRangeColorOn();
  lut->UseBelowRangeColorOn();

  return lut;

}

vtkSmartPointer<vtkLookupTable>
SQUEEZ(const double min, const double max, const size_t N)
{

  // Create a lookup table to map cell data to colors
  const auto lut = vtkSmartPointer<vtkLookupTable>::New();
  lut->SetNumberOfTableValues(N);
  lut->Build();

  const auto ctf = vtkSmartPointer<vtkColorTransferFunction>::New();
  ctf->SetScaleToLinear();
  ctf->AddRGBPoint(0.0, 0.0, 0.0, 1.0);
  ctf->AddRGBPoint(0.5, 1.0, 0.0, 0.0);
  ctf->AddRGBPoint(1.0, 1.0, 1.0, 0.0);

  lut->Build();
 
  for(size_t i = 0; i < N; ++i)
    {
    double *rgb;
    rgb = ctf->GetColor(static_cast<double>(i)/N);
    lut->SetTableValue(i,rgb[0], rgb[1], rgb[2]);
    }

  const double gray = 0.3;
  lut->SetTableRange(min, max);

  return lut;

}

} // end namespace LUT

} // end namespace dv
