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

int main(int argc, char **argv) {
  std::multimap<std::string, std::string> replacements;
  bool collecting_replacements = true;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      collecting_replacements = false;
    } else if (collecting_replacements) {
      auto sep = line.find(" => ");
      replacements.insert({line.substr(0, sep), line.substr(sep + 4)});
    } else {
      std::set<std::string> new_molecules;
      for (unsigned pos = 0; pos < line.size(); ++pos) {
        auto [it, ite] = replacements.equal_range(line.substr(pos, 1));
        while (it != ite) {
          new_molecules.insert(line.substr(0, pos) + it->second +
                               line.substr(pos + 1));
          ++it;
        }
        if (pos < line.size() - 1) {
          auto [it, ite] = replacements.equal_range(line.substr(pos, 2));
          while (it != ite) {
            new_molecules.insert(line.substr(0, pos) + it->second +
                                 line.substr(pos + 2));
            ++it;
          }
        }
      }
      std::cout << line << "\n";
      std::cout << "Solution: " << new_molecules.size() << "\n";
    }
  }

  return 0;
}