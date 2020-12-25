#include <algorithm>
#include <dvReverseFindIf.h>
#include <iostream>
#include <numeric>
#include <vector>

int
main()
{
  std::vector<int> v(11);
  std::iota(v.begin(), v.end(), -5);

  const auto f =
    std::find_if(v.cbegin(), v.cend(), [](int i) { return i == 2; });
  std::cout << "Forward: " << *f << ' ' << std::distance(v.cbegin(), f)
            << std::endl;

  const auto r =
    dv::ReverseFindIf(v.cbegin(), v.cend(), [](int i) { return i == 2; });
  std::cout << "Reverse: " << *r << ' ' << std::distance(v.cbegin(), r)
            << std::endl;

  std::cout << (f == r) << std::endl;

  return EXIT_SUCCESS;
}
