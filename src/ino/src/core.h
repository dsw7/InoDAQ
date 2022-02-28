#ifndef CORE_H
#define CORE_H

#include <Arduino.h>

#define BAUD_RATE 9600

namespace Core
{
    void run_setup();
    void run_handshake();
    void run_loop();
}

#endif
