#include "serial.h"

void error(std::string message)
{
    std::cerr << message << std::endl;
}

void info(std::string message)
{
    std::cout << message << std::endl;
}

bool Serial::open_connection(std::string serial_port)
{
    this->serial_port_fd = open(serial_port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (this->serial_port_fd == -1)
    {
        error(strerror(errno));
        return false;
    }

    if (fcntl(this->serial_port_fd, F_SETFL, 0) == -1)
    {
        error(strerror(errno));
        return false;
    }

    info("Successfully opened serial port: " + serial_port);
    return true;
}

bool Serial::configure_connection()
{
    struct termios serial_port_configs;

    // https://linux.die.net/man/3/cfsetispeed
	if (cfsetispeed(&serial_port_configs, B9600) == -1)
    {
        error(strerror(errno));
        return false;
    }

    // https://linux.die.net/man/3/cfsetospeed
	if (cfsetospeed(&serial_port_configs, B9600) == -1)
    {
        error(strerror(errno));
        return false;
    }

	serial_port_configs.c_cflag &= ~PARENB; // equivalent to python serial.PARITY_NONE
	serial_port_configs.c_cflag &= ~CSTOPB; // equivalent to python serial.STOPBITS_ONE
	serial_port_configs.c_cflag &= ~CSIZE;  // equivalent to python serial.EIGHTBITS
	serial_port_configs.c_cflag |= CS8;

    // https://linux.die.net/man/3/tcsetattr
	if (tcsetattr(this->serial_port_fd, TCSANOW, &serial_port_configs) == -1)
    {
        error(strerror(errno));
        return false;
    }

	return true;
}

bool Serial::write_data(std::string message)
{
    //unsigned char send_b[] = {0x44, 0x02, 0x0a};

    // https://man7.org/linux/man-pages/man2/write.2.html
    if (write(this->serial_port_fd, message.c_str(), message.size()) == -1)
    {
        error(strerror(errno));
        return false;
    }

    return true;
}

bool Serial::read_data(std::string &message)
{
    char n;
    fd_set rdfs;
    struct timeval timeout;

    timeout.tv_sec = 10; // ten second timeout
    timeout.tv_usec = 0;

	// https://man7.org/linux/man-pages/man2/select.2.html
    FD_ZERO(&rdfs);
    FD_SET(this->serial_port_fd, &rdfs);

	// https://man7.org/linux/man-pages/man2/select.2.html
	n = select(this->serial_port_fd + 1, &rdfs, NULL, NULL, &timeout);

	if (n < 0)
	{
        error(strerror(errno));
        return false;
	}
	else if (n == 0)
	{
        error("Timeout!");
        return false;
	}

	info("Bytes detected on the port!");
    return true;
}

void Serial::close_connection()
{
    if (this->serial_port_fd == 0)
    {
        return;
    }

    // https://man7.org/linux/man-pages/man2/close.2.html
    if (close(this->serial_port_fd) == -1)
    {
        error(strerror(errno));
    }
}

/*
int main(void)
{
    Serial s;
    if (not s.open_connection("/dev/ttyS2"))
    {
        return EXIT_FAILURE;
    }

    if (not s.configure_connection())
    {
        s.close_connection();
        return EXIT_FAILURE;
    }

    sleep(3);

    if (s.write_data("test\n"))
    {
        std::string result_data;
        s.read_data(result_data);
    }

    s.close_connection();
    return 0;
}
*/
