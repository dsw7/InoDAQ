#include "payload_handler.h"

bool lexer(char *payload, int &pin)
{
    pin = -1;

    if (strncmp(payload, "D", 1) != 0)
    {
        return false;
    }

    // Allocate a 3 byte array
    char substr_pin[3];

    // Get the address of payload at the 2nd char, then
    // grab the 2rd and 3rd chars (the 2 arg)
    strncpy(substr_pin, &payload[1], 2);

    // NULL terminate the C-string to ensure that we don't
    // get undefined behaviour from atoi
    substr_pin[2] = '\0';

    /*
     * It's okay if we have:
     * char s[3];
     * s[0] = '1'
     * s[2] = '\0'
     *
     * With s[1] unset. The atoi() function will handle this case.
     */
    pin = atoi(substr_pin);

    if ((pin < 2) or (pin > 13))
    {
        pin = -1;
        return false;
    }

    return true;
}

void payload_handler(char *payload)
{
    if (strcmp(payload, "Hi InoDAQ") == 0)
    {
        Serial.println("Hello from InoDAQ");
    }
    else
    {
        int pin;
        if (lexer(payload, pin))
        {
            Serial.println(pin); // set pin on and off here
        }
        else
        {
            Serial.println(payload);
        }
    }
}
