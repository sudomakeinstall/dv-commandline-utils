
#include <cstdlib>
#include <dvMath.h>

int
main(int, char**)
{
  if (!dv::Close(3.0, 3.0)) {
    return EXIT_FAILURE;
  }

  if (dv::Close(3.0, 3.1)) {
    return EXIT_FAILURE;
  }

  if (!dv::Close(dv::Clamp(3.0, 2.0, 4.0), 3.0)) {
    return EXIT_FAILURE;
  }

  if (!dv::Close(dv::Clamp(1.0, 2.0, 4.0), 2.0)) {
    return EXIT_FAILURE;
  }

  if (!dv::Close(dv::Clamp(5.0, 2.0, 4.0), 4.0)) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
