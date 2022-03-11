#include "commands.h"

/*
void run_control_panel(std::string &serial_port)
{
    ControlPanel panel(serial_port);
    panel.loop();
}
*/

int run_ping_command(std::string &serial_port, const bool &is_quiet)
{
    info("Running ping command", is_quiet);

    Serial connection{is_quiet, serial_port};

    if (not connection.connect())
    {
        return EXIT_FAILURE;
    }

    usleep(RW_DELAY_USEC);

    connection.disconnect();
    return EXIT_SUCCESS;
}

/*
int run_test_command(std::string &serial_port, const bool &is_quiet)
{
    info("Running test command", is_quiet);

    static std::vector<std::string> digital_pins = {
        "D2\n", "D3\n", "D4\n", "D5\n",
        "D6\n", "D7\n", "D8\n", "D9\n",
        "D10\n", "D11\n", "D12\n", "D13\n"
    };

    Serial connection(is_quiet);

    if (not connection.open_connection(serial_port))
    {
        return EXIT_FAILURE;
    }

    if (not connection.configure_connection())
    {
        connection.close_connection();
        return EXIT_FAILURE;
    }

    info("", is_quiet);
    info("Turning digital pins on", is_quiet);
    info("------------------------", is_quiet);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {

        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data();
            info("", is_quiet);
        }
    }

    info("Turning digital pins off", is_quiet);
    info("------------------------", is_quiet);

    sleep(1);

    for (std::vector<std::string>::iterator p = digital_pins.begin(); p != digital_pins.end(); ++p)
    {

        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data();
            info("", is_quiet);
        }
    }

    connection.close_connection();
    return EXIT_SUCCESS;
}
*/
