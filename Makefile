.PHONY = \
    help \
	compile-ino upload-ino test-ino full-ino \
	compile-cpp test-cpp full-cpp \
	full

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"
FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno
KERNEL := $(shell uname --kernel-name)
PYTHON_INTERP = /usr/bin/python3.9

PATH_INO_SRC = src/ino
PATH_CPP_SRC = src/cpp
CMAKE_BINARY_DIR = /tmp/build
CMAKE_BINARY = $(CMAKE_BINARY_DIR)/inodaq

ifneq (,$(findstring CYGWIN,$(KERNEL)))
	SERIAL_PORT = COM3
else
	SERIAL_PORT = /dev/ttyUSB0
endif

define MESSAGE
	@echo -e $(LIGHT_PURPLE)\> $(1)$(NO_COLOR)
endef

define HELP_LIST_TARGETS
To display all targets:
    $$ make help
Compile hardware control layer code:
    $$ make compile-ino
Upload compiled Arduino code to board:
    $$ make upload-ino
Test hardware control layer code:
    $$ make test-ino
Make all hardware control layer targets:
    $$ make full-ino
Compile presentation layer code:
    $$ make compile-cpp
Test presentation layer code:
    $$ make test-cpp
Make all presentation layer targets:
    $$ make full-cpp
Build entire product:
    $$ make full
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

compile-ino:
	$(call MESSAGE,Compiling hardware control layer code)
	@arduino-cli compile --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

upload-ino:
	$(call MESSAGE,Uploading hardware control layer code)
	@arduino-cli upload --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

test-ino:
	$(call MESSAGE,Running basic tests to ensure hardware control layer is working properly)
	@$(PYTHON_INTERP) -m pytest $(PATH_INO_SRC)/tests/

full-ino: compile-ino upload-ino test-ino

compile-cpp:
	$(call MESSAGE,Compiling presentation layer code)
	@cmake -S $(PATH_CPP_SRC)/ -B $(CMAKE_BINARY_DIR)/ && make -j12 -C $(CMAKE_BINARY_DIR)/

test-cpp:
	$(call MESSAGE,Testing presentation layer code)
	@$(CMAKE_BINARY) --ping --serial-port $(SERIAL_PORT)

full-cpp: compile-cpp test-cpp

full: full-ino full-cpp
	$(call MESSAGE,Product successfully built!)
