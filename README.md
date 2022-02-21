# InoDAQ
A low cost, primitive alternative to some commercially available digital I/O tools. I built this software mainly to troubleshoot some logic circuits. This software essentially consists of a C++ `ncurses` interface that toggles the GPIO pins on an Arduino device by sending and receiving messages via UART.
## Table of Contents
  - [Setup](#installation)
    - [Compiling a binary](#compiling-a-binary)
## Setup
### Compiling a binary
To compile a binary, first change directories to this project's root, then run:
```bash
cmake -S src/cpp/ -B build/ && make -j12 -C build/
```
