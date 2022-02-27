#include <Arduino.h>

#define BAUD_RATE 9600

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN\n";
    const char* MESSAGE_ACK = "ACK";
    const char* MESSAGE_SYN_ACK = "SYN-ACK\n";
    const char* MESSAGE_TERMINATOR = "\n";

    const unsigned int SIZE_MESSAGE_ACK = strlen(MESSAGE_ACK);
    const unsigned int SIZE_MESSAGE_BUF = 100;
    const unsigned long PERIOD_TRANSMISSION = 1e6;
}

void run_setup()
{
    init();
    Serial.begin(BAUD_RATE);
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
}

void run_loop()
{
    size_t bytes_recv;
    char buffer[Protocol::SIZE_MESSAGE_BUF] = {'\0'};

    while (true)
    {
        if (Serial.available() > 2)
        {
            bytes_recv = Serial.readBytesUntil(Protocol::MESSAGE_TERMINATOR, buffer, Protocol::SIZE_MESSAGE_BUF);
            Serial.print(buffer);
        }
    }
}

int main()
{
    run_setup();
    run_handshake();
    run_loop();

    return 0;
}
