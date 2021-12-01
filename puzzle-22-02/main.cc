#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>

using namespace std::string_literals;

struct State {
    State(unsigned long enemy_hp, unsigned long enemy_damage) : enemy_hp_(enemy_hp), enemy_damage_(enemy_damage) {}

    unsigned long my_hp_{50};
    unsigned long my_mana_{500};
    unsigned long my_armour_{0};
    unsigned long enemy_hp_;
    unsigned long enemy_damage_;
    unsigned long shield_counter_{0};
    unsigned long poison_counter_{0};
    unsigned long recharge_counter_{0};
    unsigned long cost_{0};

    bool operator==(State const &rhs) const {
        return my_hp_ == rhs.my_hp_ && my_mana_ == rhs.my_mana_ && my_armour_ == rhs.my_armour_ &&
               enemy_hp_ == rhs.enemy_hp_ && enemy_damage_ == rhs.enemy_damage_ &&
               shield_counter_ == rhs.shield_counter_ && poison_counter_ == rhs.poison_counter_ &&
               recharge_counter_ == rhs.recharge_counter_;
    }

    bool apply_effects() {
        if (shield_counter_ > 0) {
            if (--shield_counter_ == 0) {
                my_armour_ -= 7;
            }
        }
        if (poison_counter_ > 0) {
            hit_enemy(3);
            --poison_counter_;
        }
        if (recharge_counter_ > 0) {
            my_mana_ += 101;
            --recharge_counter_;
        }
        return enemy_hp_ == 0;
    }

    void hit_enemy(unsigned long damage) {
        enemy_hp_ = (damage > enemy_hp_) ? 0 : enemy_hp_ - damage;
    }

    void hit_me() {
        auto amt{my_armour_ >= enemy_damage_ ? 1 : enemy_damage_ - my_armour_};
        my_hp_ = (amt > my_hp_) ? 0 : my_hp_ - amt;
    }

    void spend_mana(unsigned long amt) {
        assert(my_mana_ >= amt);
        my_mana_ -= amt;
        cost_ += amt;
    }

    void enemy_turn() {
        if (!apply_effects()) {
            hit_me();
        }
    }

    [[nodiscard]] State magic_missile() const {
        State next{*this};
        next.spend_mana(53);
        next.hit_enemy(4);
        next.enemy_turn();
        return next;
    }

    [[nodiscard]] State drain() const {
        State next{*this};
        next.spend_mana(73);
        next.hit_enemy(2);
        next.my_hp_ += 2;
        next.enemy_turn();
        return next;
    }

    [[nodiscard]] State shield() const {
        State next{*this};
        assert(shield_counter_ == 0);
        next.spend_mana(113);
        next.my_armour_ += 7;
        next.shield_counter_ = 6;
        next.enemy_turn();
        return next;
    }

    [[nodiscard]] State poison() const {
        State next{*this};
        assert(poison_counter_ == 0);
        next.spend_mana(173);
        next.poison_counter_ = 6;
        next.enemy_turn();
        return next;
    }

    [[nodiscard]] State recharge() const {
        State next{*this};
        assert(recharge_counter_ == 0);
        next.spend_mana(229);
        next.recharge_counter_ = 5;
        next.enemy_turn();
        return next;
    }
};

unsigned long get_value(std::string const &begin) {
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cerr << "Missing line in input\n";
        std::exit(1);
    }
    if (line.substr(0, begin.length()) != begin) {
        std::cerr << "Line doesn't begin with: " << begin << '\n';
        std::exit(1);
    }
    return std::stoul(line.substr(begin.length()));
}

/** \brief  Add \a state to \a states.
 *
 * @param states Vector of states
 * @param state  State to add.
 *
 * If there is already an element in \a states that is the same as \a state we don't add a new state, instead we just
 * update the cost to the minimum.
 */
void add_state(std::vector<State> &states, State const &state) {
    auto it = std::find(states.begin(), states.end(), state);
    if (it == states.end()) {
        states.push_back(state);
    } else if (it->cost_ > state.cost_) {
        it->cost_ = state.cost_;
    }
}

int main() {
    const std::string hp_begin{"Hit Points: "};
    const std::string d_begin{"Damage: "};

    auto enemy_hp{get_value(hp_begin)};
    auto enemy_damage{get_value(d_begin)};

    /* States - list of states we've generated.  */
    std::vector<State> states;
    states.emplace_back(enemy_hp, enemy_damage);

    /* Cost of the cheapest winning state so far.  */
    auto best_cost{std::numeric_limits<unsigned long>::max()};

    while (!states.empty()) {
        /* Get the lowest cost element in the list of states and process that.  */
        auto it = std::min_element(states.begin(), states.end(),
                                   [](State const &lhs, State const &rhs) { return lhs.cost_ < rhs.cost_; });
        State candidate{*it};
        states.erase(it);
        if (candidate.cost_ >= best_cost) {
            /* Because we've searched for the minimum element above we know that all future candidates are going to
             * cost more than the current best cost for winning - so just stop here.  */
            break;
        }

        /* Someone has died so this is a winning state - deal with it.  */
        if (candidate.my_hp_ == 0 || candidate.enemy_hp_ == 0) {
            if (candidate.enemy_hp_ == 0 && candidate.cost_ < best_cost) {
                best_cost = candidate.cost_;
            }
            continue;
        }

        /* Part two's addition - we lose health at the start of our turn.  */
        if (--candidate.my_hp_ == 0) {
            continue;
        }

        /* Apply effects at start of our turn.  If someone dies push this state back on the queue.  */
        if (candidate.apply_effects()) {
            states.push_back(candidate);
        }

        /* Handle each of our options (followed by the enemy's turn).  Making sure that when we add the state we
         * don't duplicate ourselves.
         */
        if (candidate.my_mana_ >= 53) {
            add_state(states, candidate.magic_missile());
        }

        if (candidate.my_mana_ >= 73) {
            add_state(states, candidate.drain());
        }

        if (candidate.my_mana_ >= 113 && candidate.shield_counter_ == 0) {
            add_state(states, candidate.shield());
        }

        if (candidate.my_mana_ >= 173 && candidate.poison_counter_ == 0) {
            add_state(states, candidate.poison());
        }

        if (candidate.my_mana_ >= 229 && candidate.recharge_counter_ == 0) {
            add_state(states, candidate.recharge());
        }
    }

    std::cout << "Best cost " << best_cost << "\n";
    return 1;
}