cmake_minimum_required(VERSION 3.8.1)
project(InoDAQ)

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# Set binary name
set(OUTPUT_NAME inodaq)

# Set where to dump binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/tmp/bin")
message(STATUS "Compiled binary will be ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_NAME}")

# Macro definitions
add_compile_definitions(MAX_SIZE_PAYLOAD=25)              # Maximum payload size in bytes (must match device)
add_compile_definitions(MAX_SLEEP_WAIT_FOR_AUTO_RESET=2)  # Number of seconds to wait for device to auto-reset
add_compile_definitions(RW_DELAY_USEC=50000)              # Delay between writing to and reading from device

# Avoid using glob patterns
add_executable(${OUTPUT_NAME}
    "main.cpp"
    "serial.cpp"
    "commands.cpp"
    "logging.cpp"
)
