#include "core.h"

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN\n";
    const char* MESSAGE_ACK = "ACK";
    const char* MESSAGE_SYN_ACK = "SYN-ACK\n";
    const char* MESSAGE_TERMINATOR = '\n';

    const unsigned int SIZE_MESSAGE_ACK = strlen(MESSAGE_ACK);
    const unsigned int SIZE_MESSAGE_BUF = 40;
    const unsigned long PERIOD_TRANSMISSION = 1e6;
}

namespace Core
{

void run_setup()
{
    init();
    Serial.begin(BAUD_RATE);

    for (unsigned int pin = 2; pin <= 13; pin++)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    pinMode(LED_BUILTIN, OUTPUT);
}

void run_handshake()
{
    size_t bytes_recv;
    bool ack_received = false;

    unsigned long start_time = micros();
    char buffer[Protocol::SIZE_MESSAGE_ACK + 1] = {'\0'}; // make extra room for null terminator

    while (ack_received == false)
    {
        if ((micros() - start_time) >= Protocol::PERIOD_TRANSMISSION)
        {
            Serial.print(Protocol::MESSAGE_SYN);
            Serial.flush();
            start_time = micros();
        }

        if (Serial.available() > 2)
        {
            bytes_recv = Serial.readBytesUntil(Protocol::MESSAGE_TERMINATOR, buffer, Protocol::SIZE_MESSAGE_ACK);

            if (bytes_recv == Protocol::SIZE_MESSAGE_ACK)
            {
                if (strcmp(buffer, Protocol::MESSAGE_ACK) == 0)
                {
                    ack_received = true;
                }
            }
        }
    }

    Serial.print(Protocol::MESSAGE_SYN_ACK);
    Serial.flush();
}

void run_loop()
{
    size_t bytes_recv;

    while (true)
    {
        if (Serial.available() > 0)
        {
            char buffer[Protocol::SIZE_MESSAGE_BUF] = {'\0'};
            bytes_recv = Serial.readBytesUntil(Protocol::MESSAGE_TERMINATOR, buffer, Protocol::SIZE_MESSAGE_BUF);

            // readBytesUntil strips out MESSAGE_TERMINATOR - need to add it back
            buffer[bytes_recv] = '\n';
            Serial.print(buffer);
            Serial.flush();
        }
    }
}

} // namespace Core
