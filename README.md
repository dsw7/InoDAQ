# InoDAQ
A low cost, primitive alternative to some commercially available digital I/O tools. I built this software
mainly to troubleshoot some logic circuits I was messing around with in my spare time. This software
essentially consists of a C++ `ncurses` interface that toggles the GPIO pins on an Arduino device by sending
and receiving messages via UART.
## Table of Contents
  - [Setup](#installation)
    - [Configuring `arduino-cli`](#configuring-arduino-cli)
      - [Install `arduino-cli`](#install-arduino-cli)
      - [Compile the Arduino code](#compile-the-arduino-code)
      - [Upload the Arduino code](#upload-the-arduino-code)
    - [Quick setup](#quick-setup)
    - [Setting up the presentation layer](#setting-up-the-presentation-layer)
    - [Setting up the hardware control layer](#setting-up-the-hardware-control-layer)
    - [Example: build product from end-to-end](#example-build-product-from-end-to-end)
  - [Usage](#usage)
    - [Step 1](#step-1)
    - [Step 2](#step-2)
    - [Step 3](#step-3)
    - [Step 4](#step-4)
    - [Step 5](#step-5)
  - [Example - testing a DM74LS00N NAND gate](#example---testing-a-dm74ls00n-nand-gate)
    - [A = 0 & B = 0](#a--0--b--0)
    - [A = 1 & B = 0](#a--1--b--0)
    - [A = 0 & B = 1](#a--0--b--1)
    - [A = 1 & B = 1](#a--1--b--1)
  - [Testing and troubleshooting](#testing-and-troubleshooting)
    - [Automated testing](#automated-testing)
    - [Troubleshooting with `strace`](#troubleshooting-with-strace)
## Setup
Setup can be broken down into two components. First, the _presentation layer_ (i.e. the C++ `ncurses`
interface) must be compiled. Next, the _hardware control layer_ must be compiled and uploaded to the Arduino
device. **NOTE:** Setup assumes that `arduino-cli` is set up and configured to upload to an Arduino device. See
[Configuring `arduino-cli`](#configuring-arduino-cli) if `arduino-cli` has not been configured.
### Configuring `arduino-cli`
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
**NOTE:** Cygwin users may encounter an issue with `COM` ports and `dev` device file naming. The general
mapping follows:
```
ttySn => COM(n + 1)
```
For example, one would pass:
```bash
arduino-cli upload --port COM3 --fqbn <fqbn> src/ino/
```
If plugging in the device allocates the `/dev/ttyS2` device file.
### Quick setup
To set up the product from end to end, first `git clone` this repository, change directories into the
repository,
then run:
```bash
make full SERIAL_PORT=<serial-port-or-device-file>
```
This `make` target will first compile, upload and test the hardware control layer, then compile and test the
presentation layer. The hardware control and presentation layers can be built individually as well. A full
list of targets and their descriptions can be obtained by running:
```bash
make
# OR
make help
```
### Setting up the presentation layer
To compile an `inodaq` binary, run:
```bash
make full-cpp SERIAL_PORT=<serial-port-or-device-file>
```
This chain of commands will compile a binary named `inodaq` and place the binary under `/tmp/build/`. The
binary
can and should be moved to a convenient location such as under `$PATH`.
### Setting up the hardware control layer
To set up the hardware control layer, change directories from the project root to `src/ino` and upload the
`ino.ino` sketch via the Arduino CLI or the Arduino GUI. I strongly recommend using the CLI and am including
instructions here.
To set up the hardware control layer (i.e. upload the Arduino code to the device), run:
```bash
make full-ino SERIAL_PORT=<serial-port-or-device-file>
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
## Example - testing a DM74LS00N NAND gate
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
This package includes some primitive automated tests for testing the hardware control layer. To run the
tests, change directories to the project root and run:
```shell
python3 -m pytest -vs .
```
After compiling and uploading the Arduino code to the device.
### Troubleshooting with `strace`
The `strace` utility can be used to troubleshoot communication issues between the host and the device given
that the host writes to and reads from a device file (i.e. makes `read` and `write` system calls). To
troubleshoot writes, run
```bash
strace -s1000 -e write /tmp/inodaq <arguments>
```
For example:
```bash
strace -s1000 -e write /tmp/inodaq --ping --quiet --serial-port /dev/ttyUSB0
```
Notice in the above command that the `--quiet` flag was passed. This will suppress writing to `stdout` and
`stderr`, or file descriptors 1 and 2, respectively. Therefore the output follows:
```
write(3, "test\n", 5)                   = 5
write(3, "test\n", 5)                   = 5
+++ exited with 0 +++
```
The `--ping` command writes the string `test\n` to the `ttyUSB0` device file (whom was allocated file
descriptor 3).  Writes to `stdout` can, of course, be enabled by omitting the `--quiet` flag:
```bash
strace -s1000 -e write /tmp/inodaq --ping --serial-port /dev/ttyUSB0 >/dev/null
```
The output follows:
```
write(1, "\33[32m[ I ]\33[0m Running ping command\n", 36) = 36
...
write(1, "\33[32m[ I ]\33[0m Writing message 'test' to device\n", 48) = 48
write(3, "test\n", 5)                   = 5
write(1, "\33[32m[ I ]\33[0m Successfully wrote out 5 bytes to serial port\n", 61) = 61
write(1, "\33[32m[ I ]\33[0m Reading data from serial port\n", 45) = 45
write(1, "\33[32m[ I ]\33[0m Number of bytes read from serial port: 25\n", 57) = 57
write(1, "\33[32m[ I ]\33[0m Received message from device: 'Built in LED is ONxxxxxx'\n", 72) = 72
write(1, "\33[32m[ I ]\33[0m Built in LED should now turn off\n", 48) = 48
write(1, "\33[32m[ I ]\33[0m Writing message 'test' to device\n", 48) = 48
write(3, "test\n", 5)                   = 5
...
+++ exited with 0 +++
```
The `strace` utility can also be used to troubleshoot bidirectional I/O. For example:
```bash
strace -s1000 -e read,write /tmp/inodaq --ping --quiet --serial-port /dev/ttyUSB0 >/dev/null
```
The output follows:
```
...
write(3, "test\n", 5)                   = 5
read(3, "Built in LED is ONxxxxxxx", 25) = 25
write(3, "test\n", 5)                   = 5
read(3, "Built in LED is OFFxxxxxx", 25) = 25
+++ exited with 0 +++
```
The above example demonstrates proper read/write sequence.
