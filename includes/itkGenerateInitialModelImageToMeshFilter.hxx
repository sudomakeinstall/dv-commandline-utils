#ifndef itkGenerateInitialModelImageToMeshFilter_hxx
#define itkGenerateInitialModelImageToMeshFilter_hxx

#include "itkGenerateInitialModelImageToMeshFilter.h"

// ITK
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkQuadEdgeMesh.h>
//#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>

// CGAL
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>

// Custom
#include <itkEnforceBoundaryBetweenLabelsImageFilter.h>
#include <itkFillHolesInSegmentationImageFilter.h>
#include <itkExtractConnectedComponentsImageFilter.h>
//#include <itkRefineValenceThreeVerticesQuadEdgeMeshFilter.h>
#include <dvITKMeshToCGALSurfaceMesh.h>
#include <dvCGALSurfaceMeshToITKMesh.h>
#include <dvEdge_preserving_midpoint_placement.h>

namespace itk
{

template <typename TInputImage, typename TOutputMesh>
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::GenerateInitialModelImageToMeshFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->m_LVClosingRadius = 3;
  this->m_GeneralClosingRadius = 3;
}

template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::SetInput(const InputImageType * image)
{
  this->ProcessObject::SetNthInput(0, const_cast<InputImageType *>(image));
}

template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <typename TInputImage, typename TOutputMesh>
void
GenerateInitialModelImageToMeshFilter<TInputImage, TOutputMesh>
::GenerateData()
{

  const auto image = Superclass::GetInput(0);
  typename OutputMeshType::Pointer mesh = Superclass::GetOutput();

  using TKernel = itk::BinaryBallStructuringElement<InputPixelType, InputImageDimension>;
  using TClose = itk::BinaryMorphologicalClosingImageFilter<InputImageType, InputImageType, TKernel>;
  using TFill = itk::FillHolesInSegmentationImageFilter<InputImageType>;
  using TEnforce = itk::EnforceBoundaryBetweenLabelsImageFilter<InputImageType>;
  using TConnected = itk::ExtractConnectedComponentsImageFilter<InputImageType>;
  using TPad = itk::ConstantPadImageFilter<InputImageType, InputImageType>;
  using TCuberille = itk::CuberilleImageToMeshFilter< InputImageType, OutputMeshType >;
  using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<OutputMeshType>;
//  using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<OutputMeshType>;
//  using TRefine = itk::RefineValenceThreeVerticesQuadEdgeMeshFilter<OutputMeshType>;

  using TCGALKernel = CGAL::Simple_cartesian<double>;
  using TCGALPoint = TCGALKernel::Point_3;
  using TCGALMesh = CGAL::Surface_mesh<TCGALPoint>;
  namespace SMS = CGAL::Surface_mesh_simplification;
  using TCGALPlacement = SMS::Bounded_normal_change_placement<SMS::Edge_preserving_midpoint_placement<TCGALMesh>>;

  TKernel closeKernel;
  closeKernel.SetRadius(this->GetGeneralClosingRadius());
  closeKernel.CreateStructuringElement();

  std::array<typename TClose::Pointer, 8> closing;
  for (size_t i = 0; i < 8; ++i) {
    closing[i] = TClose::New();
    closing[i]->SetKernel(closeKernel);
    closing[i]->SetForegroundValue( i + 2 );
    if (i > 0) {
      closing[i]->SetInput( closing[i - 1]->GetOutput() );
    } else {
      closing[i]->SetInput( image );
    }
  }

  TKernel lvCloseKernel;
  lvCloseKernel.SetRadius(this->GetLVClosingRadius());
  lvCloseKernel.CreateStructuringElement();

  const auto closing_lv = TClose::New();
  closing_lv->SetInput( closing.back()->GetOutput() );
  closing_lv->SetKernel( lvCloseKernel );
  closing_lv->SetForegroundValue( 1 );

  const auto enforce0 = TEnforce::New();
  enforce0->SetInput( closing_lv->GetOutput() );
  enforce0->SetLabels1({4, 5});
  enforce0->SetLabels2({2, 3, 6, 7, 8, 9});

  const auto enforce1 = TEnforce::New();
  enforce1->SetInput( enforce0->GetOutput() );
  enforce1->SetLabels1({3});
  enforce1->SetLabels2({1, 6, 7, 8, 9});

  const auto connected = TConnected::New();
  connected->SetInput( enforce1->GetOutput() );

  typename InputImageType::SizeType padding;
  padding.Fill(1);

  const auto pad = TPad::New();
  pad->SetInput(connected->GetOutput());
  pad->SetPadUpperBound(padding);
  pad->SetPadLowerBound(padding);
  pad->SetConstant(static_cast<InputPixelType>(0));

  const auto cuberille = TCuberille::New();
  cuberille->SetInput(pad->GetOutput());
  cuberille->GenerateTriangleFacesOn();
  cuberille->RemoveProblematicPixelsOn();
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();
  cuberille->Update();

  //
  // CONVERT ITK TO CGAL
  //

  auto surface_mesh = dv::ITKMeshToCGALSurfaceMesh<TOutputMesh, TCGALMesh>( cuberille->GetOutput() );

  //
  // VERIFY AND DECIMATE
  //

  itkAssertOrThrowMacro(CGAL::is_triangle_mesh(surface_mesh), "Input geometry is not triangulated.")

  SMS::Count_stop_predicate<TCGALMesh> stop(this->GetNumberOfCellsInDecimatedMesh());
  SMS::edge_collapse(
    surface_mesh
    , stop
    , CGAL::parameters::get_placement(TCGALPlacement())
  );

  surface_mesh.collect_garbage();

  //
  // CONVERT CGAL TO ITK
  //

  const auto o_mesh = dv::CGALSurfaceMeshToITKMesh<TCGALMesh, TOutputMesh>(surface_mesh);

//  const auto refine = TRefine::New();
//  refine->SetInput( decimate->GetOutput() );

//  using TEdge = typename TOutputMesh::EdgeCellType;
//  std::list<TEdge*> edges;
//  for (auto it = o_mesh->GetEdgeCells()->Begin(); it != o_mesh->GetEdgeCells()->End(); ++it) {
//    const auto edge = static_cast<TEdge*>(it.Value());
//    if (edge->GetQEGeom()->GetLeft() != edge->GetQEGeom()->GetRight()) {
//      edges.push_back(edge);
//    }
//  }
//
//  const auto delaunay = TDelaunay::New();
//  delaunay->SetInput( o_mesh );
//  delaunay->SetListOfConstrainedEdges( edges );

  const auto loop = TLoop::New();
//  loop->SetInput( delaunay->GetOutput() );
  loop->SetInput( o_mesh );
  loop->Update();

  mesh->Graft( loop->GetOutput() );

}

} // end namespace itk

#endif // itkGenerateInitialModelImageToMeshFilter_hxx
