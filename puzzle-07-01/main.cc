#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <variant>

// helper type for the visitor #4
template <class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template <class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

enum class Action { Set, And, Or, LShift, RShift, Not };

std::ostream &operator<<(std::ostream &os, Action act) {
  switch (act) {
  case Action::Set:
    os << "SET";
    break;
  case Action::And:
    os << "AND";
    break;
  case Action::Or:
    os << "OR";
    break;
  case Action::LShift:
    os << "LSHIFT";
    break;
  case Action::RShift:
    os << "RSHIFT";
    break;
  case Action::Not:
    os << "NOT";
    break;
  }
  return os;
}

using Value = std::uint16_t;
using Wire = std::string;
using Signal = std::variant<Value, Wire>;

std::ostream &operator<<(std::ostream &os, Signal const &signal) {
  return std::visit(
      [&os](auto &&arg) -> std::ostream & {
        os << arg;
        return os;
      },
      signal);
}

/** \brief  An instruction.  */
struct Instruction {
  /** \brief  Construct an instruction.
   *
   * \subsection Grammar
   *
   *    wire   := [a-z]+
   *    value  := [0-9]+
   *    signal := value | wire
   *    set    := signal '->' wire
   *    not    := 'NOT' set
   *    op     := 'AND' | 'OR' | 'LSHIFT' | 'RSHIFT'
   *    binop  := signal op signal '->' wire
   *    instr  := binop | not | set
   */
  Instruction(std::string const &s) {
    if (parse_bin_op(s)) {
      return;
    }
    if (parse_not(s)) {
      return;
    }
    if (parse_set(s)) {
      return;
    }
    std::cout << "Unrecognised string: " << s << "\n";
    assert(false);
  }

  Action action() const noexcept { return act_; }
  Wire const &dest() const noexcept { return dest_; }
  Signal const &src1() const noexcept { return src1_; }
  Signal const &src2() const noexcept { return src2_; }

private:
  bool parse_not(std::string const &s) {
    if (s.substr(0, 4) == "NOT ") {
      std::string::size_type pos = 4;
      while (s[pos] == ' ') {
        ++pos;
      }
      return parse_set(s.substr(pos), Action::Not);
    }
    return false;
  }

  bool parse_bin_op(std::string const &s) {
    static const std::regex re("^([[:lower:][:digit:]]+) ([[:upper:]]+) "
                               "([[:lower:][:digit:]]+) -> ([[:lower:]]+)");
    std::smatch m;
    if (!std::regex_search(s, m, re)) {
      return false;
    }

    if (m.str(2) == "AND") {
      act_ = Action::And;
    } else if (m.str(2) == "OR") {
      act_ = Action::Or;
    } else if (m.str(2) == "LSHIFT") {
      act_ = Action::LShift;
    } else if (m.str(2) == "RSHIFT") {
      act_ = Action::RShift;
    } else {
      return false;
    }
    dest_ = m.str(4);
    src1_ = make_signal(m.str(1));
    src2_ = make_signal(m.str(3));
    std::cout << act_ << " " << dest_ << ", " << src1_ << ", " << src2_ << "\n";
    return true;
  }

  bool parse_set(std::string const &s, Action act = Action::Set) {
    static const std::regex re("^([[:lower:][:digit:]]+) -> ([[:lower:]]+)");
    std::smatch m;
    if (!std::regex_search(s, m, re)) {
      return false;
    }
    act_ = act;
    dest_ = m.str(2);
    src1_ = make_signal(m.str(1));
    std::cout << act_ << " " << dest_ << ", " << src1_ << "\n";
    return true;
  }

  Signal make_signal(std::string const &s) {
    if (std::isdigit(s[0])) {
      auto u = std::stoul(s, nullptr, 10);
      assert(u <= UINT16_MAX);
      return Signal(static_cast<std::uint16_t>(u));
    } else {
      return Signal(s);
    }
  }

  Action act_;
  Wire dest_;
  Signal src1_, src2_;
};

std::ostream &operator<<(std::ostream &os, Instruction const &instr) {
  os << instr.action() << " " << instr.dest() << ", " << instr.src1();
  if (instr.action() != Action::Set && instr.action() != Action::Not) {
    os << ", " << instr.src2();
  }
  return os;
}
using ValueMap = std::map<std::string, std::uint16_t>;
using Instructions = std::vector<Instruction>;

struct VM {
  void add_instr(Instruction const &instr) { instrs_.push_back(instr); }

  bool has_value(Wire const &w) const noexcept {
    return values_.find(w) != values_.end();
  }

  bool has_value(Signal const &s) const noexcept {
    return std::visit(Overloaded{[](Value v) { return true; },
                                 [&](Wire const &w) { return has_value(w); }},
                      s);
  }

  Value value(Wire const &w) const noexcept {
    assert(has_value(w));
    return values_.find(w)->second;
  }

  Value value(Signal const &s) const noexcept {
    return std::visit(Overloaded{[](Value v) { return v; },
                                 [&](Wire const &w) { return value(w); }},
                      s);
  }

  void value(Wire const &w, Value value) {
    auto [it, success] = values_.insert({w, value});
    assert(success);
  }

  void value(Signal const &s, Value v) {
    std::visit(Overloaded{[v](Value v2) { assert(v == v2); },
                          [&, v](Wire const &w) { value(w, v); }},
               s);
  }

  bool execute() {
    bool done_anything = false;
    for (auto const &instr : instrs_) {
      done_anything |= execute_instr(instr);
    }

    return done_anything;
  }

private:
  bool execute_instr(Instruction const &instr) {
    // First of all check there is something to do - that the destination
    // register has not been set already.

    std::cout << instr << " # ";
    Wire dest = instr.dest();
    if (has_value(dest)) {
      std::cout << "already has value: " << dest << " = " << value(dest)
                << "\n";
      return false;
    }

    switch (instr.action()) {
    case Action::Set: {
      Signal src = instr.src1();
      if (has_value(src)) {
        value(dest, value(src));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      } else {
        std::cout << "missing value for wire: " << src << "\n";
      }
      break;
    }
    case Action::Not: {
      Signal src = instr.src1();
      if (has_value(src)) {
        value(dest, ~value(src));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      }
      break;
    }
    case Action::And: {
      Signal src1 = instr.src1();
      Signal src2 = instr.src2();
      if (has_value(src1) && has_value(src2)) {
        value(dest, value(src1) & value(src2));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      }
    } break;
    case Action::Or: {
      Signal src1 = instr.src1();
      Signal src2 = instr.src2();
      if (has_value(src1) && has_value(src2)) {
        value(dest, value(src1) | value(src2));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      }
    } break;
    case Action::LShift: {
      Signal src1 = instr.src1();
      Signal src2 = instr.src2();
      if (has_value(src1) && has_value(src2)) {
        value(dest, value(src1) << value(src2));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      }
    } break;
    case Action::RShift: {
      Signal src1 = instr.src1();
      Signal src2 = instr.src2();
      if (has_value(src1) && has_value(src2)) {
        value(dest, value(src1) >> value(src2));
        std::cout << "setting wire to: " << dest << " = " << value(dest)
                  << "\n";
        return true;
      }
    } break;
    }

    return false;
  }

  ValueMap values_;
  Instructions instrs_;
};

int main(int argc, char **argv) {
  VM vm;

  for (std::string line; std::getline(std::cin, line);) {
    Instruction instr(line);
    vm.add_instr(instr);
  }

  bool changed = true;
  while (changed) {
    changed = vm.execute();
  }

  Wire a = Wire("a");
  std::cout << "a = ";
  if (!vm.has_value(a)) {
    std::cout << "UNSET\n";
  } else {
    std::cout << vm.value(a) << "\n";
  }

  return 0;
}