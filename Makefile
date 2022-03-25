.PHONY = help compile-ino upload-ino test-ino compile-cpp full

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"
FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno
KERNEL := $(shell uname --kernel-name)
PYTHON_INTERP = /usr/bin/python3.9

PATH_INO_SRC = src/ino
PATH_CPP_SRC = src/cpp
PATH_BUILD_DIR = /tmp/build

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
Compile presentation layer code:
    $$ make compile-cpp
Make all targets:
    $$ make full
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

compile-ino:
	$(call MESSAGE,Compiling Arduino code)
	@arduino-cli compile --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

upload-ino: compile-ino
	$(call MESSAGE,Uploading Arduino code)
	@arduino-cli upload --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

test-ino:
	$(call MESSAGE,Running basic test)
	@$(PYTHON_INTERP) -m pytest $(PATH_INO_SRC)/tests/

compile-cpp:
	$(call MESSAGE,Compiling frontend code)
	@cmake -S $(PATH_CPP_SRC)/ -B $(PATH_BUILD_DIR)/ && make -j12 -C $(PATH_BUILD_DIR)/

full: upload-ino test-ino
