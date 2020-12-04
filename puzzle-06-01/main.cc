#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <set>
#include <string>

enum class Action { TurnOn, Toggle, TurnOff };

using Point = std::pair<unsigned, unsigned>;

/// A command
struct Command {
  Command(std::string const &s) {
    const char *re =
        "(turn on|toggle|turn off)\\s(\\d+),(\\d+)\\sthrough\\s(\\d+),(\\d+)";
    std::smatch m;
    if (!std::regex_search(s, m, std::regex(re))) {
      std::cerr << "Unable to interpret:" << s << "\n";
      assert(false);
    }
    if (m.str(1) == std::string("turn on")) {
      act_ = Action::TurnOn;
    } else if (m.str(1) == std::string("turn off")) {
      act_ = Action::TurnOff;
    } else if (m.str(1) == std::string("toggle")) {
      act_ = Action::Toggle;
    } else {
      assert(false);
    }
    bottom_left_.first = std::stoul(m.str(2), nullptr, 10);
    bottom_left_.second = std::stoul(m.str(3), nullptr, 10);
    top_right_.first = std::stoul(m.str(4), nullptr, 10);
    top_right_.second = std::stoul(m.str(5), nullptr, 10);
  }

  Action act_;
  Point bottom_left_;
  Point top_right_;
};

/// Array of lights
template <unsigned N> struct Array {
  Array() noexcept {
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        lights_[i][j] = false;
      }
    }
  }

  /// Apply a command
  void apply(Command const &command) noexcept {
    assert(command.bottom_left_.first < N);
    assert(command.bottom_left_.second < N);
    assert(command.top_right_.first < N);
    assert(command.top_right_.second < N);

    for (unsigned i = command.bottom_left_.first; i <= command.top_right_.first;
         ++i) {
      for (unsigned j = command.bottom_left_.second;
           j <= command.top_right_.second; ++j) {
        switch (command.act_) {
        case Action::TurnOn:
          lights_[i][j] = true;
          break;
        case Action::Toggle:
          lights_[i][j] = !lights_[i][j];
          break;
        case Action::TurnOff:
          lights_[i][j] = false;
          break;
        }
      }
    }
  }

  /// How many lights are on
  unsigned num_on() const noexcept {
    unsigned count = 0;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        count += lights_[i][j];
      }
    }

    return count;
  }

  /// Output a bitmap
  void bitmap() const {
    std::cout << "P1\n" << N << " " << N << "\n";
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        std::cout << (lights_[i][j] ? "1" : "0");
        if (j % 70 == 0) {
          std::cout << "\n";
        }
      }
      std::cout << "\n";
    }
  }

  bool lights_[N][N];
};

int main(int argc, char **argv) {
  Array<1000> arr;
  for (std::string line; std::getline(std::cin, line);) {
    Command cmd(line);
    arr.apply(cmd);
  }
  arr.bitmap();
  std::cout << arr.num_on() << '\n';

  return 0;
}