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
        std::string serial_port;

    public:
        Serial(const bool &is_quiet, std::string &serial_port);

        bool setup_fd();
        bool configure_connection();
        bool configure_fd();
        bool write_data(const std::string &payload);
        bool read_data(std::string &payload);
        bool flush_buffer();
        void teardown_fd();

        bool connect();
        bool disconnect();
};

#endif
