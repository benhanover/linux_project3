cmake_minimum_required(VERSION 3.13)
project(project3)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)              # Enable C++20 standard

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-w)  # Suppress all warnings

# Find libzip package
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED IMPORTED_TARGET libzip)

# Set your source files
set(MAIN dbServiceMain.cpp)
file(GLOB MISSIONS CONFIGURE_DEPENDS "./missions/*.cpp")

# Include the utilities directory
add_subdirectory(utilities)

# Add the dbServiceFuncs.cpp file to the sources
set(DB_SERVICE_FUNCS dbServiceFuncs.cpp)

# Create the executable and link against libzip, utilities, and dbServiceFuncs.cpp
add_executable(dbService ${MAIN} ${MISSIONS} ${DB_SERVICE_FUNCS})
target_link_libraries(dbService PRIVATE PkgConfig::LIBZIP utilities)
