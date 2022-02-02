#include "payload_handler.h"

#define BAUD_RATE 9600
#define MAX_SIZE_PAYLOAD 12

void setup()
{
    Serial.begin(BAUD_RATE);

    for (unsigned int i = 2; i <= 13; i++)
    {
        pinMode(i, OUTPUT);
    }
}

void loop()
{
    if (Serial.available() > 0)
    {
        static char payload[MAX_SIZE_PAYLOAD];
        static unsigned int payload_idx = 0;

        char incoming_byte = Serial.read();

        if ((incoming_byte != '\n') and (payload_idx < MAX_SIZE_PAYLOAD -1))
        {
            payload[payload_idx] = incoming_byte;
            payload_idx++;
        }
        else
        {
            payload[payload_idx] = '\0';
            payload_handler(payload);
            payload_idx = 0;
        }
    }
}
