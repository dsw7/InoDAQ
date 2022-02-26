#include <Arduino.h>

#define BAUD_RATE 9600

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN\n";
    const char* MESSAGE_ACK = "ACK";

    const unsigned int SIZE_MESSAGE_ACK = strlen(MESSAGE_ACK);
    const unsigned long PERIOD_TRANSMISSION = 1e6;
}

void establish_connection()
{
    size_t bytes_recv;
    bool ack_received = false;

    unsigned long start_time = micros();
    char buffer[sizeof(Protocol::MESSAGE_ACK) + 1]; // make extra room for null terminator

    while (ack_received == false)
    {
        if ((micros() - start_time) >= Protocol::PERIOD_TRANSMISSION)
        {
            Serial.print(Protocol::MESSAGE_SYN);
            start_time = micros();
        }

        if (Serial.available() > 2)
        {
            bytes_recv = Serial.readBytesUntil('\n', buffer, Protocol::SIZE_MESSAGE_ACK);

            if (bytes_recv == Protocol::SIZE_MESSAGE_ACK)
            {
                if (strcmp(buffer, Protocol::MESSAGE_ACK) == 0)
                {
                    ack_received = true;
                }
            }
        }
    }
}

int main()
{
    init();

    Serial.begin(BAUD_RATE);
    establish_connection();

    while(1)
    {

    }

    return 0;
}
