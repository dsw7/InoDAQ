cmake_minimum_required(VERSION 3.8.1)
project(InoDAQ)

# Do not support Windows or Mac OS X at the moment
if(UNIX)
    message(STATUS "Detected *nix platform. Proceeding.")
    if (APPLE)
        message(FATAL_ERROR "Detected Mac OS X *nix platform. Cannot proceed!")
    endif(APPLE)
else()
    message(FATAL_ERROR "Cannot compile on non-*nix platform!")
endif(UNIX)

# Use C++ 17 or later
set(CMAKE_CXX_STANDARD 17)

# Lots of warnings + all warnings as errors
add_compile_options(-Wall -Wextra -pedantic -Werror)

# Set binary name
set(OUTPUT_NAME inodaq)

# Print where binary will be dumped
message(STATUS "Compiled binary will be ${CMAKE_BINARY_DIR}/${OUTPUT_NAME}")

set(DEFAULT_SERIAL_PORT_DEV_X86_X86_64 "/dev/ttyS2")
set(DEFAULT_SERIAL_PORT_DEV_NIX "/dev/ttyUSB0")

# Macro definitions
add_compile_definitions(CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}") # See https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
add_compile_definitions(MAX_SLEEP_WAIT_FOR_AUTO_RESET=2)          # Number of seconds to wait for device to auto-reset
add_compile_definitions(RW_DELAY_USEC=50000)                      # Delay between writing to and reading from device
add_compile_definitions(TIMEOUT_SELECT_SECS=2)                    # select(2) timeout in seconds

if (CYGWIN)                                                       # ttyS0 is the device for the serial console on x86 and x86_64 arch
    add_compile_definitions(DEFAULT_SERIAL_PORT_DEVICE="${DEFAULT_SERIAL_PORT_DEV_X86_X86_64}")
    message(STATUS "The default serial port device will be ${DEFAULT_SERIAL_PORT_DEV_X86_X86_64}")
else()                                                            # All other *nix systems default to ttyUSB0 it seems
    add_compile_definitions(DEFAULT_SERIAL_PORT_DEVICE="${DEFAULT_SERIAL_PORT_DEV_NIX}")
    message(STATUS "The default serial port device will be ${DEFAULT_SERIAL_PORT_DEV_NIX}")
endif(CYGWIN)

# Link curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Avoid using glob patterns
add_executable(${OUTPUT_NAME}
    "main.cpp"
    "serial.cpp"
    "commands.cpp"
    "logging.cpp"
    "control_panel.cpp"
)

# Libs need to be linked after adding executable (i.e. ~ $ g++ <main.cpp> -lncurses)
target_link_libraries(${OUTPUT_NAME} ${CURSES_LIBRARIES})
