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

using Comparison = std::function<bool(unsigned long, unsigned long)>;
using Info = std::pair<unsigned long, Comparison>;
using InfoMap = std::map<std::string, Info>;

bool Equality(unsigned long a, unsigned long b) { return a == b; }
bool GreaterThan(unsigned long a, unsigned long b) { return a > b; }
bool LessThan(unsigned long a, unsigned long b) { return a < b; }

static const InfoMap the_real_aunt_sue({{"children", {3, Equality}},
                                        {"cats", {7, GreaterThan}},
                                        {"samoyeds", {2, Equality}},
                                        {"pomeranians", {3, LessThan}},
                                        {"akitas", {0, Equality}},
                                        {"vizslas", {0, Equality}},
                                        {"goldfish", {5, LessThan}},
                                        {"trees", {3, GreaterThan}},
                                        {"cars", {2, Equality}},
                                        {"perfumes", {1, Equality}}});

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
      if (!it->second.second(amount, it->second.first)) {
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