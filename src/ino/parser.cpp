#include "parser.h"

bool digital_pin_lexer(char *payload, int &pin)
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

void help_menu()
{
    char menu[] =
    "> InoDAQ, 2022\n"
    "> System accepts the following messages:\n"
    "> 1. 'help'          <- Get this help message\n"
    "> 2. 'test'          <- Toggle built in LED (for handshaking purposes)\n"
    "> 3. 'D[2-9]|1[1-3]' <- Toggle digital pins 2-13 on and off";

    Serial.println(menu);
}

void payload_parser(char *payload)
{
    if (strcmp(payload, "help") == 0)
    {
        help_menu();
        return;
    }

    if (strcmp(payload, "test") == 0)
    {
        toggle_builtin_led();
        return;
    }

    int pin;

    if (digital_pin_lexer(payload, pin))
    {
        set_digital_pin_status(pin);
        return;
    }

    send_message(payload);
}
