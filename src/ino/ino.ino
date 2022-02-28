#include "src/core.h"

int main()
{
    Core::run_setup();
    Core::run_handshake();
    Core::run_loop();

    return 0;
}
