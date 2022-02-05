#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>
#include "digital_pin_handler.h"
#include "led_builtin.h"

bool digital_pin_lexer(char *payload, int &pin);
void help_menu();
void payload_parser(char *payload);

#endif
