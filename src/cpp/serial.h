#ifndef SERIAL_H
#define SERIAL_H

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>

void error(std::string message);
void info(std::string message);

class Serial
{
    private:
        int serial_port_fd = 0;

    public:
        bool open_connection(std::string serial_port);
        bool configure_connection();
        bool write_data(const std::string &message);
        bool read_data(std::string &message);
        void close_connection();
};

#endif
