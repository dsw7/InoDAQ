#include <getopt.h>
#include <iostream>
#include <string>

void help_message(char *file)
{
    std::cerr << "Usage:\n\n";
    std::cerr << "  $ " << file;
    std::cerr << " [-h]";
    std::cerr << " [-p <serial-port>]\n\n";
    std::cerr << "Options:\n\n";
    std::cerr << "  -h, --help                      Print help information and exit\n";
    std::cerr << "  -p, --serial-port=<tcp-port>    Specify which serial port to send data to\n";
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    int c;

    std::string serial_port = "/dev/ttyS2";

    while (1)
    {
        static struct option long_options[] =
        {
            {"help",        no_argument,       0, 'h'},
            {"serial-port", required_argument, 0, 'p'}
        };

        // What's the point of this?
        int option_index = 0;

        c = getopt_long(
            argc, argv, "hp:", long_options, &option_index
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
            case 'p':
                serial_port = optarg;
                break;
            default:
                help_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    };

    std::cout << "Will send data to serial port: " << serial_port << std::endl;

    return 0;
}
