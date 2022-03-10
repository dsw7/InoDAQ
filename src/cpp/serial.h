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

        bool setup_fd(std::string serial_port);
        bool configure_connection();
        bool configure_fildes();
        bool write_data(const std::string &message);
        bool read_data();
        void teardown_fd();

        bool connect(std::string &serial_port);
        bool disconnect();
};

#endif
