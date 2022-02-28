#include "core.h"

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN\n";
    const char* MESSAGE_SYN_ACK = "SYN-ACK";
    const char* MESSAGE_ACK = "ACK\n";
    const char* MESSAGE_TERMINATOR = '\n';

    const unsigned int SIZE_MESSAGE_SYN_ACK = strlen(MESSAGE_SYN_ACK);
    const unsigned int SIZE_MESSAGE_BUF = 40;
    const unsigned long PERIOD_TRANSMISSION = 1e6;
}

namespace InoDAQ
{

void setup()
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

void handshake()
{
    size_t bytes_recv;
    bool syn_ack_received = false;

    unsigned long start_time = micros();
    char buffer[Protocol::SIZE_MESSAGE_SYN_ACK + 1] = {'\0'}; // make extra room for null terminator

    while (syn_ack_received == false)
    {
        if ((micros() - start_time) >= Protocol::PERIOD_TRANSMISSION)
        {
            Serial.print(Protocol::MESSAGE_SYN);
            Serial.flush();
            start_time = micros();
        }

        if (Serial.available() > 2)
        {
            bytes_recv = Serial.readBytesUntil(Protocol::MESSAGE_TERMINATOR, buffer, Protocol::SIZE_MESSAGE_SYN_ACK);

            if (bytes_recv == Protocol::SIZE_MESSAGE_SYN_ACK)
            {
                if (strcmp(buffer, Protocol::MESSAGE_SYN_ACK) == 0)
                {
                    syn_ack_received = true;
                }
            }
        }
    }

    Serial.print(Protocol::MESSAGE_ACK);
    Serial.flush();
}

void loop()
{
    bool run_loop = true;

    while (true)
    {
        if (Serial.available() > 0)
        {
            char buffer_input[Protocol::SIZE_MESSAGE_BUF] = {'\0'};
            char buffer_output[Protocol::SIZE_MESSAGE_BUF] = {'\0'};

            Serial.readBytesUntil(Protocol::MESSAGE_TERMINATOR, buffer_input, Protocol::SIZE_MESSAGE_BUF);
            run_loop = parser(buffer_input, buffer_output);

            // readBytesUntil strips out MESSAGE_TERMINATOR - need to add it back
            buffer_output[strlen(buffer_output)] = Protocol::MESSAGE_TERMINATOR;
            Serial.print(buffer_output);
            Serial.flush();

            if (not run_loop)
            {
                break;
            }
        }
    }
}

} // namespace InoDAQ
