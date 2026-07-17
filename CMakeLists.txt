cmake_minimum_required(VERSION 4.1)
project(CHIP-8-Emulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Packages
find_package(SDL3 REQUIRED)

add_executable(CHIP-8-Emulator SRC/main.cpp
SRC/SDL/Context/Context.cpp
SRC/SDL/Context/Context.hpp
SRC/SDL/Window/Window.cpp
SRC/SDL/Window/Window.hpp
SRC/Emulator/Emulator.cpp
SRC/Emulator/Emulator.hpp
SRC/Chip8/Chip8.cpp
SRC/Chip8/Chip8.hpp)

target_link_libraries(CHIP-8-Emulator PRIVATE SDL3::SDL3)