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

    sleep(3);

    info("Built in LED should now turn on", true);
    if (connection.write_data("test\n"))
    {
        connection.read_data();
    }

    sleep(1);

    info("Built in LED should now turn off", true);
    if (connection.write_data("test\n"))
    {
        connection.read_data();
    }

    connection.close_connection();
    return EXIT_SUCCESS;
}
