# Chat Backup — C++ Learning Session

**Date:** 2026-05-05
**Context:** Learning C++ in this repo (CMake + vcpkg + C++23 modules + fmt library)

---

## 1. How to compile this project from the terminal

This is a CMake + vcpkg project that uses C++23 modules and the `fmt` library. It's set up with CMake presets, so you don't have to type out long CMake flags manually.

**Configure (only needed first time, or after editing CMakeLists/presets):**
```bash
cmake --preset linux-gcc
```
Creates the build files under `build/linux-gcc/`.

**Build (Debug):**
```bash
cmake --build --preset linux-gcc-debug
```

**Run the resulting executable:**
```bash
./build/linux-gcc/Debug/Rocket
```

The binary is named `Rocket` because of `project(Rocket ...)` in CMakeLists.txt.

**For a release build instead:**
```bash
cmake --build --preset linux-gcc-release
./build/linux-gcc/Release/Rocket
```

**Note on raw g++:** Since this project uses C++23 modules (`utilities.cppm` is imported via `import utilities;` in main.cpp) and the `fmt` library managed by vcpkg, a one-line `g++ main.cpp -o app` won't work — modules need a scan/build step that CMake/Ninja handles for you, and `fmt` needs to be located via the vcpkg toolchain. Stick with the preset commands.

**Prerequisite:** `VCPKG_ROOT` env var must be set. Run `echo $VCPKG_ROOT` to check.

---

## 2. Error: `ninja: error: loading 'build-Debug.ninja': No such file or directory`

The build directory was half-configured — it had the top-level `build.ninja` but was missing the per-config `build-Debug.ninja` that `--preset linux-gcc-debug` looks for. Re-running configure regenerates them.

---

## 3. Error: GCC doesn't support C++23 module scanning

```
The target named "Rocket" has C++ sources that may use modules, but the
compiler does not provide a way to discover the import graph dependencies.
```

You have **g++ 13.3**, but CMake's C++23 module scanner needs **GCC 14+**. Your **clang++ 21** supports modules just fine — and there's already a `linux-clang` preset in `CMakePresets.json` for exactly this case.

**Use the clang preset instead:**
```bash
cmake --preset linux-clang
cmake --build --preset linux-clang-debug
./build/linux-clang/Debug/Rocket
```

That writes into `build/linux-clang/` so it won't collide with the broken `build/linux-gcc/` directory.

**Optional cleanup:** `rm -rf build/linux-gcc`

---

## 4. Q: Can I use gcc instead because it's native to Linux?

**Myth-bust:** clang is just as native on Linux as gcc — it's the default toolchain on FreeBSD/macOS/Android NDK and a first-class citizen on every major Linux distro. Both are fine choices.

**To use gcc-14 in this project:** Ubuntu 24.04's default `build-essential` ships gcc 13, which is too old for C++23 modules. You need **gcc-14** (available in Ubuntu 24.04's normal repos — no PPA needed).

Edit Dockerfile lines 14-33 to add `g++-14` and `gcc-14` to the apt install list:

```dockerfile
    apt install -y \
    git \
    ...
    sudo \
    gcc-14 \
    g++-14

RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100
```

Then rebuild the container via VS Code: **Cmd/Ctrl+Shift+P → "Dev Containers: Rebuild Container"**.

**For learning C++, the compiler choice doesn't matter** — both produce the same C++23 behavior at this level. Clang is already installed and working, so just run the clang preset and move on.

---

## 5. Day-to-day build commands (with clang)

```bash
rm -rf build/linux-gcc
cmake --preset linux-clang
cmake --build --preset linux-clang-debug
./build/linux-clang/Debug/Rocket
```

After that first run, day-to-day you only need:
```bash
cmake --build --preset linux-clang-debug
./build/linux-clang/Debug/Rocket
```

You only re-run `cmake --preset linux-clang` when you change `CMakeLists.txt` or `CMakePresets.json`.

---

## 6. Q: Why does main.cpp have squiggly lines? Need to reload the container?

**No** — you don't need to rebuild the container. The squiggles are from **clangd** (the language server), not from the compiler.

### clangd needs to reload

Your `.vscode/settings.json` points clangd at `build/linux-clang/compile_commands.json`, which only got created when you ran `cmake --preset linux-clang`. clangd was probably already running before that and is using stale data.

**Fix:** In VS Code, hit **Ctrl+Shift+P → "clangd: Restart language server"**.

### The `import utilities;` squiggle may persist

C++23 modules support in clangd is still maturing. Even after restarting, clangd may not fully resolve `import utilities;` and the `sum` symbol that depends on it — because the module's compiled interface (`.pcm` file) only exists *after* you build, and clangd's module-aware indexing is experimental.

**The compiler doesn't care.** As long as the build succeeds and the binary runs, your code is correct. The squiggle is purely a tooling limitation.

---

## 7. Q: Which has better import linters — gcc or clang?

**Reframe:** the squiggles aren't from gcc *or* clang — they come from **clangd** (the language server), which runs separately from whichever compiler you build with. Different tools.

- **clangd** is the only mature C++23 module-aware language server. Part of LLVM/clang.
- **GCC has no equivalent.** No "gccd" — GCC compiles modules, but doesn't provide an IDE backend.
- Your `.vscode/settings.json` already wires up clangd and disables Microsoft IntelliSense.

**Even if you switched to building with g++-14, your editor squiggles would still come from clangd.** Compiler choice has zero effect on what you see in the editor.

### Linters specifically (clang-tidy)

- **clang-tidy** — part of LLVM, hundreds of checks, de facto standard, already installed in your container
- **GCC** has `-Wall -Wextra -Wpedantic` warnings but nothing comparable to clang-tidy as a separate tool

**Bottom line:** for editor experience and linting, the LLVM/clang ecosystem wins. For compiling, both produce equivalent C++23 binaries.

---

## 8. How to create another file and import it (Python analogy)

| Python | C++23 modules |
|---|---|
| Create `foo.py` | Create `foo.cppm` |
| `import foo` | `import foo;` |
| Done | **Also tell CMake about the file** |

Python finds `foo.py` automatically. C++ doesn't — you must list new module files in `CMakeLists.txt`.

### Step-by-step: adding a `math_helpers` module

**1. Create `math_helpers.cppm`:**
```cpp
module;

export module math_helpers;

export int multiply(int a, int b) {
    return a * b;
}

export int square(int n) {
    return n * n;
}
```

- `module;` — declares this is a module file (must be the first line)
- `export module math_helpers;` — names the module (this is what you `import`)
- `export` before each function — only `export`-ed things are visible to importers

**2. Edit CMakeLists.txt to add it to the FILE_SET:**
```cmake
target_sources(${PROJECT_NAME}
        PUBLIC
        FILE_SET CXX_MODULES FILES
        utilities.cppm
        math_helpers.cppm
)
```

**3. Use it from main.cpp:**
```cpp
import utilities;
import math_helpers;

int main(){
    int product = multiply(4, 5);
    int sq = square(6);
    fmt::println("multiply(4,5) = {}", product);
    fmt::println("square(6) = {}", sq);
    return 0;
}
```

**4. Re-configure and build (because CMakeLists.txt changed):**
```bash
cmake --preset linux-clang
cmake --build --preset linux-clang-debug
./build/linux-clang/Debug/Rocket
```

### Quick reference: what `export` does

```cpp
export module math_helpers;

export int multiply(int a, int b) { ... }   // visible when imported
int helper_only_used_here() { ... }          // private to this module
```

Think of `export` like Python's `__all__` — only marked things leak out.

---

## 9. Review of `src/datatypes.cppm`

Putting it under `src/` is a totally valid project layout, and the CMakeLists.txt entry `src/datatypes.cppm` is right (paths are relative to CMakeLists.txt). Two small issues:

### Module name doesn't match the file name

```cpp
export module get_datatypes;   // module is named "get_datatypes"
```
…but the file is `datatypes.cppm`. Compiles fine, but confusing. Convention: file name matches module name.
- File `datatypes.cppm` → `export module datatypes;`
- File `get_datatypes.cppm` → `export module get_datatypes;`

### Function name shadows the module name

```cpp
export module get_datatypes;
export void get_datatypes(bool is_enabled) { ... }   // same identifier!
```

C++ technically allows this (modules and functions live in different name domains), but it makes code hard to read. Rename one:

```cpp
export module datatypes;

export void print_datatypes(bool is_enabled) { ... }
```

### Don't forget to import in main.cpp

```cpp
import utilities;
import datatypes;        // <-- add this

// ...inside main():
print_datatypes(true);
```

---

## 10. Q: Why does float have `f` trailing but double doesn't?

In C++, **a number with a decimal point is a `double` by default**. The `f` suffix says "no, I want this to be a `float` instead."

```cpp
1.1231231231231     // type: double (the default)
1.2321312f          // type: float (the 'f' overrides the default)
```

### Why C++ chose `double` as the default

`double` has more precision (~15-17 significant digits) vs. `float` (~6-7 digits). Designers picked the safer default and made you opt into the smaller type explicitly.

### What happens without the `f`?

```cpp
float x = 1.5;        // 1.5 is a double; gets narrowed to float when assigned
float y = 1.5f;       // 1.5f is already a float; clean assignment
```

With `-Wnarrowing` and brace-init:
```cpp
float x {1.5};        // narrowing conversion warning/error
float x {1.5f};       // clean
```

This codebase uses brace-initialization (`int num_1 {5};`), so getting in the habit of writing `f` for float literals saves warnings.

### Other suffixes

| Suffix | Type | Example |
|---|---|---|
| (none) | `int` for whole numbers | `42` |
| (none) | `double` for decimals | `3.14` |
| `f` / `F` | `float` | `3.14f` |
| `l` / `L` | `long` (or `long double` for decimals) | `42L`, `3.14L` |
| `u` / `U` | `unsigned` | `42u` |
| `ll` / `LL` | `long long` | `42LL` |
| `ull` | `unsigned long long` | `42ull` |

**Mental model:** the suffix is a type tag for the literal itself, not a conversion. `1.5f` is *born* as a float; `1.5` is *born* as a double.

---

## 11. Q: What kinds of apps need 15 digits of precision?

Honestly, fewer than people assume. `float`'s 7 digits is enough for most "normal" computing.

### Cases where you genuinely need `double`

- **GPS / mapping.** 1° of latitude ≈ 111 km. Float's 7 digits gives ~10 m precision — fine for a phone, terrible for surveying or self-driving cars. Double gives sub-millimeter.
- **Large-world games & simulations.** Float is fine near origin (0,0,0). In a space sim or huge open-world game, once you're 100 million units from origin, float precision drops to several meters and characters jitter visibly. (Famous example: Minecraft's "Far Lands.")
- **Scientific computing.** Astronomy (planetary positions over centuries), molecular dynamics (billions of timesteps), climate models — error accumulates with each step.
- **Numerical algorithms.** Matrix solvers, FFTs, anything where you multiply/add millions of numbers.
- **Timestamps.** Unix time in seconds since 1970 has ~10 digits before the decimal already. Want millisecond precision? Past 13 digits.

### What does *NOT* need double precision

- **Money** — counterintuitively, use **integers** (cents) or a decimal library, not double. `0.1 + 0.2 != 0.3` in floating-point. Use `int64_t` cents.
- **Most game graphics** — float is the GPU native type
- **Machine learning** — actually uses *less* than float: `float16`, `bfloat16`, even `int8`
- **Audio** — 24-bit integers or float is plenty
- **UI / pixel positions** — float is overkill, ints work

### Mental model

| Question | Answer |
|---|---|
| Multiplied/added millions of times? | use `double` |
| Position in a huge coordinate space? | use `double` |
| Money? | use **integer cents**, not float at all |
| Otherwise? | `float` is fine |

For learning C++, just use `double` by default — language default for a reason, no speed difference until GPU/SIMD work.

---

*End of chat backup.*
