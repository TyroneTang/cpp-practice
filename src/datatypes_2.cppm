module;

#include <fmt/base.h>

export module datatypes_2;

export void read_datatypes_2(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_datatypes_2 module is not enabled. skipping...");
        return;
    }
    int elephant_count; // init to junk, will assign a random val to allocate space in ram

    // diff between bracer and functional,
    // {} catches dataloss bugs, () does not.
    // example
    // int x{3.14};   // ❌ compile ERROR: narrowing conversion not allowed
    // int y(3.14);   // ✅ compiles, silently becomes 3
    // int z = 3.14;  // ✅ compiles, silently becomes 3
    
    
    
    // {} are braced initialization
    int lion_count{}; // init to zero, value is zero, same as int lion_count {0};

    int dog_count{10}; // value is 10.

    int cat_count{20};


    // initialized as expression
    int domesticated_animals { dog_count + cat_count };

    fmt::println("check value dog {}, cat {}, total {}", dog_count, cat_count, domesticated_animals);

    // () functional initialization
    // it works BUT never use this unless is for constructor of a class.
    // int apple_count(10);
    // int orange_count(20);
    // int fruit_count(apple_count + orange_count);
    
    // fmt::println("fruit count appl: {}, orange: {}, and total {}", apple_count, orange_count, fruit_count);

}


export void read_integer_mod(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_integer_mod module is not enabled. skipping...");
        return;
    }

    // allows both - and +
    signed int value {10};
    signed int value_2 {-100};
    signed int total = value + value_2;

    fmt::println("check total val, {}", total);

    // implicitly interpreted.
    int value_3 {-10};

    fmt::println("check total val 3, {}", value_3);

    // compiler error
    // unsigned int value_4 {-10};

    short int value_5 { -32768 };
    fmt::println("val 5 {}, with size {}, others int {}, short int {}, long int {}", value_5, sizeof(value_5), sizeof(int), sizeof(short int), sizeof(long int));
    
    long int value_6 { -3271010101010101010 };
    fmt::println("val 6 {}, with size {}", value_6, sizeof(value_6));

    // overall
    fmt::println("assigned values int {}, long int {}, long long int {}, unsigned long long int {}", sizeof(int), sizeof(long int), sizeof(long long int), sizeof(unsigned long long int));
}