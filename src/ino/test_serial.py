from time import sleep
from platform import uname
from json import dumps
import logging
import serial

if 'CYGWIN' in uname().system:
    SERIAL_PORT = '/dev/ttyS2'
else:
    SERIAL_PORT = '/dev/ttyUSB0'

CONNECTION_KWARGS = {
    'port': SERIAL_PORT,
    'baudrate': 9600,
    'parity': serial.PARITY_NONE,
    'stopbits': serial.STOPBITS_ONE,
    'bytesize': serial.EIGHTBITS,
    'timeout': 5
}

MESSAGE_SYN = b'SYN\n'
MESSAGE_ACK = b'ACK\n'


class Serial:

    def __init__(self) -> None:

        self.serial_port_obj = None
        self.run = True

    def open_connection(self) -> bool:

        logging.debug('Connecting using parameters: %s', dumps(CONNECTION_KWARGS, indent=4))

        try:
            self.serial_port_obj = serial.Serial(**CONNECTION_KWARGS)
        except serial.serialutil.SerialException:
            logging.exception('An exception occurred when connecting:')
            return False

        if not self.serial_port_obj.is_open:
            logging.error('Could not connect to %s', self.serial_port_obj.name)
            return False

        # Opening a connection will send a DTR (Data Terminal Ready) signal to device, which will
        # force the device to reset. Give device 2 seconds to reset

        logging.debug('DTR (Data Terminal Ready) was sent. Waiting for device to reset')
        sleep(2)

        self.flush_input_buffer()

        return True, None

    def flush_input_buffer(self) -> None:

        logging.debug('Flushing input buffer')
        self.serial_port_obj.reset_input_buffer()

    def flush_output_buffer(self) -> None:

        logging.debug('Flushing output buffer')
        self.serial_port_obj.reset_output_buffer()

    def close_connection(self) -> None:

        if self.serial_port_obj is None:
            logging.debug('Not closing connection. Connection was never opened!')
            return

        logging.debug('Closing connection!')

        if self.serial_port_obj.is_open:
            self.serial_port_obj.close()

    def wait_for_syn(self) -> None:

        logging.debug('Waiting to receive SYN message')
        syn_received = False

        while (not syn_received) and (self.run):

            while self.serial_port_obj.in_waiting < len(MESSAGE_ACK):
                pass

            logging.debug('Ready to read data')
            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default

            logging.debug('Received message: %s', bytes_from_dev)

            if bytes_from_dev == MESSAGE_SYN:
                logging.debug('Accepted SYN message')
                syn_received = True
            else:
                logging.debug('Received unknown bytes %s', bytes_from_dev)

    def send_ack(self) -> None:

        logging.debug('Sending ACK message')
        self.serial_port_obj.write(MESSAGE_ACK)

    def send_message(self, message: bytes) -> None:

        logging.debug('Sending message: "%s"', message)
        self.serial_port_obj.write(message)

    def receive_message(self) -> bytes:
        message = self.serial_port_obj.read_until()
        logging.debug('Received message: "%s"', message)

        return message


class TestSerial:

    def setup_method(self) -> None:

        self.serial_obj = Serial()
        self.serial_obj.open_connection()
        self.serial_obj.wait_for_syn()
        self.serial_obj.send_ack()

        # Note that SYN will still exist in input buffer after sending ACK
        self.serial_obj.flush_input_buffer()
        self.serial_obj.flush_output_buffer()

    def teardown_method(self) -> None:
        self.serial_obj.close_connection()

    def test_unknown_message(self) -> None:
        self.serial_obj.send_message(b'foobar')
        assert self.serial_obj.receive_message() == b'foobar'
