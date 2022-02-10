#include "commands.h"

int run_ping_command(std::string &serial_port, const bool &is_verbose)
{
    info("Running ping command", true);

    Serial connection(is_verbose);

    if (not connection.open_connection(serial_port))
    {
        return EXIT_FAILURE;
    }

    if (not connection.configure_connection())
    {
        connection.close_connection();
        return EXIT_FAILURE;
    }

    info("Built in LED should now turn on", true);
    if (connection.write_data("test\n"))
    {
        connection.read_data();
    }

    usleep(250000);

    info("Built in LED should now turn off", true);
    if (connection.write_data("test\n"))
    {
        connection.read_data();
    }

    connection.close_connection();
    return EXIT_SUCCESS;
}

int run_test_command(std::string &serial_port, const bool &is_verbose)
{
    info("Running test command", true);

    static std::vector<std::string> digital_pins = {
        "D2\n", "D3\n", "D4\n", "D5\n",
        "D6\n", "D7\n", "D8\n", "D9\n",
        "D10\n", "D11\n", "D12\n", "D13\n"
    };

    Serial connection(is_verbose);

    if (not connection.open_connection(serial_port))
    {
        return EXIT_FAILURE;
    }

    if (not connection.configure_connection())
    {
        connection.close_connection();
        return EXIT_FAILURE;
    }

    info("", true);
    info("Turning digital pins on", true);
    info("------------------------", true);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {
        usleep(50000);

        if (connection.write_data(*p))
        {
            connection.read_data();
            info("", true);
        }
    }

    info("Turning digital pins off", true);
    info("------------------------", true);

    sleep(1);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {
        usleep(50000);

        if (connection.write_data(*p))
        {
            connection.read_data();
            info("", true);
        }
    }

    connection.close_connection();
    return EXIT_SUCCESS;
}
