# InoDAQ
A low cost, primitive alternative to some commercially available digital I/O tools. I built this software
mainly to troubleshoot some logic circuits I was messing around with in my spare time. This software
essentially consists of a C++ `ncurses` interface that toggles the GPIO pins on an Arduino device by sending
and receiving messages via UART.
## Table of Contents
  - [Setup](#installation)
    - [Setting up the presentation layer](#setting-up-the-presentation-layer)
    - [Setting up the hardware control layer](#setting-up-the-hardware-control-layer)
  - [Usage](#usage)
    - [Step 1](#step-1)
    - [Step 2](#step-2)
    - [Step 3](#step-3)
    - [Step 4](#step-4)
    - [Step 5](#step-5)
  - [Example](#example)
    - [Row A](#row-a)
    - [Row B](#row-b)
    - [Row C](#row-c)
    - [Row D](#row-d)
## Setup
Setup can be broken down into two components. First, the _presentation layer_ (i.e. the C++ `ncurses`
interface) must be compiled. Next, the _hardware control layer_ must be compiled and uploaded to the Arduino
device.
### Setting up the presentation layer
To compile a binary, first change directories to this project's root, then run:
```bash
cmake -S src/cpp/ -B build/ && make -j12 -C build/
```
This chain of commands will compile a binary named `inodaq` and place the binary onto the host's temporary
directory. The binary can and should be moved to a convenient location such as under `$PATH`.
### Setting up the hardware control layer
To set up the hardware control layer, change directories from the project root to `src/ino` and upload the
`ino.ino` sketch via the Arduino CLI or the Arduino GUI.
## Usage
### Step 1
Start off by plugging in the device into a free USB port. At this stage, it is assumed that the code has been
compiled and uploaded to device according to the [Setting up the hardware control
layer](#setting-up-the-hardware-control-layer) section.
### Step 2
Next, check the `dev` directory to determine which device file was allocated for the new USB connection. For
example, before plugging in the device, I have:
```bash
$ ls -l /dev | grep ttyS
crw-rw-rw-  1 David None 117,   0 Feb 21 00:37 ttyS0
```
In this case, the device file `ttyS0` corresponds to my modem. Next, after plugging in the device, I have:
```bash
$ ls -l /dev | grep ttyS
crw-rw-rw-  1 David None 117,   0 Feb 21 00:38 ttyS0
crw-rw-rw-  1 David None 117,   2 Feb 21 00:38 ttyS2
```
### Step 3
The device file corresponding to the serial port is `/dev/ttyS2`. Therefore, the presentation layer can be
used to connect to the device by attaching to this device file. Run:
```bash
/path/to/inodaq --serial-port /dev/ttyS2 --interface
```
Which will open a new interface:
```
==============================================================================================

  > [ ]  D2
    [ ]  D3
    [ ]  D4
    [ ]  D5
    [ ]  D6
    [ ]  D7
    [ ]  D8
    [ ]  D9
    [ ]  D10
    [ ]  D11
    [ ]  D12
    [ ]  D13

    Status: All digital pins are low

==============================================================================================

  Key             Description

  j, KEY_DOWN     Scroll cursor down
  k, KEY_UP       Scroll cursor up
  c               Connect to serial port specified via command line
  d               Disconnect from serial port specified via command line
  ENTER           Toggle pin at current cursor position
  q               Quit the program

==============================================================================================
```
### Step 4
Connect to the device. To connect to the device, press the `c` key. This step will take 2 seconds as the
device resets. The interface follows if a connection is successfully opened:
```
==============================================================================================

  > [ ]  D2
    [ ]  D3
    [ ]  D4
    [ ]  D5
    [ ]  D6
    [ ]  D7
    [ ]  D8
    [ ]  D9
    [ ]  D10
    [ ]  D11
    [ ]  D12
    [ ]  D13

    Status: Connected on port /dev/ttyS2

==============================================================================================

  Key             Description

  j, KEY_DOWN     Scroll cursor down
  k, KEY_UP       Scroll cursor up
  c               Connect to serial port specified via command line
  d               Disconnect from serial port specified via command line
  ENTER           Toggle pin at current cursor position
  q               Quit the program

==============================================================================================
```
To disconnect, simply press `d`.
### Step 5
The pins on the device can now be toggled. For example, to set pin `D3` to high, press the `j` or down arrow
key to position the cursor over the `D3` switch, then press enter. To set `D3` to low, press enter again.
## Example
In this basic example, I test a DM74LS00N NAND gate. See [this datasheet](./docs/74LS00_datasheet_fairchild.pdf). From this datasheet, I derive the following truth table:
| Row | D2 | D3 | Y |
| --- | -- | -- | - |
| A   | 0  | 0  | 1 |
| B   | 1  | 0  | 1 |
| C   | 0  | 1  | 1 |
| D   | 1  | 1  | 0 |
### Row A
### Row B
### Row C
### Row D
