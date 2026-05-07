/*
    . Project description
        . Topic #1
        . Topic #2
*/

// #include <fmt/format.h>
#include <fmt/base.h>
#include <fmt/format.h>
#include <iostream>
import utilities;
import datatypes;
import datatypes_2;
import files;


int main(){
    fmt::print("Hello, World!\n");
    fmt::print("One\n");
    fmt::print("Two\n");
    fmt::print("Three\n");

    int num_1 {5};
    int num_2 {7};

    int result = sum(num_1, num_2);

    fmt::println("Result from utils sum: {}", result);


    read_datatypes(true);

    read_datatypes_2(true);

    read_integer_mod(true);

    
    fmt::println("Enter a path");
    std::string path;

    std::cin >> path;

    file_module::bulk_rename(path);


    return 0;
}