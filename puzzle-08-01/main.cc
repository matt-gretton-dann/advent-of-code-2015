#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <variant>

enum class State { Begin, Normal, Escape, Hex1, Hex2, End };

std::string unescape(std::string const &s) {
  std::string unescaped;
  static const std::string hex = "0123456789abcdef0123456789ABCDEF";

  State state = State::Begin;
  unsigned char byte = 0;
  for (auto c : s) {
    switch (state) {
    case State::Begin:
      assert(c == '"');
      state = State::Normal;
      break;
    case State::Normal:
      if (c == '\\') {
        state = State::Escape;
      } else if (c == '"') {
        state = State::End;
      } else {
        unescaped += c;
      }
      break;
    case State::Escape:
      if (c == '\\' || c == '"') {
        state = State::Normal;
        unescaped += c;
      } else if (c == 'x') {
        byte = 0;
        state = State::Hex1;
      } else {
        assert(false);
      }
      break;
    case State::Hex1: {
      auto idx = hex.find(c);
      assert(idx != std::string::npos);
      byte = idx & 0xf;
      state = State::Hex2;
      break;
    }
    case State::Hex2: {
      auto idx = hex.find(c);
      assert(idx != std::string::npos);
      byte = (byte << 4) + (idx & 0xf);
      unescaped += (char)byte;
      state = State::Normal;
      break;
    }
    case State::End:
      assert(false);
    }
  }
  return unescaped;
}

int main(int argc, char **argv) {
  unsigned len = 0;

  // Parse the input
  for (std::string line; std::getline(std::cin, line);) {
    std::string unescaped = unescape(line);
    len += line.length() - unescaped.length();
    std::cout << line << ": " << line.length() << " written bytes, "
              << unescaped.length() << " memory bytes, difference: "
              << line.length() - unescaped.length() << "\n";
  }

  std::cout << len << "\n";

  return 0;
}