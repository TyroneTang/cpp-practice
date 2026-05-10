module;
#include <algorithm>
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <fmt/chrono.h>
#include <fmt/base.h>
#include <fmt/ranges.h>
#include <chrono>
#include <ctime>
#include <optional>

export module datatypes_2;

export void read_datatypes_2(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_datatypes_2 module is not enabled. skipping...");
        return;
    }
    [[maybe_unused]]
    int elephant_count; // init to junk, will assign a random val to allocate space in ram

    // diff between bracer and functional,
    // {} catches dataloss bugs, () does not.
    // example
    // int x{3.14};   // ❌ compile ERROR: narrowing conversion not allowed
    // int y(3.14);   // ✅ compiles, silently becomes 3
    // int z = 3.14;  // ✅ compiles, silently becomes 3
    
    
    
    // {} are braced initialization
    [[maybe_unused]]
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

export void read_char(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_char module is not enabled. skipping...");
        return;
    }
    char letter = {'a'};
    fmt::println("check char {}", letter);
  
    bool is_true { true };

    fmt::println("check if is true", is_true);

    std::string new_string {"hello world"};

    // constants
    // non mutable

    // const (known at runtime) vs constexpr (known at compile time, similar to rust &'str)
    // when initializing a val always use constexpr unless when created during runtime use const.
    [[maybe_unused]]
    const int c { 1337 };
    [[maybe_unused]]
    constexpr int d { 1338 };
    
}


export void get_loops(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("get_loops module is not enabled. skipping...");
        return;
    }

    // for loop
    const auto now = std::chrono::system_clock::now();
    const auto tt = std::chrono::system_clock::to_time_t(now);
    const std::tm t = *std::localtime(&tt);

    const auto subsec = now - std::chrono::time_point_cast<std::chrono::seconds>(now);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(subsec);
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(subsec - ms);

    fmt::println("{:02}:{:02}:{:02}:{:02}:{:03}:{:06}",
        t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
        ms.count(), ns.count()
    );
    
    
    for (long int counter { 0 }; counter < 10000000; counter ++) {
        // fmt::println("reading counter {}", counter);
    }

    const auto end = std::chrono::system_clock::now();
    const auto ttend = std::chrono::system_clock::to_time_t(end);
    const std::tm tend = *std::localtime(&ttend);

    const auto subsec_end = end - std::chrono::time_point_cast<std::chrono::seconds>(end);
    const auto ms_end = std::chrono::duration_cast<std::chrono::milliseconds>(subsec_end);
    const auto ns_end = std::chrono::duration_cast<std::chrono::nanoseconds>(subsec_end - ms_end);

    fmt::println("{:02}:{:02}:{:02}:{:02}:{:03}:{:06}",
        tend.tm_mday, tend.tm_hour, tend.tm_min, tend.tm_sec,
        ms_end.count(), ns_end.count()
    );
}

export void read_functions(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_functions module is not enabled. skipping...");
        return;
    }

    // lambda
    auto add = [](int a, int b) -> int {
        return a + b;
    };

    int result = add(1, 5);
    fmt::println("check lambda result {}", result);

}


export void read_arr_vec(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_functions module is not enabled. skipping...");
        return;
    }

    // old array (NON-DYAMIC but mutable)
    int num_arr[5] = {1, 2,3, 4,5};
    // assignment
    num_arr[2] = 20;

    // new c++ arr
    std::array<int, 5> new_arr = { 1, 2, 3, 4, 5 };
    new_arr[1] = 30; 

    new_arr.at(4) = 25;

    fmt::println("new arr val {}", new_arr);

    // vectors are dynamic arrays and mutable
    std::vector<int> num_vec { 1, 3, 5, 7, 8, 9 };
    num_vec.push_back(20);

    fmt::println("num vec val {}", num_vec);

    for (int el : num_vec) {
        fmt::println("get loop el {}", el);
    }
}


std::optional<int> getIfCool(int a) {
    if (a % 2 == 1 && a > 5) {
        return a;
    } else {
        return std::nullopt;
    }
}

export void read_optionals_func(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_functions module is not enabled. skipping...");
        return;
    }

    std::optional<int> val = getIfCool(10);

    if (not val.has_value()) {
        fmt::println("is nullllllllllllll!");
    } else {
        fmt::println("value is non-null of {}", val.value());
    }
}


export void read_reference(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_reference module is not enabled. skipping...");
        return;
    }

    int zulu { 7 };
    int& zulu_ref { zulu };

    fmt::println("chekc ref value bef change, {}", zulu_ref);

    // updates original and refernced
    zulu_ref = 8;

    fmt::println("chekc ref value aft change, ref: {}, origin {}", zulu_ref, zulu);

}


export void read_ptrs_and_smrt_ptrs(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_ptrs_and_smrt_ptrs module is not enabled. skipping...");
        return;
    }

    int a = 20;

    // &a => & - get address in memory of a
    // * => (pointer) stores the address of something (integer) and is named ptr
    int* ptr { &a };

    // deref the pointer *p to access value, otherwise its the memory address.
    fmt::println("value of a of poiner {}", *ptr);

    // hold ownership of data
    std::unique_ptr<int> smart_ptr{ std::make_unique<int>(10) };
    fmt::println("check new ownershipt {}", *smart_ptr);
}


export void read_classes_and_structs(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("read_classes_and_structs module is not enabled. skipping...");
        return;
    }


    // class
    class House {
        protected:
            int price;
            int height;
            int num_of_doors;
        
        public:
            House(int price, int height, int num_of_doors)
                :price { price }, height { height }, num_of_doors { num_of_doors } 
            {
                fmt::println("a new house is created!");
            }

            virtual void get_info() const {
                fmt::println("Price {}, height: {}, num of doors {}", price, height, num_of_doors);
            }

    };

    // create a new obj
    House new_home(400, 300, 10);
    new_home.get_info();



    // inheritance
    class SpecialHouse : public House {
        protected:
            int mult;
        public:
            // constructor
            SpecialHouse(int mult) : House(mult * 30, mult * 50, mult * 1), 
            // init member
            mult {mult}
            {
                fmt::println("A special house was craeted");
            }

            // const here means read only. No modification or return data from parameters without
            // modifications.
            void jump() const {
                fmt::println("The hse jumped!");
            }

            void increase_val(int amt) {
                price += amt;
            }

            void get_info() const override {
                fmt::println("check special with multiplier {}. Price ${}, h: {}, doors: {}", mult, price, height, num_of_doors);
            }
    };

    SpecialHouse new_special_house(30);
    new_special_house.get_info();
    new_special_house.jump();
    new_special_house.increase_val(500);
    new_special_house.get_info();


    bool is_determined = true;

    House* house;

    if (not is_determined)
        house = &new_home;

    else
        house = &new_special_house;

    house -> get_info();

}


export void algorithms(bool is_enabled) {
    if (!is_enabled) {
        fmt::println("algorithms module is not enabled. skipping...");
        return;
    }

    std::vector<int> new_vec {1, 2, 3, 5, 6, 1, 0, 20, 11, 222, 80, 90, 99, 60};
    std::sort(new_vec.begin(), new_vec.end());

    for (int num : new_vec) {
        fmt::println("{}", num);
    }

    fmt::println("is sorted!!!!!");
}