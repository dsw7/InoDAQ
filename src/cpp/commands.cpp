#include "commands.h"

bool run_test_command(std::string &serial_port)
{
    Serial connection;

    if (not connection.open_connection(serial_port))
    {
        return false;
    }

    if (not connection.configure_connection())
    {
        s.close_connection();
        return false;
    }

    sleep(3);

    if (connection.write_data("test\n"))
    {
        std::string result_data;
        s.read_data(result_data);
    }

    connection.close_connection();
    return true;
}
