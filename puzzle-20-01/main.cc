#include <string>
#include <iostream>
#include <cstdlib>

int main() {
    std::string line;
    std::getline(std::cin, line);

    auto target{std::stoul(line)};

    /* This is a really stupid way to do this in terms of effort, but it produces the right answer and doesn't involve
     * thinking too much:
     *
     * n is the current house we're at.
     */
    for (auto n{2UL}; true; ++n) {
        auto amt{0UL};
        /* Find the number of presents delivered to house `n`.  We do this by walking through all numbers <= sqrt(n) and
         * seeing if they are a factor.  If so we add presents for that number (i) and also (n/i), being careful not to
         * double count for square roots.  This reduces the amount of work we have to do significantly.
         */
        for (auto i{1UL}; i <= (n / i); ++i) {
            if (n % i == 0) {
                amt += i * 10;
                if (i != n / i) {
                    amt += (n / i) * 10;
                }
            }
        }
        if (amt >= target) {
            std::cout << "Target: " << target << " met at: " << n << " with amount: " << amt << '\n';
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}