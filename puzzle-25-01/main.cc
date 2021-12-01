#include <cstdint>
#include <string>
#include <iostream>

std::uint64_t lcg(std::uint64_t val)
{
    return (val * 252533) % 33554393;
}

int main()
{
    std::string line;
    std::getline(std::cin, line);

    using namespace std::string_literals;
    auto line_begin{"To continue, please consult the code grid in the manual.  Enter the code at row "s};
    auto line_middle{", column "s};
    auto line_end{"."s};

    assert(line.substr(0, line_begin.size()) == line_begin);
    line = line.substr(line_begin.size());
    std::size_t idx = 0;
    unsigned long target_row = std::stoul(line, &idx);
    line = line.substr(idx);
    assert(line.substr(0, line_middle.size()) == line_middle);
    line = line.substr(line_middle.size());
    unsigned long target_column = std::stoul(line, &idx);
    line = line.substr(idx);
    assert(line == line_end);

    unsigned long row{1};
    unsigned long column{1};
    std::uint64_t num{20151125};
    while (row != target_row || column != target_column) {
        if (row == 1) {
            row = column + 1;
            column = 1;
        }
        else {
            --row;
            ++column;
        }
        num = lcg(num);
    }

    std::cout << "Target row " << target_row << " column " << target_column << '\n';
    std::cout << "Row " << row << " column " << column << " num " << num << '\n';
    return 0;
}