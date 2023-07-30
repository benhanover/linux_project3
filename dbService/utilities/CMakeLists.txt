# Utilities Subdirectory

# Collect all the source files for utilities
file(GLOB UTILITIES_SOURCES
  *.cpp
)

# Create a library for utilities
add_library(utilities SHARED
  ${UTILITIES_SOURCES}
)

