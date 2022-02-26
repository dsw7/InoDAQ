#include "led_builtin.h"

void toggle_builtin_led()
{
    static bool status = false;

    status = !status;
    digitalWrite(LED_BUILTIN, status);

    if (status)
    {
        send_message("Built in LED is ON");
        return;
    }

    send_message("Built in LED is OFF");
}

