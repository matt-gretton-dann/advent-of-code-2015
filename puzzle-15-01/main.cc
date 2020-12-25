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

using Score = long;
using PropertyMap = std::map<std::string, Score>;

struct Ingredient {
  explicit Ingredient(std::string const &s) {
    auto colon = s.find(':');
    name_ = s.substr(0, colon);
    auto suffix = s.substr(colon + 1);
    std::cout << name_ << "\n";
    while (!suffix.empty()) {
      static const std::regex re(",? (\\w+) (-?\\d+)");
      std::smatch m;
      if (std::regex_search(suffix, m, re)) {
        auto [it, success] =
            properties_.insert({m.str(1), std::stol(m.str(2))});
        assert(success);
        std::cout << "  " << it->first << ": " << it->second << "\n";
        suffix = m.suffix();
      } else {
        assert(false);
      }
    }
  }

  bool operator<(Ingredient const &rhs) const noexcept {
    return name_ < rhs.name_;
  }

  std::string const &name() const noexcept { return name_; }

  void update_score(Score amount, PropertyMap &totals) const {
    for (auto const &kv : properties_) {
      if (kv.first == "calories") {
        continue;
      }
      auto [it, success] = totals.insert({kv.first, amount * kv.second});
      if (!success) {
        it->second += amount * kv.second;
      }
    }
  }

private:
  std::string name_;
  PropertyMap properties_;
};

struct Ingredients {
  void add_ingredient(std::string const &s) {
    ingredients_.push_back(Ingredient(s));
  }

  Score best_combination(Score amount) const {
    PropertyMap totals;
    return best_combination(amount, ingredients_.begin(), 0UL, totals);
  }

private:
  Score best_combination(Score amount,
                         std::vector<Ingredient>::const_iterator it,
                         Score best_score, PropertyMap &totals) const {
    it->update_score(amount, totals);
    auto it2 = it;
    ++it2;
    if (it2 == ingredients_.end()) {
      auto score = calculate_score(totals);
      best_score = std::max(best_score, score);
      it->update_score(-amount, totals);
      return best_score;
    }

    for (auto allocation = amount - 1; allocation > 0; --allocation) {
      it->update_score(-1, totals);
      best_score =
          std::max(best_score, best_combination(amount - allocation, it2,
                                                best_score, totals));
    }
    it->update_score(-1, totals);

    return best_score;
  }

  Score calculate_score(PropertyMap const &totals) const {
    Score r = 1;
    for (auto const &kv : totals) {
      if (kv.first == "calories") {
        continue;
      }
      if (kv.second < 0) {
        return 0;
      }
      r *= kv.second;
    }

    return r;
  }

  std::vector<Ingredient> ingredients_;
};

int main(int argc, char **argv) {
  Ingredients ingredients;

  std::string line;
  while (std::getline(std::cin, line)) {
    ingredients.add_ingredient(line);
  }

  auto r = ingredients.best_combination(100);
  std::cout << "Solution: " << r << "\n";
  return 0;
}