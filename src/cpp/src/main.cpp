#include <getopt.h>
#include <iostream>
#include <string>

#include "commands.h"

struct cli_options
{
    std::string serial_port = "";
    bool is_quiet = false;
    bool run_ping_command = false;
    bool run_test_command = false;
    bool run_control_panel = false;
};

void help_message(char *file)
{
    std::cerr << "Usage:\n\n";
    std::cerr << "  $ " << file;
    std::cerr << " [-h]";
    std::cerr << " [-q]";
    std::cerr << " [-c]";
    std::cerr << " [-t]";
    std::cerr << " [-i]";
    std::cerr << " [-p <serial-port>]\n\n";
    std::cerr << "Options:\n\n";
    std::cerr << "  -h, --help                      Print help information and exit\n";
    std::cerr << "  -q, --quiet                     Suppress logging when running commands\n";
    std::cerr << "  -c, --ping                      Ping device. Will blink built in LED\n";
    std::cerr << "  -t, --test                      Test communication with device by toggling digital pins\n";
    std::cerr << "  -i, --interface                 Open up curses interface for toggling digital pins\n";
    std::cerr << "  -p, --serial-port=<tcp-port>    Specify which serial port to send data to\n";
    std::cout << std::endl;
}

bool resolve_serial_port(std::string &serial_port, const bool &is_quiet)
{
    if (serial_port.size() < 1)
    {
        error("Serial port was not provided. Cannot continue!", is_quiet);
        return false;
    }

    std::string platform = std::string(CMAKE_SYSTEM_NAME);

    if (platform.compare("Linux") == 0)
    {
        return true;
    }

    if (platform.compare("CYGWIN") == 0)
    {
        info("Detected Cygwin platform. Checking if COM port was passed...", is_quiet);

        if (serial_port.size() < 4)
        {
            error("Received invalid port '" + serial_port + "'. Cannot proceed", is_quiet);
            return false;
        }

        if (serial_port.compare(0, 3, "COM") == 0)
        {
            info("Found COM port '" + serial_port + "'. Casting to device file representation", is_quiet);
            int port;

            try
            {
                port = std::stoi(serial_port.substr(3, serial_port.size()));
            }
            catch (const std::exception& exception)
            {
                error("An exception was caught with message: '" + std::string(exception.what()) + "'", is_quiet);
                error("Serial port: '" + serial_port + "' is invalid!", is_quiet);
                return false;
            }

            serial_port = "/dev/ttyS" + std::to_string(port - 1);
            info("Cast COM port to device file representation '" + serial_port + "'", is_quiet);
        }

        return true;
    }

    error("Unsupported platform '" + platform + "'. Cannot continue!", is_quiet);
    return false;
}

int main(int argc, char **argv)
{
    int option;

    cli_options options;

    while (1)
    {
        static struct option long_options[] =
        {
            {"help",        no_argument,       0, 'h'},
            {"quiet",       no_argument,       0, 'q'},
            {"ping",        no_argument,       0, 'c'},
            {"test",        no_argument,       0, 't'},
            {"interface",   no_argument,       0, 'i'},
            {"serial-port", required_argument, 0, 'p'}
        };

        int option_index = 0;

        option = getopt_long(
            argc, argv, "hqctip:", long_options, &option_index
        );

        // End of options
        if (option == -1)
        {
            break;
        }

        switch (option)
        {
            case 'h':
                help_message(argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'q':
                options.is_quiet = true;
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

    if (not resolve_serial_port(options.serial_port, options.is_quiet))
    {
        return EXIT_FAILURE;
    }

    if (options.run_ping_command)
    {
        return run_ping_command(options.serial_port, options.is_quiet);
    }

    if (options.run_test_command)
    {
        return run_test_command(options.serial_port, options.is_quiet);
    }

    if (options.run_control_panel)
    {
        // XXX temp
        //run_control_panel(options.serial_port);
        return 0;
    }

    std::cerr << "Ran no commands. Try " + std::string(argv[0]) + " --help for more information" << std::endl;
    return 0;
}
