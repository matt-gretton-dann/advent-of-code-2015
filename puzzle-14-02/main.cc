#include <cassert>
#include <cctype>
#include <climits>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

using Distance = unsigned long;
using Time = unsigned long;
using Speed = unsigned long;

struct Reindeer;
using ReindeerSet = std::set<Reindeer>;

struct Reindeer {
  Reindeer(std::string const &s) {
    static const std::regex re(
        "(\\w+) can fly (\\d+) km/s for (\\d+) seconds?, "
        "but then must rest for (\\d+) seconds?.");
    std::smatch m;
    if (std::regex_search(s, m, re)) {
      name_ = m.str(1);
      speed_ = std::stoul(m.str(2));
      fly_time_ = std::stoul(m.str(3));
      rest_time_ = std::stoul(m.str(4));
    } else {
      assert(false);
    }
  }

  bool operator<(Reindeer const &rhs) const noexcept {
    return name_ < rhs.name_;
  }

  std::string const &name() const { return name_; }

  Distance distance(Time t) const { // Period and number of them
    Time period = fly_time_ + rest_time_;
    unsigned periods = t / period;

    // How far did we fly in the complete periods.
    Distance result = (speed_ * fly_time_ * periods);

    // And the remainder distance
    t -= periods * period;
    t = std::min(t, fly_time_);
    result += t * speed_;

    return result;
  }

  std::string name_;
  Speed speed_;
  Time fly_time_;
  Time rest_time_;
};

int main(int argc, char **argv) {
  ReindeerSet reindeer;

  for (std::string line; std::getline(std::cin, line);) {
    reindeer.insert(Reindeer(line));
  }

  std::map<std::string, unsigned> score;
  for (unsigned t = 1; t < 2504; ++t) {
    auto it =
        std::max_element(reindeer.begin(), reindeer.end(),
                         [t](Reindeer const &lhs, Reindeer const &rhs) -> bool {
                           return lhs.distance(t) < rhs.distance(t);
                         });
    auto [iit, success] = score.insert({it->name(), 1});
    if (!success) {
      iit->second++;
    }
  }

  auto it = std::max_element(score.begin(), score.end(),
                             [](auto const &lhs, auto const &rhs) -> bool {
                               return lhs.second < rhs.second;
                             });
  std::cout << it->first << " wins with a score of " << it->second << "\n";

  return 0;
}