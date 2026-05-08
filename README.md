# General

## Compile commands
From `/workspace/project`:

1. Configure (only needed first time, or after editing CMakeLists/presets):


`cmake --preset linux-gcc`
This creates the build files under build/linux-gcc/ and tells CMake to use g++ with the vcpkg toolchain.

for clang use:
`cmake --preset linux-clang`

2. Build (Debug):


`cmake --build --preset linux-gcc-debug`
for clang:
`cmake --build --preset linux-clang-debug`


3. Run the resulting executable:


`./build/linux-gcc/Debug/Rocket`
The binary is named Rocket because of project(Rocket ...) in CMakeLists.txt:7.

## For a release build instead

`cmake --build --preset linux-gcc-release`
`./build/linux-gcc/Release/Rocket`

## Alternative: Clang preset
If you'd rather use clang++:


`cmake --preset linux-clang`
`cmake --build --preset linux-clang-debug`
`./build/linux-clang/Debug/Rocket`

## A note on "raw" g++

Since this project uses C++23 modules (utilities.cppm is imported via import utilities; in main.cpp:10) and the fmt library managed by vcpkg, a one-line g++ main.cpp -o app won't work — modules need a scan/build step that CMake/Ninja handles for you, and fmt needs to be located via the vcpkg toolchain. Stick with the preset commands above.

Prerequisite: VCPKG_ROOT env var must be set (referenced in CMakeLists.txt:4). Run echo $VCPKG_ROOT to check — if it's empty, the configure step will fail.

## Squiggly import lines
restart the language server
`ctrl + shift + p`

`clangd: Restart language server`


## Pointers vs References

**TL;DR:** Prefer references for non-owning aliases. Use smart pointers (`std::unique_ptr` / `std::shared_ptr`) for ownership. Use raw pointers only when you need null, reseating, or pointer arithmetic.

Pointers are the older, more flexible cousin of references. The two operators `*` and `&` have dual roles depending on context.

### The two operators have dual roles

| Symbol | In a declaration | In an expression |
|---|---|---|
| `*` | `int* p` — "pointer to int" | `*p` — "the value at p" (dereference) |
| `&` | `int& r` — "reference to int" | `&x` — "address of x" |

Same symbol, two roles. You learn to read by context.

### Pointer basics

A pointer is **a variable that holds a memory address**.

```cpp
int x = 5;          // x is at some address, say 0x7ffd1234
int* p = &x;        // p = address-of x  → p == 0x7ffd1234

// Now use the pointer:
fmt::println("{}", x);    // 5      — direct
fmt::println("{}", *p);   // 5      — dereference: "value at p"
fmt::println("{}", fmt::ptr(p));   // 0x7ffd1234 — the address itself

*p = 10;            // dereference + assign: changes what p points to
fmt::println("{}", x);    // 10     — x changed because p points at it
```

Mental model:

```
       ┌───────┐         ┌──────┐
   x:  │  10   │  ←──────│  p   │
       └───────┘         └──────┘
       (an int)        (holds x's address)
```

### Step-by-step

```cpp
// 1. Declare a normal variable
int x = 5;

// 2. Declare a pointer to int, and point it at x
int* p = &x;
//  ^      ^
//  type:  address-of operator gets x's location
//  pointer to int

// 3. Read through the pointer (dereference)
int y = *p;            // y == 5

// 4. Write through the pointer (dereference + assign)
*p = 99;               // x is now 99

// 5. Reseat the pointer (something refs can't do)
int z = 100;
p = &z;                // p now points at z, no more relation to x
*p = 200;              // z is now 200
```

### Pointer vs reference at a glance

| Feature | `int& r = x;` | `int* p = &x;` |
|---|---|---|
| Must init at declaration | yes | technically no (but should) |
| Can be `nullptr` / "nothing" | no | yes |
| Can be reseated | no | yes |
| Read the value | `r` (implicit) | `*p` (explicit) |
| Get the address | `&r` | `p` (already an address) |
| Modify the value | `r = 99;` | `*p = 99;` |
| Syntactic weight | feels like the original variable | always need `*` to access |

### `nullptr` — the "no value" pointer

Pointers can hold a special value meaning "points to nothing":

```cpp
int* p = nullptr;       // p doesn't point anywhere

if (p != nullptr) {     // always check before deref
    *p = 5;
}
*p = 5;                 // DEREFERENCING NULL — crash (segfault)
```

References cannot be null — that's one of their safety guarantees.

### A complete worked example

```cpp
void increment(int* p) {        // takes a pointer
    if (p == nullptr) return;   // null check
    *p += 1;                    // dereference + modify
}

void increment_ref(int& r) {    // takes a reference (cleaner)
    r += 1;                     // no deref syntax needed
}

int main() {
    int n = 10;

    increment(&n);              // pass address — must use &
    fmt::println("{}", n);      // 11

    increment_ref(n);           // pass directly — no syntax
    fmt::println("{}", n);      // 12
}
```

The reference version is cleaner — no `*`, no `&` at the call site, no null checks. That's why modern C++ prefers references for "this function modifies my value" use cases.

### Pointer arithmetic (briefly)

Pointers can be moved through memory:

```cpp
int arr[5] = {10, 20, 30, 40, 50};
int* p = arr;          // points to arr[0]

fmt::println("{}", *p);          // 10
fmt::println("{}", *(p + 1));    // 20  — next int over
fmt::println("{}", *(p + 2));    // 30
fmt::println("{}", p[2]);        // 30  — same as *(p + 2)

p += 3;
fmt::println("{}", *p);          // 40
```

This is how arrays work under the hood. References don't support this.

### Comparison with Rust

| C++ | Rust equivalent |
|---|---|
| `T*` (raw pointer) | `*const T` / `*mut T` (unsafe) |
| `T&` (reference) | `&mut T` (without exclusivity check) |
| `const T&` | `&T` (without exclusivity check) |
| `nullptr` | no equivalent for refs; `Option<&T>` for "maybe null" |

Rust has both raw pointers and references too — but raw pointers are `unsafe` to dereference, while references are safe and lifetime-checked. C++ raw pointers are the "do whatever" version, with no safety net.

### When to use which

| Use | Choose |
|---|---|
| Pass-by-reference for a function arg that's always present | `T&` or `const T&` |
| Function arg that may or may not be present | `T*` (or better, `std::optional<T>` / pointer to const) |
| Walking through arrays or buffers | `T*` (with arithmetic) |
| Ownership / heap-allocated objects | `std::unique_ptr<T>` / `std::shared_ptr<T>` (NOT raw `T*`) |
| Just aliasing a local | `T&` |

### Modern C++ guidance

- For function parameters: **prefer references** unless you need null
- For ownership: **never raw `new`/`delete`** — use `std::unique_ptr` / `std::shared_ptr`
- For "optional" arguments: `std::optional<T>` or `T*` (with null check)
- Raw pointers in modern code mostly mean "non-owning, may be null" — observation only

### Quick exercise to internalize it

```cpp
int x = 100;
int* p = &x;        // address-of x

fmt::println("x  = {}", x);     // 100
fmt::println("*p = {}", *p);    // 100  (deref)
fmt::println("p  = {}", fmt::ptr(p));   // some address

*p = 200;
fmt::println("x  = {}", x);     // 200  (changed via pointer)

int y = 50;
p = &y;             // reseat
fmt::println("*p = {}", *p);    // 50

int* null_p = nullptr;
fmt::println("null_p = {}", fmt::ptr(null_p));   // 0x0
// *null_p = 5;     // would crash
```

Note: `fmt::ptr(p)` is fmt's way to print pointer addresses cleanly. Plain `fmt::println("{}", p)` won't work for pointers — you have to wrap it in `fmt::ptr()`.
