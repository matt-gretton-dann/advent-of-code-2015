#include <cassert>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    int floor = 0;
    for (auto c : line) {
      assert(c == '(' || c == ')');
      floor += (c == '(') - (c == ')');
    }
    std::cout << floor << "\n";
  }

  return 0;
}