.PHONY = help compile upload

LIGHT_PURPLE = "\033[1;1;35m"
NO_COLOR = "\033[0m"
FULLY_QUALIFIED_BOARD_NAME = arduino:avr:uno
KERNEL := $(shell uname --kernel-name)
PYTHON_INTERP = /usr/bin/python3.9

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
Compile Arduino code:
    $$ make compile
Upload compiled Arduino code to board:
    $$ make upload
Make all targets:
    $$ make full
endef

export HELP_LIST_TARGETS

help:
	@echo "$$HELP_LIST_TARGETS"

compile:
	$(call MESSAGE,Compiling Arduino code)
	@arduino-cli compile --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose

upload: compile
	$(call MESSAGE,Uploading Arduino code)
	@arduino-cli upload --port $(SERIAL_PORT) --fqbn $(FULLY_QUALIFIED_BOARD_NAME) --verbose

test:
	$(call MESSAGE,Running basic test)
	@$(PYTHON_INTERP) -m pytest --verbose -o log_cli=true -o log_cli_level=DEBUG \
	-o log_format="%(asctime)s.%(msecs)03d %(levelname)s %(message)s" . \
	-o log_date_format="%H:%M:%S" .

full: upload test
