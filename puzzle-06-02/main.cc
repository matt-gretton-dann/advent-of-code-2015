#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <set>
#include <string>

enum class Action { TurnOn, Toggle, TurnOff };
using Point = std::pair<unsigned, unsigned>;
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

template <unsigned N> struct Array {
  Array() noexcept {
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        lights_[i][j] = 0;
      }
    }
  }

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
          ++lights_[i][j];
          break;
        case Action::Toggle:
          lights_[i][j] += 2;
          break;
        case Action::TurnOff:
          if (lights_[i][j] > 0) {
            --lights_[i][j];
          }
          break;
        }
      }
    }
  }

  unsigned brightness() const noexcept {
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
    unsigned max = 0;
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        if (lights_[i][j] > max) {
          max = lights_[i][j];
        }
      }
    }
    std::cout << "P2\n" << N << " " << N << "\n" << max << "\n";
    for (unsigned i = 0; i < N; ++i) {
      for (unsigned j = 0; j < N; ++j) {
        std::cout << lights_[i][j] << "\n";
      }
    }
  }

  unsigned lights_[N][N];
};

int main(int argc, char **argv) {
  Array<1000> arr;
  for (std::string line; std::getline(std::cin, line);) {
    Command cmd(line);
    arr.apply(cmd);
  }
  arr.bitmap();
  std::cout << arr.brightness() << '\n';

  return 0;
}