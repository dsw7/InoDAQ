#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <iostream>
#include <math.h>
#include <unistd.h>

void info(const std::string &message, const bool &is_quiet);
void warning(const std::string &message, const bool &is_quiet);
void error(const std::string &message, const bool &is_quiet);
void render_progress_bar(int delay_secs);

#endif
