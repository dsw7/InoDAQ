#ifndef SERIAL_H
#define SERIAL_H

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#include "logging.h"

class Serial
{
    private:
        int serial_port_fd = 0;
        bool is_quiet;

    public:
        Serial(const bool &is_quiet);

        bool open_connection(std::string serial_port);
        bool configure_connection();
        bool configure_fildes();
        bool write_data(const std::string &message);
        bool read_data();
        void close_connection();

        bool connect(std::string &serial_port);
        bool disconnect();
};

#endif
