#include <cstdlib>
#include <dvGlobFiles.h>
#include <iostream>
int
main()
{

  const auto files = dv::GlobFiles("~/Desktop/*");

  for (const auto& f : files)
    std::cout << f << std::endl;

  return EXIT_SUCCESS;
}
