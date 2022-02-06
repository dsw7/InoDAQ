#include "logging.h"

void info(const std::string &message, const bool &verbose)
{
    if (verbose)
    {
        std::cout << message << std::endl;
    }
}

void warning(const std::string &message, const bool &verbose)
{
    if (verbose)
    {
        std::cout << message << std::endl;
    }
}

void error(const std::string &message)
{
    std::cerr << message << std::endl;
}
