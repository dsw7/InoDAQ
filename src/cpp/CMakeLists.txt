cmake_minimum_required(VERSION 3.8.1)
project(InoDAQ)

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# Set binary name
set(OUTPUT_NAME inodaq)

# Set where to dump binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/tmp/bin")
message(STATUS "Compiled binary will be ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}")

# Avoid using glob patterns
add_executable(${OUTPUT_NAME}
    "main.cpp"
)
