.PHONY = \
	help \
	check-env \
	compile-ino upload-ino test-ino full-ino \
	compile-cpp test-cpp full-cpp \
	full

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"
FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno

PATH_INO_SRC = src/ino
PATH_CPP_SRC = src/cpp
CMAKE_BINARY_DIR = /tmp/build
CMAKE_BINARY = $(CMAKE_BINARY_DIR)/inodaq

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

check-env:
ifndef SERIAL_PORT
	$(error SERIAL_PORT is not set. Run "make <target> SERIAL_PORT=<serial-port-or-device-file>")
endif

compile-ino: check-env
	$(call MESSAGE,Compiling hardware control layer code)
	@arduino-cli compile --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

upload-ino: check-env
	$(call MESSAGE,Uploading hardware control layer code)
	@arduino-cli upload --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose $(PATH_INO_SRC)/

test-ino: check-env
	$(call MESSAGE,Running basic tests to ensure hardware control layer is working properly)
	@python3 -m pytest $(PATH_INO_SRC)/tests/ --port $(SERIAL_PORT)

full-ino: compile-ino upload-ino test-ino
	$(call MESSAGE,Hardware control layer successfully built!)

compile-cpp:
	$(call MESSAGE,Compiling presentation layer code)
	@cmake -S $(PATH_CPP_SRC)/src/ -B $(CMAKE_BINARY_DIR)/ && make -j12 -C $(CMAKE_BINARY_DIR)/

test-cpp: check-env
	$(call MESSAGE,Testing presentation layer code)
	@python3 -m pytest $(PATH_CPP_SRC)/tests/ --port $(SERIAL_PORT) --binary $(CMAKE_BINARY)

full-cpp: compile-cpp test-cpp
	$(call MESSAGE,Presentation layer successfully built!)

full: full-ino full-cpp
	$(call MESSAGE,Full product successfully built!)
