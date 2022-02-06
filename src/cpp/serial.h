#ifndef SERIAL_H
#define SERIAL_H

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "logging.h"

class Serial
{
    private:
        int serial_port_fd = 0;
        bool is_verbose = false;

    public:
        bool open_connection(std::string serial_port);
        bool configure_connection();
        bool write_data(const std::string &message);
        bool read_data(std::string &message);
        void close_connection();
};

#endif
