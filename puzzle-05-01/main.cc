#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>

// Check if a string is nice:
// Nice strings have:
//  >=3 vowels
//  At least one double letter
//  No instances of 'ab', 'cd', 'pq', or 'xy'.
bool is_nice(std::string const &s) noexcept {
  unsigned vowel_count = 0;
  bool repeated = false;
  char last = '\0';

  std::string vowels("aeiou");
  for (auto c : s) {
    if (vowels.find(c) != std::string::npos) {
      ++vowel_count;
    }
    if (c == last) {
      repeated = true;
    }
    if ((last == 'a' && c == 'b') || (last == 'c' && c == 'd') ||
        (last == 'p' && c == 'q') || (last == 'x' && c == 'y')) {
      return false;
    }
    last = c;
  }

  return repeated && vowel_count >= 3;
}

int main(int argc, char **argv) {
  unsigned nice_strings;
  for (std::string line; std::getline(std::cin, line);) {
    nice_strings += is_nice(line);
  }
  std::cout << nice_strings << '\n';

  return 0;
}