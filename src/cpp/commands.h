#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <unistd.h>

#include "serial.h"
#include "logging.h"
#include "control_panel.h"

void run_control_panel();
int run_ping_command(std::string &port, const bool &is_verbose);
int run_test_command(std::string &port, const bool &is_verbose);

#endif
