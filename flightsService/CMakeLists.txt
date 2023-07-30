cmake_minimum_required(VERSION 3.13)
project(project3)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable C++20 standard

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-w)  # Suppress all warnings

# Set your source files
set(MAIN flightsServiceMain.cpp)
set(SOURCES flightsServiceFuncs.cpp)

# Create the executable
add_executable(flightsService ${MAIN} ${SOURCES})
