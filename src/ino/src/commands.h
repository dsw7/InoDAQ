#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>
#include <string.h>

void toggle_builtin_led(char *buffer);
bool parser(char* buffer_input, char* buffer_output);

#endif
