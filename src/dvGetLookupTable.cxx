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
  lut->SetNumberOfTableValues(9);
  lut->Build();

  // Fill in a few known colors, the rest will be generated if needed
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  lut->SetTableValue(0, colors->GetColor4d("Black").GetData());
  lut->SetTableValue(1, colors->GetColor4d("Red").GetData());
  lut->SetTableValue(2, colors->GetColor4d("Orange").GetData());
  lut->SetTableValue(3, colors->GetColor4d("Yellow").GetData());
  lut->SetTableValue(4, colors->GetColor4d("Green").GetData());
  lut->SetTableValue(5, colors->GetColor4d("Blue").GetData());
  lut->SetTableValue(6, colors->GetColor4d("Purple").GetData());
  lut->SetTableValue(7, colors->GetColor4d("Cyan").GetData());
  lut->SetTableValue(8, colors->GetColor4d("Magenta").GetData());

  const double gray = 0.3;
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

  lut->SetTableRange(min, max);
  lut->Build();
 
  for(size_t i = 0; i < N; ++i)
    {
    double *rgb;
    rgb = ctf->GetColor(static_cast<double>(i)/N);
    lut->SetTableValue(i,rgb[0], rgb[1], rgb[2]);
    }

  const double gray = 0.3;
  lut->SetBelowRangeColor( gray, gray, gray, 1.0 ); // Gray
  lut->SetAboveRangeColor( gray, gray, gray, 1.0 ); // Gray
  lut->UseAboveRangeColorOn();
  lut->UseBelowRangeColorOn();

  return lut;

}

} // end namespace LUT

} // end namespace dv
