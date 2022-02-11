#include "commands.h"

void run_control_panel()
{
    ControlPanel panel;
    panel.loop();
}

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
        usleep(RW_DELAY_USEC);
        connection.read_data();
    }

    usleep(RW_DELAY_USEC);

    info("Built in LED should now turn off", true);
    if (connection.write_data("test\n"))
    {
        usleep(RW_DELAY_USEC);
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

    info("", is_verbose);
    info("Turning digital pins on", true);
    info("------------------------", is_verbose);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {

        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data();
            info("", is_verbose);
        }
    }

    info("Turning digital pins off", true);
    info("------------------------", is_verbose);

    sleep(1);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {

        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data();
            info("", is_verbose);
        }
    }

    connection.close_connection();
    return EXIT_SUCCESS;
}
