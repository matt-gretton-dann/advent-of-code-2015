#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <numeric>

using namespace std::string_literals;

struct Item {
    std::string name_;
    unsigned long cost_;
    unsigned long damage_;
    unsigned long armour_;

    bool operator==(Item const& rhs) const { return name_ == rhs.name_; }
};

struct Person {
    Person(unsigned long hp, unsigned long damage, unsigned long armour) : name_("Enemy"), hp_(hp),
                                                                           cost_(std::numeric_limits<unsigned>::max()),
                                                                           damage_(damage),
                                                                           armour_(armour) {}

    Person(Item const &weapon, Item const &armour, Item const &left_ring, Item const &right_ring) :
            name_("Player: "s + weapon.name_ + ", "s + armour.name_ + ", "s + left_ring.name_ + ", " +
                  right_ring.name_),
            hp_(100),
            cost_(weapon.cost_ + armour.cost_ + left_ring.cost_ + right_ring.cost_),
            damage_(weapon.damage_ + armour.damage_ + left_ring.damage_ + right_ring.damage_),
            armour_(weapon.armour_ + armour.armour_ + left_ring.armour_ + right_ring.armour_) {}

    unsigned get_attacked(Person const &attacker) {
        auto damage{(attacker.damage_ > armour_) ? attacker.damage_ - armour_ : 1};
        hp_ = (hp_ > damage) ? hp_ - damage : 0;
        return hp_;
    }

    std::string name_;
    unsigned long hp_;
    unsigned long cost_;
    unsigned long damage_;
    unsigned long armour_;
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

bool me_beats_enemy(Person me, Person enemy) {
    while (true) {
        enemy.get_attacked(me);
        if (enemy.hp_ == 0) { return true; }
        me.get_attacked(enemy);
        if (me.hp_ == 0) { return false; }
    }
}

int main() {
    const std::string hp_begin{"Hit Points: "};
    const std::string d_begin{"Damage: "};
    const std::string a_begin{"Armor: "};

    const std::vector<Item> weapons{
            {"Dagger",     8,  4, 0},
            {"Shortsword", 10, 5, 0},
            {"Warhammer",  25, 6, 0},
            {"Longsword",  40, 7, 0},
            {"Greataxe",   74, 8, 0},
    };

    const std::vector<Item> armours{
            {"Nothing",    0,   0, 0},
            {"Leather",    13,  0, 1},
            {"Chainmail",  31,  0, 2},
            {"Splintmail", 53,  0, 3},
            {"Bandedmail", 75,  0, 4},
            {"Platedmail", 102, 0, 5},
    };

    const std::vector<Item> rings{
            {"Empty",      0,   0, 0},
            {"Damage +1",  25,  1, 0},
            {"Damage +2",  50,  2, 0},
            {"Damage +3",  100, 3, 0},
            {"Defense +1", 20,  0, 1},
            {"Defense +2", 40,  0, 2},
            {"Defense +3", 80,  0, 3},
    };


    auto enemy_hp{get_value(hp_begin)};
    auto enemy_damage{get_value(d_begin)};
    auto enemy_armour{get_value(a_begin)};

    Person best_result{0, 0, 0};
    Person worst_result{0, 0, 0};
    worst_result.cost_ = 0;
    Person enemy{enemy_hp, enemy_damage, enemy_armour};

    for (auto const &weapon: weapons) {
        for (auto const &armour: armours) {
            for (auto const &left_ring: rings) {
                for (auto const &right_ring: rings) {
                    if (left_ring == right_ring) continue;
                    Person me{weapon, armour, left_ring, right_ring};
                    if (me_beats_enemy(me, enemy)) {
                        if (me.cost_ < best_result.cost_) {
                            best_result = me;
                        }
                    }
                    else if (me.cost_ > worst_result.cost_) {
                        worst_result = me;
                    }
                }
            }
        }
    }

    std::cout << "Best person: " << best_result.name_ << " at cost of " << best_result.cost_ << '\n';
    std::cout << "Worst person: " << worst_result.name_ << " at cost of " << worst_result.cost_ << '\n';
}