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

using InfoMap = std::map<std::string, unsigned long>;

static const InfoMap the_real_aunt_sue({{"children", 3},
                                        {"cats", 7},
                                        {"samoyeds", 2},
                                        {"pomeranians", 3},
                                        {"akitas", 0},
                                        {"vizslas", 0},
                                        {"goldfish", 5},
                                        {"trees", 3},
                                        {"cars", 2},
                                        {"perfumes", 1}});

bool matches_sue(std::string const &s) {
  assert(s.substr(0, 4) == "Sue ");
  std::size_t pos = 4;
  std::size_t len = 0;
  unsigned long id = std::stoul(s.substr(4), &len);
  std::cout << "Sue " << id << ": ";
  pos += len;
  assert(s[pos] == ':');
  ++pos;
  while (pos < s.size()) {
    if (s[pos] == ' ' || s[pos] == ',') {
      ++pos;
    } else {
      auto colon = s.find(':', pos);
      assert(colon != std::string::npos);
      std::string name = s.substr(pos, colon - pos);
      pos = colon + 1;
      while (pos < s.size() && s[pos] == ' ') {
        ++pos;
      }
      auto amount = std::stoul(s.substr(pos), &len);
      pos += len;
      auto it = the_real_aunt_sue.find(name);
      std::cout << " " << name << "=" << amount;
      assert(it != the_real_aunt_sue.end());
      if (it->second != amount) {
        std::cout << " (NO!)\n";
        return false;
      }
    }
  }
  std::cout << "(YES!)\n";
  return true;
}

int main(int argc, char **argv) {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (matches_sue(line)) {
      std::cout << "MATCH: " << line << "\n";
      return 0;
    }
  }
  return 0;
}