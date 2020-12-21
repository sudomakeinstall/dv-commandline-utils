// ITK
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <itkImageFileReader.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>

// VTK
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkSmoothPolyDataFilter.h>

// Custom
#include <dvITKMeshToVTKPolyData.h>
#include <dvSegmentationView.h>
#include <dvGetLookupTable.h>

namespace dv {

SegmentationView::SegmentationView(
  const std::string FileName,
  vtkRenderer* Renderer,
  const std::vector<unsigned int> Labels,
  const std::vector<std::array<double, 3>> Colors)
  : m_FileName(FileName)
  , m_Renderer(Renderer)
  , m_Colors(Colors)
{
  this->Setup();
}

SegmentationView::SegmentationView(const SegmentationView& other) {
  this->m_FileName = other.m_FileName;
  this->m_Renderer = other.m_Renderer;
  this->m_Colors   = other.m_Colors;

  this->Setup();
  
}

void
SegmentationView::Setup()
{

  using TImage = itk::Image<short, 3>;
  const unsigned int Dimension = 3;
  using TCoordinate = float;
  using TMesh = itk::QuadEdgeMesh<TCoordinate, Dimension>;

  using TReader = itk::ImageFileReader<TImage>;
  using TPad = itk::ConstantPadImageFilter<TImage, TImage>;
  using TCuberille = itk::CuberilleImageToMeshFilter< TImage, TMesh >;

  TImage::SizeType size;
  size.Fill( 1 );

  const auto reader = TReader::New();
  reader->SetFileName(this->m_FileName);

  const auto pad = TPad::New();
  pad->SetInput( reader->GetOutput() );
  pad->SetPadLowerBound( size );
  pad->SetPadUpperBound( size );

  const auto cuberille = TCuberille::New();
  cuberille->SetInput(pad->GetOutput());
  cuberille->GenerateTriangleFacesOn();
  cuberille->RemoveProblematicPixelsOn();
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();
  try {
    cuberille->Update();
  }
  catch (itk::ExceptionObject &e) {
    std::cout << "Caught exception!" << std::endl;
    std::cerr << e << std::endl;
    return;
  }

  const auto poly_data = dv::ITKMeshToVTKPolyData< TMesh >( cuberille->GetOutput() );

  const auto smooth = vtkSmartPointer<vtkSmoothPolyDataFilter>::New();
  smooth->SetInputData(poly_data);
  smooth->SetNumberOfIterations(this->m_Iterations);
  smooth->SetRelaxationFactor(this->m_Relaxation);
  smooth->FeatureEdgeSmoothingOn();
  smooth->SetFeatureAngle(this->m_FeatureAngle);
  smooth->BoundarySmoothingOff();

  const auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
  normals->SetInputConnection(smooth->GetOutputPort());
  normals->ComputePointNormalsOn();
  normals->ComputeCellNormalsOn();
  normals->SplittingOn();
  normals->SetFeatureAngle(this->m_FeatureAngle);
  normals->Update();

  const auto lut = dv::LUT::Rainbow();
  this->m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->m_Mapper->SetInputData(normals->GetOutput());
  this->m_Mapper->SetLookupTable( lut );
  this->m_Mapper->SetScalarRange( 0, 8 );

  this->m_Actor = vtkSmartPointer<vtkActor>::New();
  this->m_Actor->SetMapper(m_Mapper);

}

void
SegmentationView::AddActor() {
  this->m_Renderer->AddActor(this->m_Actor);
}

void
SegmentationView::RemoveActor() {
  this->m_Renderer->RemoveActor(this->m_Actor);
}

}
