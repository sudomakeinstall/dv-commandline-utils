
#include <dvProgress.h>

int
main(int, char**)
{

  size_t N = 10000;
  auto pgrs = dv::Progress(N);
  for (size_t i = 0; i < N*2; ++i) {
    pgrs.UnitCompleted();
  }

  return EXIT_SUCCESS;
}
