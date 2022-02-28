#include "commands.h"

bool parser(char* buffer_input, char* buffer_output)
{

    if (strcmp(buffer_input, "exit") == 0)
    {
        strcpy(buffer_output, "Disconnecting from device");
        return false;
    }

    if (strcmp(buffer_input, "test") == 0)
    {
        strcpy(buffer_output, "Toggling built in LED");
        return true;
    }

    strcpy(buffer_output, buffer_input);
    return true;
}
