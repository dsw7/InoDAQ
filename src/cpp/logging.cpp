#include "logging.h"

void info(const std::string &message, const bool &verbose)
{
    if (verbose)
    {
        std::cout << "\033[32m[ I ]\033[0m " << message << std::endl;
    }
}

void warning(const std::string &message, const bool &verbose)
{
    if (verbose)
    {
        std::cout << "\033[33m[ W ]\033[0m " << message << std::endl;
    }
}

void error(const std::string &message, const bool &verbose)
{
    if (verbose)
    {
        std::cerr << "\033[31m[ E ]\033[0m " << message << std::endl;
    }
}

void render_progress_bar(int delay_secs)
{

    int step_size = delay_secs * pow(10, 6) / 20;

    std::string progress_bar = "[                    ]";

    for (unsigned int i = 1; i < 21; i++)
    {
        if (i == 1)
        {
            progress_bar.at(i) = '>';
        }
        else
        {
            progress_bar.at(i - 1) = '-';
            progress_bar.at(i) = '>';
        }
        usleep(step_size);
        std::cout << progress_bar + "\r" << std::flush;
    }

    std::cout << std::endl;
}
