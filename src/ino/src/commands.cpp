#include "commands.h"

namespace Protocol
{
    const char* MESSAGE_EXIT = "EXIT";
    const char* MESSAGE_TOGGLE_BUILTIN_LED = "TEST";
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

bool parser(char* buffer_input, char* buffer_output)
{

    if (strcmp(buffer_input, Protocol::MESSAGE_EXIT) == 0)
    {
        strcpy(buffer_output, "Disconnecting from device");
        return false;
    }

    if (strcmp(buffer_input, Protocol::MESSAGE_TOGGLE_BUILTIN_LED) == 0)
    {
        toggle_builtin_led(buffer_output);
        return true;
    }

    strcpy(buffer_output, buffer_input);
    return true;
}
