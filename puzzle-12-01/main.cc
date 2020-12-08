#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

int parse_numbers(std::string const &s) {
  static const std::regex re("-?\\d+");
  std::string left = s;
  std::smatch m;
  int acc = 0;
  while (std::regex_search(left, m, re)) {
    acc += std::stoi(m.str(0));
    left = m.suffix();
  }

  return acc;
}

int main(int argc, char **argv) {
  int acc = 0;
  for (std::string line; std::getline(std::cin, line);) {
    acc += parse_numbers(line);
  }

  std::cout << "Accumulated value: " << acc << "\n";
  return 0;
}