cmake_minimum_required(VERSION 3.16)
project(RegulusDerivative LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Wpedantic")

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/poly.cpp
    src/frac.cpp
    src/parser.cpp
)

# Create executable
add_executable(code ${SOURCES})

# Set output directory
set_target_properties(code PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})