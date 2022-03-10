#include "serial.h"

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN";
    const char* MESSAGE_SYN_ACK = "SYN-ACK";
    const char* MESSAGE_ACK = "ACK";

    const char* MESSAGE_FIN = "FIN";
    const char* MESSAGE_FIN_ACK = "FIN-ACK";
}

Serial::Serial(const bool &is_quiet, std::string &serial_port)
{
    this->is_quiet = is_quiet;
    this->serial_port = serial_port;
}

bool Serial::setup_fd()
{
    info("Attempting to open serial port: " + this->serial_port, this->is_quiet);

    this->serial_port_fd = open(this->serial_port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    // Wait for device to reset after sending DTR (Data Terminal Ready) signal
    sleep(MAX_SLEEP_WAIT_FOR_AUTO_RESET);

    if (this->serial_port_fd == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    info("Successfully opened serial port: " + this->serial_port, this->is_quiet);
    info("The assigned file descriptor is " + std::to_string(this->serial_port_fd), this->is_quiet);

    return true;
}

bool Serial::configure_fd()
{
    info("Configurating file descriptor " + std::to_string(this->serial_port_fd), this->is_quiet);

    if (fcntl(this->serial_port_fd, F_SETFL, 0) == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    return true;
}

bool Serial::configure_connection()
{
    info("Configuring I/O settings on file descriptor " + std::to_string(this->serial_port_fd), this->is_quiet);

    struct termios serial_port_configs;

    // https://linux.die.net/man/3/cfsetispeed
	if (cfsetispeed(&serial_port_configs, B9600) == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    // https://linux.die.net/man/3/cfsetospeed
	if (cfsetospeed(&serial_port_configs, B9600) == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

	serial_port_configs.c_cflag &= ~PARENB; // equivalent to python serial.PARITY_NONE
	serial_port_configs.c_cflag &= ~CSTOPB; // equivalent to python serial.STOPBITS_ONE
	serial_port_configs.c_cflag &= ~CSIZE;  // equivalent to python serial.EIGHTBITS
	serial_port_configs.c_cflag |= CS8;

    // https://linux.die.net/man/3/tcsetattr
	if (tcsetattr(this->serial_port_fd, TCSANOW, &serial_port_configs) == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    info("Successfully set all configurations on file descriptor", this->is_quiet);
	return true;
}

bool Serial::write_data(const std::string &payload)
{
    if (payload.size() > 0)
    {
        std::string payload_no_newline = payload.substr(0, payload.size() - 1);
        info("Writing message '" + payload_no_newline + "' to file descriptor", this->is_quiet);
    }
    else
    {
        error("Message is empty. Doing nothing", this->is_quiet);
        return false;
    }

    // https://man7.org/linux/man-pages/man2/write.2.html
    int num_bytes_written = write(this->serial_port_fd, payload.c_str(), payload.size());

    if (num_bytes_written == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    info("Successfully wrote out " + std::to_string(num_bytes_written) + " bytes to file descriptor", this->is_quiet);
    return true;
}

bool Serial::read_data(std::string &payload)
{
    info("Reading data from file descriptor", this->is_quiet);

    fd_set fds_ready_for_reading;
    struct timeval timeout;

    timeout.tv_sec = TIMEOUT_SELECT_SECS;
    timeout.tv_usec = 0;

	// https://man7.org/linux/man-pages/man2/select.2.html
    FD_ZERO(&fds_ready_for_reading);
    FD_SET(this->serial_port_fd, &fds_ready_for_reading);

	// https://man7.org/linux/man-pages/man2/select.2.html
    // specifically see the part about how we must have the largest file descriptor + 1
	char n = select(this->serial_port_fd + 1, &fds_ready_for_reading, NULL, NULL, &timeout);

	if (n < 0)
	{
        error(strerror(errno), this->is_quiet);
        return false;
	}
	else if (n == 0)
	{
        error("Timeout!", this->is_quiet);
        return false;
	}

    const unsigned int max_size_payload = 25;
    char buffer[max_size_payload];
    unsigned int index = 0;

    while (index < max_size_payload)
    {
        if (read(this->serial_port_fd, &buffer[index], 1) == -1)
        {
            error(strerror(errno), this->is_quiet);
            break;
        }

        if (buffer[index] == '\n')
        {
            break;
        }

        index++;
    }

    buffer[index] = '\0';
    payload = std::string(buffer);

	info("Number of bytes read from file descriptor: " + std::to_string(payload.size()), this->is_quiet);
    info("Received message from device: '" + payload + "'", this->is_quiet);

    return true;
}

void Serial::teardown_fd()
{
    info("Closing file descriptor " + std::to_string(this->serial_port_fd), this->is_quiet);

    if (this->serial_port_fd == 0)
    {
        return;
    }

    // https://man7.org/linux/man-pages/man2/close.2.html
    if (close(this->serial_port_fd) == -1)
    {
        error(strerror(errno), this->is_quiet);
    }
}

bool Serial::connect()
{
    if (not this->setup_fd())
    {
        return false;
    }

    if (not this->configure_fd())
    {
        this->teardown_fd();
        return false;
    }

    if (not this->configure_connection())
    {
        this->teardown_fd();
        return false;
    }

    std::string syn;
    this->read_data(syn);

    return true;
}

bool Serial::disconnect()
{
    this->teardown_fd();
    return true;
}
