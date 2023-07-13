#!/bin/bash

# Get the current directory (project path)
project_directory="$(pwd)"

# Create a build directory if it doesn't exist
build_directory="$project_directory/build"
if [ ! -d "$build_directory" ]; then
  mkdir "$build_directory"
fi



# Navigate to the build directory
cd "$project_directory/DB"

chmod u+x ./flightScanner.sh
chmod u+x ./clean.sh

cd ../build

# Run CMake to generate the build files
cmake "$project_directory"

# Build the project using make
make

cd "$project_directory/DB"
