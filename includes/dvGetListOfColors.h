#ifndef dv_GetListOfColors_h
#define dv_GetListOfColors_h

#include <cstdlib>
#include <array>
#include <vector>

namespace dv
{

std::vector<std::array<double, 3>>
GetListOfColors(const bool normalize = true)
{

  std::vector<std::array<double, 3>> colors;
  colors.emplace_back(std::array<double, 3>{ 0.0, 0.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 127.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 0.0, 127.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 27.0, 155.0 });
  colors.emplace_back(
    std::array<double, 3>{ 155.0, 0.0, 27.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 0.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 255.0, 127.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 0.0, 0.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 255.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 127.0, 255.0 });
  colors.emplace_back(
    std::array<double, 3>{ 127.0, 127.0, 127.0 });

  if (normalize) {
    for (auto &color : colors) {
      for (auto &channel : color) {
        channel /= 255.0;
      }
    }
  }

  return colors;
}

}

#endif
