#include "payload_handler.h"

void payload_handler(char *payload)
{
    if (strcmp(payload, "Hi InoDAQ") == 0)
    {
        Serial.println("Hello from InoDAQ");
    }
    else
    {
        Serial.println(payload);
    }
}
