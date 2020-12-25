
#include <algorithm>
#include <cstdlib>
#include <dvVectorOperations.h>
#include <vector>

int
main(int, char**)
{
  const std::vector<short> inp = { 1, 3, 3, 7 };
  const short val = 3;
  const auto out = dv::ElementwiseEqual(inp, val);
  const std::vector<bool> exp = { false, true, true, false };

  if (!std::equal(out.begin(), out.end(), exp.begin())) {
    return EXIT_FAILURE;
  }

  const auto slice = dv::BooleanIndexing(inp, out);
  const std::vector<short> exp2 = { 3, 3 };
  if (!std::equal(slice.begin(), slice.end(), exp2.begin())) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
