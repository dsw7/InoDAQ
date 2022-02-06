#include <getopt.h>
#include <iostream>
#include <string>

#include "commands.h"
#include "logging.h"

struct cli_options
{
    std::string serial_port = "/dev/ttyS2";
    bool run_test_command = false;
};

void help_message(char *file)
{
    std::cerr << "Usage:\n\n";
    std::cerr << "  $ " << file;
    std::cerr << " [-h]";
    std::cerr << " [-t]";
    std::cerr << " [-p <serial-port>]\n\n";
    std::cerr << "Options:\n\n";
    std::cerr << "  -h, --help                      Print help information and exit\n";
    std::cerr << "  -t, --test                      Run a connection test. Will blink built in LED\n";
    std::cerr << "  -p, --serial-port=<tcp-port>    Specify which serial port to send data to\n";
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    int c;

    cli_options options;

    while (1)
    {
        static struct option long_options[] =
        {
            {"help",        no_argument,       0, 'h'},
            {"test",        no_argument,       0, 't'},
            {"serial-port", required_argument, 0, 'p'}
        };

        // What's the point of this?
        int option_index = 0;

        c = getopt_long(
            argc, argv, "htp:", long_options, &option_index
        );

        // End of options
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 'h':
                help_message(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 't':
                options.run_test_command = true;
                break;
            case 'p':
                options.serial_port = optarg;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };

    if (options.run_test_command)
    {
        return run_test_command(options.serial_port, true);
    }

    warning("Ran no commands. Try " + std::string(argv[0]) + " --help for more information", true);
    return 0;
}
