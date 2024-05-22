cmake_minimum_required(VERSION 3.4...3.18)
project(pybindOS)

# unsures that the compiler is c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(pybind11)
pybind11_add_module(sortingEngine main.cpp startup.cpp process.cpp)