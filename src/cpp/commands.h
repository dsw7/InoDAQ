#ifndef COMMANDS_H
#define COMMANDS_H

#include "serial.h"
#include "logging.h"

int run_ping_command(std::string &port, const bool &is_verbose);

#endif
