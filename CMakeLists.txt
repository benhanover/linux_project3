cmake_minimum_required(VERSION 3.13)
project(project2)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable C++20 standard

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-w)  # Suppress all warnings

# Find libzip package
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED IMPORTED_TARGET libzip)

# Set your source files
set(MAIN RunMe.cpp)
file(GLOB MISSIONS CONFIGURE_DEPENDS "./missions/*.cpp")

# Include the utilities directory
add_subdirectory(utilities)

# Create the executable and link against libzip and utilities
add_executable(runMe ${MAIN} ${MISSIONS})
target_link_libraries(runMe PRIVATE PkgConfig::LIBZIP utilities)