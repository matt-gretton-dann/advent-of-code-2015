#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <map>

enum class Op {
    hlf, tpl, inc, jmp, jie, jio
};
enum class Reg {
    a, b
};

struct Instr {
    static Op get_opcode(std::string const &str) {
        static std::map<std::string, Op> ops{
                {"hlf ", Op::hlf},
                {"tpl ", Op::tpl},
                {"inc ", Op::inc},
                {"jmp ", Op::jmp},
                {"jie ", Op::jie},
                {"jio ", Op::jio}
        };
        auto it = ops.find(str.substr(0, 4));
        assert(it != ops.end());
        return it->second;
    }

    static Reg get_register(std::string const &str) {
        assert(str.size() >= 1);
        if (str[0] == 'a') { return Reg::a; }
        else if (str[0] == 'b') { return Reg::b; }
        else { abort(); }
    }

    static long get_offset(std::string const &str) {
        return std::stol(str);
    }

    explicit Instr(std::string const &str) {
        op_ = get_opcode(str);
        switch (op_) {
            case Op::hlf:
            case Op::tpl:
            case Op::inc:
                reg_ = get_register(str.substr(4));
                pc_add_ = 1;
                assert(str.length() == 5);
                break;
            case Op::jmp:
                reg_ = Reg::a;
                pc_add_ = get_offset(str.substr(4));
                break;
            case Op::jie:
            case Op::jio:
                reg_ = get_register(str.substr(4));
                assert(str.at(5) == ',');
                assert(str.at(6) == ' ');
                pc_add_ = get_offset(str.substr(7));
                break;
            default:
                abort();
        }
    }

    Op op_;
    Reg reg_;
    long pc_add_;
};

struct State {
    unsigned long a_{0};
    unsigned long b_{0};
    unsigned long pc_{0};

    unsigned long& reg(Reg r) {
        switch (r) {
            case Reg::a: return a_;
            case Reg::b: return b_;
            default: abort();
        }
    }

    void execute(Instr const &instruction) {
        switch (instruction.op_) {
            case Op::hlf:
                reg(instruction.reg_) /= 2;
                pc_ += instruction.pc_add_;
                break;
            case Op::tpl:
                reg(instruction.reg_) *= 3;
                pc_ += instruction.pc_add_;
                break;
            case Op::inc:
                reg(instruction.reg_) += 1;
                pc_ += instruction.pc_add_;
                break;
            case Op::jmp:
                pc_ += instruction.pc_add_;
                break;
            case Op::jie:
                if (reg(instruction.reg_) % 2 == 0) pc_ += instruction.pc_add_;
                else pc_ += 1;
                break;
            case Op::jio:
                if (reg(instruction.reg_) == 1) pc_ += instruction.pc_add_;
                else pc_ += 1;
                break;
            default:
                abort();
        }
    }
};

int main() {
    std::vector<Instr> instructions;
    std::string line;
    while (std::getline(std::cin, line)) {
        instructions.emplace_back(line);
    }

    State state;
    while (state.pc_ < instructions.size()) {
        state.execute(instructions[state.pc_]);
    }
    std::cout << "a = " << state.a_ << "\nb = " << state.b_ << '\n';
}