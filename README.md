# InoDAQ
A low cost, primitive alternative to some commercially available digital I/O tools. I built this software mainly to troubleshoot some logic circuits. This software essentially consists of a C++ `ncurses` interface that toggles the GPIO pins on an Arduino device by sending and receiving messages via UART.
## Table of Contents
  - [Setup](#installation)
    - [Setting up the presentation layer](#setting-up-the-presentation-layer)
    - [Setting up the hardware control layer](#setting-up-the-hardware-control-layer)
## Setup
Setup can be broken down into two components. First, the _presentation layer_ (i.e. the C++ `ncurses` interface) must be compiled. Next, the _hardware control layer_ must be compiled and uploaded to the Arduino device.
### Setting up the presentation layer
To compile a binary, first change directories to this project's root, then run:
```bash
cmake -S src/cpp/ -B build/ && make -j12 -C build/
```
This chain of commands will compile a binary named `inodaq` and place the binary onto the host's temporary directory. The binary can and should be moved to a convenient location such as under `$PATH`.
### Setting up the hardware control layer
To set up the hardware control layer, change directories from the project root to `src/ino` and upload the `ino.ino` sketch via the Arduino CLI or the Arduino GUI.
