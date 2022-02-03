#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>
#include "digital_pin_handler.h"

bool lexer(char *payload, int &pin);
void payload_parser(char *payload);

#endif
