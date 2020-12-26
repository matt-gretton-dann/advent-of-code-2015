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

unsigned count_combinations(Quantities::const_iterator it,
                            Quantities::const_iterator end, Quantity amount,
                            unsigned depth = 0) {
  // We have no room for this container
  std::cout << std::string(depth, ' ') << *it << ": " << amount << "\n";
  if (amount < *it) {
    return 0;
  }

  // Matched
  if (amount == *it) {
    return 1;
  }

  amount -= *it;
  auto result = 0;
  while (++it != end) {
    result += count_combinations(it, end, amount, depth + 2);
  }
  return result;
}

unsigned count_combinations(Quantities const &containers) {
  unsigned result = 0;
  for (auto it = containers.begin(); it != containers.end(); ++it) {
    result += count_combinations(it, containers.end(), total);
  }
  return result;
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

  unsigned result = count_combinations(containers);
  std::cout << "Solution: " << result << "\n";
  return 0;
}