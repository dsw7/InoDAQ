#include "digital_pin_handler.h"

void set_digital_pin_status(int &pin)
{
    int idx = pin - 2;

    static bool pins[12] = {
        false, // pin 2
        false, // pin 3
        false, // ...
        false,
        false,
        false,
        false,
        false,
        false,
        false, // ...
        false, // pin 12
        false  // pin 13
    };

    pins[idx] = !pins[idx];
    digitalWrite(pin, pins[idx]);

    // Cast pin (int) to char array
    // 9 bytes to handle strcat of "13" + ": OFF"
    // 1 3 \0 : _ 0 F F \0
    // 1 2 3  4 5 6 7 8 9
    char message[10]; // add space for one extra byte just in case

    sprintf(message, "%d", pin);

    if (pins[idx])
    {
        strcat(message, ": ON");
        send_message(message);
        return;
    }

    strcat(message, ": OFF");
    send_message(message);
}

