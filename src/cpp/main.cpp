#include <getopt.h>
#include <iostream>
#include <string>

#include "commands.h"
#include "logging.h"

struct cli_options
{
    std::string serial_port = "/dev/ttyS2";
    bool enable_logging = false;
    bool run_ping_command = false;
    bool run_test_command = false;
    bool run_control_panel = false;
};

void help_message(char *file)
{
    std::cerr << "Usage:\n\n";
    std::cerr << "  $ " << file;
    std::cerr << " [-h]";
    std::cerr << " [-v]";
    std::cerr << " [-c]";
    std::cerr << " [-t]";
    std::cerr << " [-i]";
    std::cerr << " [-p <serial-port>]\n\n";
    std::cerr << "Options:\n\n";
    std::cerr << "  -h, --help                      Print help information and exit\n";
    std::cerr << "  -v, --verbose                   Enable logging when running commands\n";
    std::cerr << "  -c, --ping                      Ping device. Will blink built in LED\n";
    std::cerr << "  -t, --test                      Test communication with device by toggling digital pins\n";
    std::cerr << "  -i, --interface                 Open up curses interface for toggling digital pins\n";
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
            {"verbose",     no_argument,       0, 'v'},
            {"ping",        no_argument,       0, 'c'},
            {"test",        no_argument,       0, 't'},
            {"interface",   no_argument,       0, 'i'},
            {"serial-port", required_argument, 0, 'p'}
        };

        // What's the point of this?
        int option_index = 0;

        c = getopt_long(
            argc, argv, "hvctip:", long_options, &option_index
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
            case 'v':
                options.enable_logging = true;
                break;
            case 'c':
                options.run_ping_command = true;
                break;
            case 't':
                options.run_test_command = true;
                break;
            case 'i':
                options.run_control_panel = true;
                break;
            case 'p':
                options.serial_port = optarg;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };

    if (options.run_ping_command)
    {
        return run_ping_command(options.serial_port, options.enable_logging);
    }

    if (options.run_test_command)
    {
        return run_test_command(options.serial_port, options.enable_logging);
    }

    if (options.run_control_panel)
    {
        run_control_panel(options.serial_port);
        return 0;
    }

    warning("Ran no commands. Try " + std::string(argv[0]) + " --help for more information", true);
    return 0;
}
