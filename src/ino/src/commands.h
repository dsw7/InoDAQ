#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>
#include <string.h>

void toggle_builtin_led(char *buffer);
void toggle_digital_pin(char* buffer_input, char* buffer_output);
bool parser(char* buffer_input, char* buffer_output);

#endif
