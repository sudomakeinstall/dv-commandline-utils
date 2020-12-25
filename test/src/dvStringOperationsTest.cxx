#include <cstdlib>
#include <dvStringOperations.h>

int
main(int, char**)
{
  if ("c" != dv::TrimCharactersFromEnd("c////", '/')) {
    return EXIT_FAILURE;
  }

  if ("c/" != dv::AppendCharacterIfAbsent("c", '/')) {
    return EXIT_FAILURE;
  }

  if ("c/" != dv::AppendCharacterIfAbsent("c/", '/')) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
