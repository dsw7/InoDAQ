#include "led_builtin.h"

void toggle_builtin_led()
{
    static bool status = false;

    status = !status;
    digitalWrite(LED_BUILTIN, status);

    if (status)
    {
        Serial.println("Built in LED is ON");
        return;
    }

    Serial.println("Built in LED is OFF");
}
