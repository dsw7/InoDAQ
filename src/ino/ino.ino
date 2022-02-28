#include "src/core.h"

int main()
{
    InoDAQ::setup();
    InoDAQ::handshake();
    InoDAQ::loop();

    return 0;
}
