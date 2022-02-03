#include "digital_pin_handler.h"

void set_digital_pin_status(int &pin)
{
    static bool state = false;
    static int idx = pin - 2;

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
}
