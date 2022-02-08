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
    char pin_c[9];
    snprintf(pin_c, 9, "%d", pin);

    if (pins[idx])
    {
        strcat(pin_c, ": ON");
        send_message(pin_c);
        return;
    }

    strcat(pin_c, ": OFF");
    send_message(pin_c);
}

