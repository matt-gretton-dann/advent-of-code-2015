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

Distance distance(std::string const &s, unsigned t) {
  static const std::regex re("(\\w+) can fly (\\d+) km/s for (\\d+) seconds?, "
                             "but then must rest for (\\d+) seconds?.");
  std::smatch m;
  if (std::regex_search(s, m, re)) {
    unsigned fly_speed = std::stoul(m.str(2));
    unsigned fly_time = std::stoul(m.str(3));
    unsigned rest_time = std::stoul(m.str(4));

    // Period and number of them
    unsigned period = fly_time + rest_time;
    unsigned periods = t / period;

    // How far did we fly in the complete periods.
    Distance result = (fly_speed * fly_time * periods);

    // And the remainder distance
    t -= periods * period;
    t = std::min(t, fly_time);
    result += t * fly_speed;

    std::cout << m.str(1) << "(" << fly_speed << ", " << fly_time << ", "
              << rest_time << ") = " << result << "\n";
    return result;
  } else {
    assert(false);
  }
}

int main(int argc, char **argv) {
  Distance max_d = 0;
  for (std::string line; std::getline(std::cin, line);) {
    max_d = std::max(max_d, distance(line, 2503));
  }

  std::cout << "Max distance: " << max_d << "\n";
  return 0;
}