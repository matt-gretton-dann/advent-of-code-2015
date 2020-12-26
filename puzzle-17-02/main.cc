#include <cassert>
#include <cctype>
#include <climits>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

using Quantity = unsigned long;
constexpr Quantity total = 150;

using Quantities = std::vector<Quantity>;

template <typename Fn1, typename Fn2>
unsigned count_combinations(Quantities::const_iterator it,
                            Quantities::const_iterator end, Quantity amount,
                            unsigned depth, Fn1 base_result,
                            unsigned init_addend, Fn2 adder) {
  // We have no room for this container
  std::cout << std::string(depth, ' ') << *it << ": " << amount;
  if (amount < *it) {
    std::cout << " - Not big enough.\n";
    return init_addend;
  }

  // Matched
  if (amount == *it) {
    std::cout << " - Filled - result = " << base_result(depth) << "\n";
    return base_result(depth);
  }

  std::cout << " - Recursing: initial result = " << init_addend << "\n";
  amount -= *it;
  auto result = init_addend;
  while (++it != end) {
    auto child_score = count_combinations(it, end, amount, depth + 1,
                                          base_result, init_addend, adder);
    result = adder(result, child_score);
  }
  std::cout << std::string(depth, ' ') << "Recursion result: " << result
            << "\n";
  return result;
}

template <typename Fn1, typename Fn2>
unsigned count_combinations(Quantities const &containers, Quantity amount,
                            Fn1 base_result, unsigned init_addend, Fn2 adder) {
  unsigned result = init_addend;
  for (auto it = containers.begin(); it != containers.end(); ++it) {
    result = adder(result, count_combinations(it, containers.end(), total, 0,
                                              base_result, init_addend, adder));
  }
  return result;
}

unsigned find_shortest_combination(Quantities const &containers) {
  return count_combinations(
      containers, total, [](unsigned depth) { return depth; }, UINT_MAX,
      [](unsigned current, unsigned child_score) {
        return std::min(current, child_score);
      });
}

unsigned count_min_length_combinations(Quantities const &containers,
                                       unsigned expected_depth) {
  return count_combinations(
      containers, total,
      [expected_depth](unsigned depth) { return depth == expected_depth; }, 0,
      [](unsigned current, unsigned child_score) {
        return current + child_score;
      });
}

int main(int argc, char **argv) {
  std::string line;
  Quantities containers;
  while (std::getline(std::cin, line)) {
    containers.push_back(std::stoul(line));
  }

  // Sort containers into reverse order
  std::sort(containers.begin(), containers.end());
  std::reverse(containers.begin(), containers.end());
  unsigned short_depth = find_shortest_combination(containers);
  std::cout << "Shortest length result: " << short_depth << "\n";
  unsigned result = count_min_length_combinations(containers, short_depth);
  std::cout << "Solution: " << result << "\n";
  return 0;
}