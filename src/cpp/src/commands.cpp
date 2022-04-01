#include "commands.h"

namespace Protocol
{
    std::string MESSAGE_TOGGLE_BUILTIN_LED = "TEST\n";
    std::vector<std::string> MESSAGE_DIGITAL_PINS = {
        "DIG2\n",
        "DIG3\n",
        "DIG4\n",
        "DIG5\n",
        "DIG6\n",
        "DIG7\n",
        "DIG8\n",
        "DIG9\n",
        "DIG10\n",
        "DIG11\n",
        "DIG12\n",
        "DIG13\n"
    };
}

void run_control_panel(std::string &serial_port)
{
    ControlPanel panel(serial_port);
    panel.loop();
}

int run_ping_command(std::string &serial_port, const bool &is_quiet)
{
    info("Running ping command", is_quiet);

    Serial connection{is_quiet, serial_port};

    if (not connection.connect())
    {
        error("Could not connect on serial port " + serial_port, is_quiet);
        return EXIT_FAILURE;
    }

    std::string payload;

    if (connection.write_data(Protocol::MESSAGE_TOGGLE_BUILTIN_LED))
    {
        usleep(RW_DELAY_USEC);
        connection.read_data(payload);
        payload.clear();
    }

    usleep(RW_DELAY_USEC);

    if (connection.write_data(Protocol::MESSAGE_TOGGLE_BUILTIN_LED))
    {
        usleep(RW_DELAY_USEC);
        connection.read_data(payload);
        payload.clear();
    }

    connection.disconnect();
    return EXIT_SUCCESS;
}

int run_test_command(std::string &serial_port, const bool &is_quiet)
{
    info("Running test command", is_quiet);

    Serial connection{is_quiet, serial_port};

    if (not connection.connect())
    {
        error("Could not connect on serial port " + serial_port, is_quiet);
        return EXIT_FAILURE;
    }

    std::string payload;

    info("", is_quiet);
    info("Turning digital pins on", is_quiet);
    info("------------------------", is_quiet);

    for (std::vector<std::string>::iterator p = Protocol::MESSAGE_DIGITAL_PINS.begin(); p != Protocol::MESSAGE_DIGITAL_PINS.end(); ++p)
    {
        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data(payload);

            payload.clear();
            info("", is_quiet);
        }
    }

    info("Turning digital pins off", is_quiet);
    info("------------------------", is_quiet);

    sleep(1);

    for (std::vector<std::string>::iterator p = Protocol::MESSAGE_DIGITAL_PINS.begin(); p != Protocol::MESSAGE_DIGITAL_PINS.end(); ++p)
    {
        if (connection.write_data(*p))
        {
            usleep(RW_DELAY_USEC);
            connection.read_data(payload);

            payload.clear();
            info("", is_quiet);
        }
    }

    connection.disconnect();
    return EXIT_SUCCESS;
}
