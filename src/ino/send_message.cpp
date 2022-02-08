#include "send_message.h"

void send_message(char *message)
{
    unsigned int size_padding = MAX_SIZE_PAYLOAD - strlen(message);

    char padding[size_padding + 1];

    for (unsigned int i = 0; i < size_padding; i++)
    {
        padding[i] = 'x';
    }

    padding[size_padding] = '\0';

    char payload[MAX_SIZE_PAYLOAD];

    strcpy(payload, message);
    strcat(payload, padding);

    // Use Serial.print() because Serial.println() adds two additional bytes, CRLF, to the outgoing
    // payload which is a problem when reading the message on the receiver's end
    Serial.print(payload);
}

