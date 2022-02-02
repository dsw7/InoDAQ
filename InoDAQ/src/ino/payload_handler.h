#ifndef PAYLOAD_HANDLER_H
#define PAYLOAD_HANDLER_H

#include <Arduino.h>

void set_pin_status(int *pin);
bool lexer(char *payload, int &pin);
void payload_handler(char *payload);

#endif
