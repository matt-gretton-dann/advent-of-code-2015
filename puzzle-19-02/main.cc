#include <cassert>
#include <cctype>
#include <climits>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>

template <typename Map>
std::size_t dedup(Map const &replacements, std::string &molecule) {
  std::size_t steps = 0;
  for (auto const &kv : replacements) {
    if (kv.first != kv.second + kv.second) {
      continue;
    }
    std::size_t pos = std::string::npos;
    do {
      pos = molecule.find(kv.first);
      if (pos != std::string::npos) {
        molecule = molecule.substr(0, pos) + kv.second +
                   molecule.substr(pos + kv.first.size());
        ++steps;
      }
    } while (pos != std::string::npos);
  }

  return steps;
}

#if 0
std::size_t
simple_find(std::unordered_map<std::string, std::string> const &replacements,
            std::string &molecule) {
  std::size_t steps = 0;
  bool changed = true;

  while (!molecule.empty() && molecule != "e" && changed) {
    changed = false;
    for (auto const &kv : replacements) {
      std::size_t pos = 0;
      while (pos != std::string::npos) {
        pos = molecule.find(kv.first, pos);
        if (pos != std::string::npos) {
          auto nm = molecule.substr(0, pos) + kv.second +
                    molecule.substr(pos + kv.first.size());
          bool good = true;
          for (auto const &kv2 : replacements) {
            auto left =
                std::max(kv2.first.size() - 1, pos) + 1 - kv2.first.size();
            if (nm.substr(left, kv2.first.size() * 2 + kv.second.size() - 2)
                    .find(kv2.first) != std::string::npos) {
              good = false;
              break;
            }
          }
          if (good) {
            ++steps;
            // std::cout << "Step: " << steps << ": " << molecule << " => " <<
            // nm
            //          << "\n";
            molecule = nm;
            changed = true;
          }
          ++pos;
        }
      }
    }
  }

  return steps;
}

std::size_t
find_molecule(std::unordered_map<std::string, std::string> const &replacements,
              std::string const &m) {
  std::string molecule = m;
  std::size_t steps = 0;
  steps += dedup(replacements, molecule);
  steps += simple_find(replacements, molecule);

  std::unordered_map<std::string, std::size_t> current_molecules = {
      {molecule, steps}};
  while (!current_molecules.empty() &&
         current_molecules.find("e") == current_molecules.end()) {
    std::unordered_map<std::string, std::size_t> new_molecules;

    std::size_t mi = 0;
    for (auto const &m : current_molecules) {
      ++mi;
      std::string molecule = m.first;
      std::size_t steps = m.second;
      ++steps;
      for (auto const &kv : replacements) {
        std::size_t pos = 0;
        while (pos != std::string::npos) {
          pos = molecule.find(kv.first, pos);
          if (pos != std::string::npos) {
            bool good = false;
            auto nm = molecule.substr(0, pos) + kv.second +
                      molecule.substr(pos + kv.first.size());
            for (auto const &kv2 : replacements) {
              auto left = std::max(kv2.first.size(), pos) - kv2.first.size();
              if (nm.substr(left, kv2.first.size() * 2 + kv.second.size())
                      .find(kv2.first) != std::string::npos) {
                good = true;
                break;
              }
            }
            if (good) {
              auto ns = steps;
              ns += dedup(replacements, nm);
              ns += simple_find(replacements, nm);
              std::cout << '\r' << steps << ": " << mi << " " << kv.first
                        << " => "
                        << kv.second; //<< " ==> "
                                      //                      << nm << "\n";
              auto [it, success] = new_molecules.insert({nm, ns});
              if (!success) {
                it->second = std::min(it->second, ns);
              }
            }
            ++pos;
          }
        }
      }
    }

    current_molecules = std::move(new_molecules);
    std::cout << "\nStep " << 1000000 << " : " << current_molecules.size()
              << " modulecules.\n"
              << std::flush;
  }
  std::cout << "\n";

  return current_molecules.find("e")->second;
}
#endif

template <typename Map>
bool is_good_replacement(Map const &replacements, std::string const &nm,
                         std::size_t pos, std::size_t replace_len) {
  for (auto const &kv : replacements) {
    auto left = std::max(kv.first.size(), pos) - kv.first.size();
    if (nm.substr(left, kv.first.size() * 2 + replace_len).find(kv.first) !=
        std::string::npos) {
      return true;
    }
  }
  return false;
}

template <typename Map>
std::size_t find_molecule(Map &replacements, std::string const &molecule) {
  std::random_device rd;
  std::mt19937 g(rd());
  while (true) {
    std::string m = molecule;
    bool changed = true;
    std::shuffle(replacements.begin(), replacements.end(), g);
    std::size_t steps = 0;
    do {
      changed = false;
      for (auto const &kv : replacements) {
        auto pos = m.find(kv.first);
        if (pos != std::string::npos) {
          m = m.substr(0, pos) + kv.second + m.substr(pos + kv.first.length());
          std::cout << "\r" << steps << "           ";
          ++steps;
          changed = true;
          if (m == "e") {
            std::cout << '\n';
            return steps;
          }
        }
      }
      std::cout << '\n';
    } while (changed);
  }
}

int main(int argc, char **argv) {
  std::vector<std::pair<std::string, std::string>> replacements;
  bool collecting_replacements = true;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      collecting_replacements = false;
    } else if (collecting_replacements) {
      auto sep = line.find(" => ");
      replacements.push_back({line.substr(sep + 4), line.substr(0, sep)});
    } else {
      std::cout << line << "\n";
      auto time = find_molecule(replacements, line);
      std::cout << "Solution: " << time << "\n";
    }
  }

  return 0;
}