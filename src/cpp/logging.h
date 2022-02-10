#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <iostream>
#include <math.h>
#include <unistd.h>

void info(const std::string &message, const bool &verbose);
void warning(const std::string &message, const bool &verbose);
void error(const std::string &message);
void render_progress_bar(int delay_secs);

#endif
