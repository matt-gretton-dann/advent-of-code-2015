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

struct Graph {
  Graph() { nodes_.insert("self"); }
  using Node = std::string;
  using Weight = int;
  using Nodes = std::set<Node>;
  using NodeList = std::vector<Node>;
  using Edge = std::pair<Node, Node>;
  using EdgeWeights = std::map<Edge, Weight>;

  void add_edge(std::string const &s) {
    static const std::regex re(
        "(\\w+) would (gain|lose) (\\d+) happiness units? "
        "by sitting next to (\\w+).");
    std::smatch m;
    if (std::regex_search(s, m, re)) {
      nodes_.insert(m.str(1));
      weights_.insert({{m.str(1), "self"}, 0});
      weights_.insert({{"self", m.str(1)}, 0});
      nodes_.insert(m.str(4));
      int delta = std::stoi(m.str(3));
      if (m.str(2) == "lose") {
        delta = -delta;
      }
      weights_.insert({{m.str(1), m.str(4)}, delta});
    } else {
      assert(false);
    }
  }

  Weight max_happiness() const {
    int max_happiness = INT_MIN;
    NodeList nl(nodes_.begin(), nodes_.end());
    std::sort(nl.begin(), nl.end());
    do {
      std::cout << "\r";
      for (auto const &s : nl) {
        std::cout << s << " ";
      }
      int h = happiness(nl);
      std::cout << "Happiness = " << h << "       ";
      if (h > max_happiness) {
        std::cout << "\n";
        max_happiness = h;
      }
    } while (std::next_permutation(nl.begin(), nl.end()));

    std::cout << "\n";
    return max_happiness;
  }

  int happiness(NodeList const &nl) const {
    int h = 0;
    h += weights_.find(std::make_pair(nl[nl.size() - 1], nl[0]))->second;
    h += weights_.find(std::make_pair(nl[0], nl[nl.size() - 1]))->second;
    for (std::size_t off = 1; off < nl.size(); ++off) {
      h += weights_.find(std::make_pair(nl[off - 1], nl[off]))->second;
      h += weights_.find(std::make_pair(nl[off], nl[off - 1]))->second;
    }
    return h;
  }

  Nodes nodes_;
  EdgeWeights weights_;
};

int main(int argc, char **argv) {
  Graph g;
  for (std::string line; std::getline(std::cin, line);) {
    g.add_edge(line);
  }

  auto h = g.max_happiness();
  std::cout << "Max happiness: " << h << "\n";
  return 0;
}