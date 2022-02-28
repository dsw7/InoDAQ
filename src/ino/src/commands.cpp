#include "commands.h"

namespace Protocol
{
    const char* MESSAGE_EXIT = "EXIT";
    const char* MESSAGE_TOGGLE_BUILTIN_LED = "TEST";
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
        strcpy(buffer_output, "Toggling built in LED");
        return true;
    }

    strcpy(buffer_output, buffer_input);
    return true;
}
