#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <dvSoftMax.h>
#include <iostream>

int
main()
{

  using T = double;
  std::vector<T> v(25);
  std::iota(v.begin(), v.end(), 200);

  const auto p = dv::SoftMax(v);

  for (const auto i : p)
    std::cout << i << ' ';
  std::cout << std::endl;

  const auto sum = std::accumulate(p.cbegin(), p.cend(), static_cast<T>(0.0));

  std::cout << sum << std::endl;

  assert(std::abs(sum - 1) < 10e-6);

  return EXIT_SUCCESS;
}
