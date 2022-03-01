#include "commands.h"

namespace Protocol
{
    const char* MESSAGE_FIN = "FIN";
    const char* MESSAGE_FIN_ACK = "FIN-ACK";
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

    strcpy(buffer_output, buffer_input);
    return true;
}
