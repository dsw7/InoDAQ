#include "commands.h"

namespace Protocol
{
    const char* MESSAGE_FIN = "FIN";
    const char* MESSAGE_FIN_ACK = "FIN-ACK";
    const char* MESSAGE_TOGGLE_BUILTIN_LED = "TEST";
    const char* MESSAGE_PREFIX_TOGGLE_DIG_PIN = "DIG";
}

void toggle_builtin_led(char *buffer)
{
    static bool status = false;

    status = !status;
    digitalWrite(LED_BUILTIN, status);

    if (status)
    {
        strcpy(buffer, "Built in LED is ON");
        return;
    }

    strcpy(buffer, "Built in LED is OFF");
}

void toggle_digital_pin(char* buffer_input, char* buffer_output)
{
    if (strcmp(buffer_input, Protocol::MESSAGE_PREFIX_TOGGLE_DIG_PIN) == 0)
    {
        strcpy(buffer_output, Protocol::MESSAGE_PREFIX_TOGGLE_DIG_PIN);
        return;
    }

    char substr_pin[3];

    strncpy(substr_pin, &buffer_input[3], 2);
    substr_pin[2] = '\0';

    int pin = atoi(substr_pin);

    if ((pin < 2) or (pin > 13))
    {
        strcpy(buffer_output, buffer_input);
        return;
    }

    // TODO continue here
    strcpy(buffer_output, "ABC");
}

bool parser(char* buffer_input, char* buffer_output)
{

    if (strcmp(buffer_input, Protocol::MESSAGE_FIN) == 0)
    {
        strcpy(buffer_output, Protocol::MESSAGE_FIN_ACK);
        return false;
    }

    if (strcmp(buffer_input, Protocol::MESSAGE_TOGGLE_BUILTIN_LED) == 0)
    {
        toggle_builtin_led(buffer_output);
        return true;
    }

    if (strncmp(buffer_input, Protocol::MESSAGE_PREFIX_TOGGLE_DIG_PIN, sizeof(Protocol::MESSAGE_PREFIX_TOGGLE_DIG_PIN)) == 0)
    {
        toggle_digital_pin(buffer_input, buffer_output);
        return true;
    }

    strcpy(buffer_output, buffer_input);
    return true;
}
