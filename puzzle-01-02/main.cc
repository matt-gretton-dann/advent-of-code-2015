#include <cassert>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    int floor = 0;
    std::string::size_type pos = 0;
    for (auto c : line) {
      ++pos;
      assert(c == '(' || c == ')');
      floor += (c == '(') - (c == ')');
      if (floor == -1) {
        break;
      }
    }
    std::cout << pos << "\n";
  }

  return 0;
}