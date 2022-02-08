#ifndef SEND_MESSAGE_H
#define SEND_MESSAGE_H

#include <Arduino.h>

#ifndef MAX_SIZE_PAYLOAD
    #define MAX_SIZE_PAYLOAD 25
#endif

void send_message(char *message);

#endif
