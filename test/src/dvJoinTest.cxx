#include <cstdlib>
#include <dvJoin.h>
#include <iostream>
#include <utility>

int
main()
{

  using TInput = std::vector<std::string>;
  using TOutput = std::string;
  using TPair = std::pair<TInput, TOutput>;

  std::vector<TPair> data;
  data.emplace_back(std::make_pair(TInput{}, ""));
  data.emplace_back(std::make_pair(TInput{ "1" }, "1"));
  data.emplace_back(std::make_pair(TInput{ "1", "2" }, "1,2"));
  data.emplace_back(std::make_pair(TInput{ "1", "2", "3" }, "1,2,3"));
  data.emplace_back(std::make_pair(TInput{ "1", "2", "3", "4" }, "1,2,3,4"));

  for (const auto& d : data) {
    const auto result = dv::Join(",", d.first.cbegin(), d.first.cend());
    if (result != d.second) {
      std::cerr << "Result: " << result << std::endl;
      std::cerr << "Expected: " << d.second << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::vector<int> v({ 0, 1, 2, 3 });

  std::cout << dv::Join(",", v.cbegin(), v.cend()) << std::endl;

  return EXIT_SUCCESS;
}
