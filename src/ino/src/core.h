#ifndef CORE_H
#define CORE_H

#include <Arduino.h>
#include "commands.h"

#define BAUD_RATE 9600

namespace InoDAQ
{
    void setup();
    void handshake();
    void loop();
}

#endif
