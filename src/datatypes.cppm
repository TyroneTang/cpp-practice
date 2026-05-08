module;

#include <limits>
#include <fmt/base.h>
#include <fmt/format.h>


// follow file name
export module datatypes;

export void read_datatypes(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("get datatypes module is not enabled. skipping...");
        return;
    }

    int number = 15;
    int number_2 = 017; // octal
    int number_3 = 0x0F; // hexadecimal
    int number_4 = 0b00001111; // binary

    fmt::println("check {}, {}, {}, {}", number, number_2, number_3, number_4);

    // without f, its default to be double.
    // float = 6 - 7 digit precision
    // double = 15 - 17 places precision.
    float single_precision = 1.125123123123121f;
    [[maybe_unused]]
    double double_precision = 1.125123123123121f;
    fmt::println("check floating, {:.3f}", single_precision);

    // only used this for if else control flow validation.
    fmt::println("check min {} max {}", std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}