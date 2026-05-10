/*
    . Project description
        . Topic #1
        . Topic #2
*/

// #include <fmt/format.h>
#include <fmt/base.h>
#include <fmt/format.h>
// #include <iostream>

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


    read_datatypes(false);

    read_datatypes_2(false);

    read_integer_mod(false);

    
    // fmt::println("Enter a path");
    // std::string path;

    // std::cin >> path;

    // file_module::bulk_rename(path);

    get_loops(false);

    read_functions(false);
    
    read_arr_vec(false);

    read_optionals_func(false);

    read_reference(false);

    read_ptrs_and_smrt_ptrs(false);

    read_classes_and_structs(true);

    return 0;
}