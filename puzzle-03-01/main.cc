#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <string>

struct Pos {
  Pos(int x, int y) : x_(x), y_(y) {}

  bool operator<(Pos const &rhs) const noexcept {
    return x_ < rhs.x_ || (x_ == rhs.x_ && y_ < rhs.y_);
  }

  bool operator==(Pos const &rhs) const noexcept {
    return x_ == rhs.x_ && y_ == rhs.y_;
  }

  void move(char c) {
    switch (c) {
    case '>':
      ++x_;
      break;
    case '<':
      --x_;
      break;
    case '^':
      ++y_;
      break;
    case 'v':
      --y_;
      break;
    default:
      std::cout << "Unrecognised character: " << c << "\n";
      break;
    }
  }

  int x_;
  int y_;
};

int main(int argc, char **argv) {
  for (std::string line; std::getline(std::cin, line);) {
    std::set<Pos> visited;
    Pos santa(0, 0);
    visited.insert(santa);
    for (auto c : line) {
      santa.move(c);
      visited.insert(santa);
    }
    std::cout << "Houses visited = " << visited.size() << "\n";
  }

  return 0;
}