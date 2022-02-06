#ifndef LOGGING_H
#define LOGGING_H

#include <string.h>
#include <iostream>

void info(const std::string &message, const bool &verbose);
void warning(const std::string &message, const bool &verbose);
void error(const std::string &message);

#endif
