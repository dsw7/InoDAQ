# InoDAQ
A low cost, primitive alternative to some commercially available digital I/O tools. I built this software
mainly to troubleshoot some logic circuits I was messing around with in my spare time. This software
essentially consists of a C++ `ncurses` interface that toggles the GPIO pins on an Arduino device by sending
and receiving messages via UART.
## Table of Contents
  - [Setup](#installation)
    - [Setting up the presentation layer](#setting-up-the-presentation-layer)
    - [Setting up the hardware control layer](#setting-up-the-hardware-control-layer)
      - [Install `arduino-cli`](#install-arduino-cli)
      - [Compile the Arduino code](#compile-the-arduino-code)
      - [Upload the Arduino code](#upload-the-arduino-code)
    - [Example: build product from end-to-end](#example-build-product-from-end-to-end)
  - [Usage](#usage)
    - [Step 1](#step-1)
    - [Step 2](#step-2)
    - [Step 3](#step-3)
    - [Step 4](#step-4)
    - [Step 5](#step-5)
  - [Example - testing a NAND gate](#example---testing-a-nand-gate)
    - [A = 0 & B = 0](#a--0--b--0)
    - [A = 1 & B = 0](#a--1--b--0)
    - [A = 0 & B = 1](#a--0--b--1)
    - [A = 1 & B = 1](#a--1--b--1)
  - [Testing and troubleshooting](#testing-and-troubleshooting)
    - [Automated testing](#automated-testing)
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
`ino.ino` sketch via the Arduino CLI or the Arduino GUI. I strongly recommend using the CLI and am including
instructions here.
#### Install `arduino-cli`
**NOTE:** These instructions vary strongly between boards. In this case I will only describe the installation
of `arduino-cli` for the Arduino Uno board.

Start by installing the `arduino-cli` suite if it is not installed:
```bash
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
```
This command will yield the `arduino-cli` binary. I had to move this binary under `/usr/bin`. Next update
the index:
```bash
arduino-cli core update-index
```
Search for cores after updating the index. Since I am using an Arduino Uno, I ran the following command:
```bash
arduino-cli core search uno
```
The command returned:
```
ID              Version Name
arduino:avr     1.8.5   Arduino AVR Boards
arduino:megaavr 1.8.7   Arduino megaAVR Boards
```
The core ID is `arduino:avr`. This is the core that I needed to install. To install the core, I ran:
```bash
arduino-cli core install arduino:avr
```
Last, confirm that the correct core was installed. In my case:
```bash
arduino-cli core list
ID          Installed Latest Name
arduino:avr 1.8.5     1.8.5  Arduino AVR Boards
```
#### Compile the Arduino code
To compile the code, change directories to the project root and run:
```bash
arduino-cli compile --fqbn <fqbn> src/ino/
```
The `<fqbn>` argument refers to the particular board's FQBN (Fully Qualified Board Name). For example, to get
the FQBN corresponding to my board, I ran:
```bash
arduino-cli board listall | grep -i uno
```
Which returned:
```
Arduino Uno                      arduino:avr:uno
Arduino Uno Mini                 arduino:avr:unomini
Arduino Uno WiFi                 arduino:avr:unowifi
```
In my case, the FQBN is `arduino:avr:uno`.
#### Upload the Arduino code
To upload the code, change directories to the project root and run:
```bash
arduino-cli upload --port <serial-port> --fqbn <fqbn> src/ino/
```
**NOTE:** Cygwin users may encounter an issue with `COM` ports and `dev` device file naming. The general mapping
follows:
```
ttySn => COM(n + 1)
```
For example, one would pass:
```bash
arduino-cli upload --port COM3 --fqbn <fqbn> src/ino/
```
If plugging in the device allocates the `/dev/ttyS2` device file.
### Example: build product from end-to-end
Here is a short example for building this entire product from end to end. The example assumes the following:
- The host's kernel is `SMP Debian 4.9.210-1`
- The FQBN is `arduino:avr:uno`
- The serial port is `/dev/ttyUSB0`
The command follows:
```bash
cmake -S src/cpp/ -B build/ \
&& make -j12 -C build/ \
&& arduino-cli compile --fqbn arduino:avr:uno src/ino/ \
&& arduino-cli upload --port /dev/ttyUSB0 --fqbn arduino:avr:uno src/ino/
```
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
## Example - testing a NAND gate
In this basic example, I test a DM74LS00N NAND gate. See [this
datasheet](./docs/74LS00_datasheet_fairchild.pdf). From this datasheet, I derive the following truth table:
| Row | D2 | D3 | Y |
| --- | -- | -- | - |
| A   | 0  | 0  | 1 |
| B   | 1  | 0  | 1 |
| C   | 0  | 1  | 1 |
| D   | 1  | 1  | 0 |
### A = 0 & B = 0
In this case, the program input follows:
```
==============================================================================

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

==============================================================================
```
Note that both `D2` and `D3` are low. Unsurprisingly, in this case the LED (Y) is logical true because both of
the inputs are logical false:
<p align="center">
  <img width="500" src=./docs/row1.jpg>
</p>

### A = 1 & B = 0
In this case, only `D2` was set to high:
```
==============================================================================

  > [x]  D2
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

    Status: Pin 2 was set to high

==============================================================================
```
In this case, the LED is logical true because at least one of the inputs is logical false:
<p align="center">
  <img width="500" src=./docs/row2.jpg>
</p>

### A = 0 & B = 1
In this case, only `D3` was set to high:
```
==============================================================================

    [ ]  D2
  > [x]  D3
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

    Status: Pin 3 was set to high

==============================================================================
```
In this case, the LED is logical true because at least one of the inputs is logical false:
<p align="center">
  <img width="500" src=./docs/row3.jpg>
</p>

### A = 1 & B = 1
In this case, both `D2` and `D3` were set to high:
```
==============================================================================

  > [x]  D2
    [x]  D3
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

    Status: Pin 2 was set to high

==============================================================================
```
In this case, the LED is logical false because both of the inputs are logical true:
<p align="center">
  <img width="500" src=./docs/row4.jpg>
</p>

## Testing and troubleshooting
### Automated testing
