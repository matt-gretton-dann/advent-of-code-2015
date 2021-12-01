#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <map>
#include <numeric>
#include <set>

struct State {
    std::set<unsigned long> weights_;

    explicit State(unsigned long w) { weights_.insert(w); }

    std::size_t cost() const { return weights_.size(); }

    unsigned long qe() const {
        return std::accumulate(weights_.begin(), weights_.end(), 1UL,
                               [](unsigned long l, unsigned long w) { return l * w; });
    }

    unsigned long weight() const {
        return std::accumulate(weights_.begin(), weights_.end(), 0UL,
                               [](unsigned long l, unsigned long w) { return l + w; });
    }

    bool operator==(State const &rhs) const { return weights_ == rhs.weights_; }

    bool operator<(State const &rhs) const {
        if (cost() < rhs.cost()) { return true; }
        if (cost() == rhs.cost()) { return qe() < rhs.qe(); }
        return false;
    }
};

int main() {
    std::vector<unsigned long> weights;
    std::string line;
    while (std::getline(std::cin, line)) {
        weights.emplace_back(std::stoul(line));
    }

    std::set<State> states;
    unsigned long target_weight;
    for (auto w: weights) {
        states.insert(State{w});
        target_weight += w;
    }
    target_weight /= 3;

    while (!states.empty()) {
        auto it = states.begin();
        State current(*it);
        states.erase(it);

        if (current.weight() == target_weight) {
            std::cout << "Cost " << current.cost() << ", qe " << current.qe() << '\n';
            return 0;
        }
        if (current.weight() > target_weight) {
            continue;
        }

        for (auto w: weights) {
            if (current.weights_.find(w) == current.weights_.end()) {
                auto next{current};
                next.weights_.insert(w);
                if (states.find(next) == states.end()) {
                    states.insert(next);
                }
            }
        }
    }

    std::cout << "It went wrong somewhere!\n";
    return 1;
}