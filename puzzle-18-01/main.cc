#include <cassert>
#include <cctype>
#include <climits>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <variant>

struct ConwayState {
  ConwayState(std::size_t width, std::string const &s)
      : width_(width), state_(width_ * width_, 0) {
    assert(s.size() == width_ * width_);

    for (std::size_t i = 0; i < state_.size(); ++i) {
      if (s[i] == '#') {
        flip(i);
      }
    }
  }

  ConwayState next_state() const {
    ConwayState next(*this);
    for (std::size_t i = 0; i < state_.size(); ++i) {
      if (state_[i] == (2 | active_) || state_[i] == (3 | active_)) {
        continue;
      }
      if (state_[i] == 3 || (state_[i] & active_) != 0) {
        next.flip(i);
      }
    }

    return next;
  }

  std::size_t num_active() const {
    return std::accumulate(state_.begin(), state_.end(), std::size_t(0),
                           [](std::size_t current, unsigned char info) {
                             return current + ((info & active_) != 0);
                           });
  }

private:
  void flip(std::size_t idx) {
    state_[idx] = state_[idx] ^ active_;
    int delta = ((state_[idx] & active_) == active_) ? 1 : -1;
    std::size_t row = idx / width_;
    std::size_t col = idx % width_;
    for (std::size_t r = std::max(std::size_t(1), row) - 1;
         r < std::min(width_, row + 2); ++r) {
      for (std::size_t c = std::max(std::size_t(1), col) - 1;
           c < std::min(width_, col + 2); ++c) {
        if (r == row && c == col) {
          continue;
        }
        state_[r * width_ + c] = state_[r * width_ + c] + delta;
      }
    }
  }

  std::size_t width_;
  std::vector<unsigned char> state_;

  static constexpr unsigned char active_ = 0x80;
  friend std::ostream &operator<<(std::ostream &os, ConwayState const &state);
};

std::ostream &operator<<(std::ostream &os, ConwayState const &state) {
  std::size_t c = 0;
  for (auto s : state.state_) {
    os << (s & ConwayState::active_ ? '#' : '.');
    os << (s & ~ConwayState::active_);
    ++c;
    if (c == state.width_) {
      os << '\n';
      c = 0;
    }
  }
  return os;
}

int main(int argc, char **argv) {
  std::string line;
  std::size_t width = 0;
  std::string init;
  while (std::getline(std::cin, line)) {
    if (width == 0) {
      width = line.size();
    }
    assert(width == line.size());
    init += line;
  }
  ConwayState conway(width, init);
  for (unsigned i = 0; i < 100; ++i) {
    conway = conway.next_state();
  }
  std::cout << "Solution: " << conway.num_active() << "\n";
  return 0;
}