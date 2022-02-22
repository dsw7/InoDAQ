#include "logging.h"

void info(const std::string &message, const bool &is_quiet)
{
    if (is_quiet)
    {
        return;
    }
    std::cout << "\033[32m[ I ]\033[0m " << message << std::endl;
}

void warning(const std::string &message, const bool &is_quiet)
{
    if (is_quiet)
    {
        return;
    }
    std::cout << "\033[33m[ W ]\033[0m " << message << std::endl;
}

void error(const std::string &message, const bool &is_quiet)
{
    if (is_quiet)
    {
        return;
    }
    std::cerr << "\033[31m[ E ]\033[0m " << message << std::endl;
}
