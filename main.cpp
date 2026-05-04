/*
    . Project description
        . Topic #1
        . Topic #2
*/

// #include <fmt/format.h>
#include <fmt/base.h>
#include <fmt/format.h>
import utilities;


int main(){
    fmt::print("Hello, World!\n");
    fmt::print("One\n");
    fmt::print("Two\n");
    fmt::print("Three\n");

    int num_1 {5};
    int num_2 {7};

    int result = sum(num_1, num_2);

    fmt::println("Result from utils sum: {}", result);

    return 0;
}