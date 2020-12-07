#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

using Weight = unsigned;
using Node = std::string;
using Edge = std::pair<Node, Node>;
using Nodes = std::set<Node>;
using Edges = std::map<Edge, Weight>;

struct Graph {
  void add_edge(std::string const &s) {
    static const std::regex re("(.+) to (.+) = (\\d+)");
    std::smatch m;
    if (!std::regex_search(s, m, re)) {
      std::cout << "Failed to match: " << s << "\n";
      assert(false);
      return;
    }
    auto n1 = m.str(1);
    auto n2 = m.str(2);
    auto w = std::stoul(m.str(3));
    auto edge = std::make_pair(n1, n2);
    auto back_edge = std::make_pair(n2, n1);

    nodes_.insert(n1);
    nodes_.insert(n2);
    weights_.insert({edge, w});
    weights_.insert({back_edge, w});
    std::cout << n1 << " <-> " << n2 << " weight: " << w << "\n";
  }

  Weight solve_tsp() const {
    Weight min_weight = ~0U;
    visit_all_perms([&min_weight](Weight w) {
      if (w < min_weight) {
        std::cout << "\nNew minimum weight: " << w << "\n";
        min_weight = w;
      }
    });
    return min_weight;
  }

  Weight solve_max_tsp() const {
    Weight max_weight = 0;
    visit_all_perms([&max_weight](Weight w) {
      if (w > max_weight) {
        std::cout << "\nNew maximum weight: " << w << "\n";
        max_weight = w;
      }
    });
    return max_weight;
  }

private:
  template <typename Fn> void visit_all_perms(Fn fn) const {
    std::vector<Node> nodes(nodes_.begin(), nodes_.end());
    std::sort(nodes.begin(), nodes.end());
    do {
      Weight current_weight = 0;
      std::cout << "\r" << nodes[0];
      for (std::size_t i = 1; i < nodes.size(); ++i) {
        std::cout << " <- " << nodes[i] << std::flush;
        Edge e = std::make_pair(nodes[i - 1], nodes[i]);
        auto it = weights_.find(e);
        assert(it != weights_.end());
        current_weight += it->second;
      }
      std::cout << "         " << std::flush;
      fn(current_weight);
    } while (std::next_permutation(nodes.begin(), nodes.end()));
    std::cout << "\n";
  }
  Nodes nodes_;
  Edges weights_;
};

int main(int argc, char **argv) {
  Graph g;

  // Parse the input
  for (std::string line; std::getline(std::cin, line);) {
    g.add_edge(line);
  }

  auto w = g.solve_tsp();
  std::cout << "Minimum weight = " << w << "\n";
  w = g.solve_max_tsp();
  std::cout << "Maximum weight = " << w << "\n";
  return 0;
}