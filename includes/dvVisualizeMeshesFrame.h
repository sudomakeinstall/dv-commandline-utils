#ifndef dv_VisualizeMeshesFrame_h
#define dv_VisualizeMeshesFrame_h

// std
#include <vector>
#include <string>

// Custom
#include <dvMeshView.h>
#include <dvSegmentationView.h>
#include <dvTextView.h>

namespace dv {

class VisualizeMeshesFrame {

public:

vtkRenderer* m_Renderer;
std::vector<MeshView> m_MeshViews;
std::vector<SegmentationView> m_SegmentationViews;
std::vector<TextView> m_TextViews;

VisualizeMeshesFrame(vtkRenderer* Renderer) {
  this->m_Renderer = Renderer;
}

void AddTextView(const std::string& text) {
  auto tv = dv::TextView(text, this->m_Renderer);
  this->m_TextViews.push_back(tv);
}

void AddMeshView(const std::string& FileName) {
  auto mv = dv::MeshView(FileName, this->m_Renderer);
  this->m_MeshViews.push_back(mv);
}

void AddSegmentationView(
    const std::string& FileName
  , const std::vector<unsigned int> Labels
  , const std::vector<std::array<double, 3>> Colors) {
  auto sv = dv::SegmentationView(FileName, this->m_Renderer, Labels, Colors);
  this->m_SegmentationViews.push_back(sv);
}

void TurnAllActorsOn() {
  for (auto &sv : this->m_SegmentationViews) {
    sv.AddActor();
  }

  for (auto &mv : this->m_MeshViews) {
    mv.AddActor();
  }

  for (auto &tv : this->m_TextViews) {
    tv.AddActor();
  }
}

void SetHue(const double hue) {

  for (auto &mv : this->m_MeshViews) {
    mv.SetHue(hue);
  }

}

void SetTextAnnotationsVisible(const bool &Visible) {

  if (Visible) {
    for (auto &tv : this->m_TextViews) {
      tv.AddActor();
    }
  } else {
    for (auto &tv : this->m_TextViews) {
      tv.RemoveActor();
    }
  }
}

void TurnAllActorsOff() {
  for (auto &sv : this->m_SegmentationViews) {
    sv.RemoveActor();
  }

  for (auto &mv : this->m_MeshViews) {
    mv.RemoveActor();
  }

  for (auto &tv : this->m_TextViews) {
    tv.RemoveActor();
  }
}

}; // end class VisualizeMeshesFrame

} // end namespace dv

#endif
