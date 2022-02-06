#include "commands.h"

int run_test_command(std::string &serial_port)
{
    Serial connection;

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

    if (connection.write_data("test\n"))
    {
        std::string result_data;
        connection.read_data(result_data);
    }

    connection.close_connection();

    return EXIT_SUCCESS;
}
