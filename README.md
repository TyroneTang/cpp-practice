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