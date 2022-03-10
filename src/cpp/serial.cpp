#include "serial.h"

namespace Protocol
{
    const char* MESSAGE_SYN = "SYN";
    const char* MESSAGE_SYN_ACK = "SYN-ACK";
    const char* MESSAGE_ACK = "ACK";

    const char* MESSAGE_FIN = "FIN";
    const char* MESSAGE_FIN_ACK = "FIN-ACK";
}

Serial::Serial(const bool &is_quiet)
{
    this->is_quiet = is_quiet;
}

bool Serial::open_connection(std::string serial_port)
{
    info("Attempting to open serial port: " + serial_port, this->is_quiet);

    this->serial_port_fd = open(serial_port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    // Wait for device to reset after sending DTR (Data Terminal Ready) signal
    sleep(MAX_SLEEP_WAIT_FOR_AUTO_RESET);

    if (this->serial_port_fd == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    info("Successfully opened serial port: " + serial_port, this->is_quiet);
    info("File descriptor: " + std::to_string(this->serial_port_fd), this->is_quiet);

    return true;
}

bool Serial::configure_fildes()
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
    info("Configuring connection on serial port", this->is_quiet);

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

    info("Successfully set all configurations on serial port", this->is_quiet);
	return true;
}

bool Serial::write_data(const std::string &message)
{
    if (message.size() > 0)
    {
        std::string message_no_newline = message.substr(0, message.size() - 1);
        info("Writing message '" + message_no_newline + "' to device", this->is_quiet);
    }
    else
    {
        error("Message is empty. Doing nothing", this->is_quiet);
        return false;
    }

    // https://man7.org/linux/man-pages/man2/write.2.html
    int num_bytes_written = write(this->serial_port_fd, message.c_str(), message.size());

    if (num_bytes_written == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

    info("Successfully wrote out " + std::to_string(num_bytes_written) + " bytes to serial port", this->is_quiet);
    return true;
}

bool Serial::read_data()
{
    info("Reading data from serial port", this->is_quiet);

    char n;
    fd_set fildes_ready_for_reading;
    struct timeval timeout;

    timeout.tv_sec = TIMEOUT_SELECT_SECS;
    timeout.tv_usec = 0;

	// https://man7.org/linux/man-pages/man2/select.2.html
    FD_ZERO(&fildes_ready_for_reading);
    FD_SET(this->serial_port_fd, &fildes_ready_for_reading);

	// https://man7.org/linux/man-pages/man2/select.2.html
    // specifically see the part about how we must have the largest file descriptor + 1
	n = select(this->serial_port_fd + 1, &fildes_ready_for_reading, NULL, NULL, &timeout);

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

    char message[MAX_SIZE_PAYLOAD];

    // The read function will wait until all MAX_SIZE_PAYLOAD number of bytes arrive. If device only
    // sends 10 bytes then read will hang until a timeout, unless fnctl sets a O_NONBLOCK on the file descriptor.
    // The solution is simple -> we just need to pad the messages with zeros to a fixed length
    // or read byte-by-byte until we receive some sort of terminator
    int num_bytes_read = read(this->serial_port_fd, &message, MAX_SIZE_PAYLOAD);

    if (num_bytes_read == -1)
    {
        error(strerror(errno), this->is_quiet);
        return false;
    }

	info("Number of bytes read from serial port: " + std::to_string(num_bytes_read), this->is_quiet);

    message[24] = '\0'; // Final byte must be NULL terminated otherwise std::string doesn't know where to stop
    info("Received message from device: '" + std::string(message) + "'", this->is_quiet);

    return true;
}

void Serial::close_connection()
{
    info("Closing connection to serial port", this->is_quiet);

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

bool Serial::connect(std::string &serial_port)
{
    if (not this->open_connection(serial_port))
    {
        return false;
    }

    if (not this->configure_fildes())
    {
        this->close_connection();
        return false;
    }

    if (not this->configure_connection())
    {
        this->close_connection();
        return false;
    }

    return true;
}

bool Serial::disconnect()
{
    return true;
}
