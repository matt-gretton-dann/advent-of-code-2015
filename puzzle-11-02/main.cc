#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

bool illegal_char(char c) { return c == 'i' || c == 'l' || c == 'o'; }

void pre_advance_password(std::string &s) {
  std::string::size_type pos = 0;
  while (pos < s.length() && !illegal_char(s[pos])) {
    ++pos;
  }
  if (pos == s.length()) {
    return;
  }
  ++s[pos++];
  while (pos < s.length()) {
    s[pos++] = 'a';
  }
}

void advance_password(std::string &s) {
  auto pos = s.length() - 1;
  while (true) {
    if (s[pos] == 'z') {
      assert(pos != 0);
      s[pos] = 'a';
      pos -= 1;
    } else {
      ++s[pos];
      if (illegal_char(s[pos])) {
        ++s[pos];
      }
      return;
    }
  }
}

bool valid_password(std::string const &s) {
  unsigned double_count = 0;
  bool run = false;
  char last2 = '\0';
  char last = '\0';
  for (auto c : s) {
    if (c == last && last2 != c) {
      ++double_count;
    } else if (c == last + 1 && c == last2 + 2) {
      run = true;
    }
    last2 = last;
    last = c;
  }

  return double_count >= 2 && run;
}

std::string next_password(std::string const &s) {
  std::string result = s;
  pre_advance_password(result);
  do {
    advance_password(result);
  } while (!valid_password(result));
  return result;
}

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    std::string next = next_password(line);
    std::string next2 = next_password(next);
    std::cout << "Current password: " << line << "; Next password: " << next
              << "; Subsequent password: " << next2 << "\n";
  }
  return 0;
}