#ifndef itkGenerateInitialModelImageToMeshFilter_hxx
#define itkGenerateInitialModelImageToMeshFilter_hxx

#include "itkGenerateInitialModelImageToMeshFilter.h"

// ITK
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkCuberilleImageToMeshFilter.h>
#include <itkQuadEdgeMesh.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h>
#include <itkDelaunayConformingQuadEdgeMeshFilter.h>
#include <itkLoopTriangleCellSubdivisionQuadEdgeMeshFilter.h>

// Custom
#include <itkEnforceBoundaryBetweenLabelsImageFilter.h>
#include <itkFillHolesInSegmentationImageFilter.h>
#include <itkExtractConnectedComponentsImageFilter.h>
#include <itkRefineValenceThreeVerticesQuadEdgeMeshFilter.h>

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
  using TNoise = itk::AdditiveGaussianNoiseQuadEdgeMeshFilter<OutputMeshType>;
  using TCriterion = itk::NumberOfFacesCriterion<OutputMeshType>;
  using TDecimation = itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter<OutputMeshType, OutputMeshType, TCriterion>;
  using TDelaunay = itk::DelaunayConformingQuadEdgeMeshFilter<OutputMeshType>;
  using TLoop = itk::LoopTriangleCellSubdivisionQuadEdgeMeshFilter<OutputMeshType>;
  using TRefine = itk::RefineValenceThreeVerticesQuadEdgeMeshFilter<OutputMeshType>;

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
  cuberille->ProjectVerticesToIsoSurfaceOff();
  cuberille->SavePixelAsCellDataOn();

//  const auto noise = TNoise::New();
//  noise->SetInput(cuberille->GetOutput());
//  noise->SetSigma(this->GetMeshNoiseSigma());
//  noise->SetSeed( 0 );
//
//  const auto criterion = TCriterion::New();
//
//  criterion->SetTopologicalChange(false);
//  criterion->SetNumberOfElements(this->GetNumberOfCellsInDecimatedMesh());
//
//  const auto decimate = TDecimation::New();
//  decimate->SetInput(noise->GetOutput());
//  decimate->SetCriterion(criterion);
//
//  const auto delaunay = TDelaunay::New();
//  delaunay->SetInput( decimate->GetOutput() );
//
//  const auto refine = TRefine::New();
//  refine->SetInput( decimate->GetOutput() );
//
//  const auto loop = TLoop::New();
//  loop->SetInput( refine->GetOutput() );
//  loop->Update();
//
//  mesh->Graft( loop->GetOutput() );
  cuberille->Update();
  mesh->Graft( cuberille->GetOutput() );

}

} // end namespace itk

#endif // itkGenerateInitialModelImageToMeshFilter_hxx
