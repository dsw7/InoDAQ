from time import sleep
from json import dumps
import logging
import serial

CONNECTION_KWARGS = {
    'baudrate': 9600,
    'parity': serial.PARITY_NONE,
    'stopbits': serial.STOPBITS_ONE,
    'bytesize': serial.EIGHTBITS,
    'timeout': 5
}

MESSAGE_SYN = b'SYN\n'
MESSAGE_SYN_ACK = b'SYN-ACK\n'
MESSAGE_ACK = b'ACK\n'
MESSAGE_FIN = b'FIN\n'
MESSAGE_FIN_ACK = b'FIN-ACK\n'


class SerialConnection:

    def __init__(self, serial_port: str) -> None:

        self.serial_port_obj: serial.Serial
        self.serial_port = serial_port

    def open_connection(self) -> bool:

        logging.debug('Connecting using parameters: %s', dumps(CONNECTION_KWARGS, indent=4))

        try:
            self.serial_port_obj = serial.Serial(**CONNECTION_KWARGS, port=self.serial_port)
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

        return True

    def close_connection(self) -> None:

        if self.serial_port_obj is None:
            logging.debug('Not closing connection. Connection was never opened!')
            return

        logging.debug('Closing connection!')

        if self.serial_port_obj.is_open:
            self.serial_port_obj.close()

    def flush_input_buffer(self) -> None:

        logging.debug('Flushing input buffer')
        self.serial_port_obj.reset_input_buffer()

    def send_message(self, message: bytes) -> None:

        logging.debug('Sending message: "%s"', message)
        logging.debug('Sent %i bytes', self.serial_port_obj.write(message))

    def wait_for_message(self) -> bytes:

        logging.debug('Waiting to receive message...')
        message_received = False

        while not message_received:

            while self.serial_port_obj.in_waiting < 1:
                pass

            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default
            message_received = True

        logging.debug('Received message: %s', bytes_from_dev)
        return bytes_from_dev

    def two_way_termination(self) -> None:

        logging.debug('Sending %s message', MESSAGE_FIN)
        self.serial_port_obj.write(MESSAGE_FIN)

        logging.debug('Waiting to receive %s message', MESSAGE_FIN_ACK)
        message = self.wait_for_message()

        if message != MESSAGE_FIN_ACK:
            logging.debug('Received unknown bytes %s', message)
            return

        logging.debug('Accepted FIN-ACK message')
        self.close_connection()

    def three_way_handshake(self) -> bool:

        if not self.open_connection():
            return False

        logging.debug('Waiting to receive %s message', MESSAGE_SYN)
        message = self.wait_for_message()

        if message != MESSAGE_SYN:
            logging.debug('Received unknown bytes %s', message)
            return False

        logging.debug('Sending %s message', MESSAGE_SYN_ACK)
        self.serial_port_obj.write(MESSAGE_SYN_ACK)

        logging.debug('Waiting to receive %s message', MESSAGE_ACK)
        message = self.wait_for_message()

        if message != MESSAGE_ACK:
            logging.debug('Received unknown bytes %s', message)
            return False

        # Extra newline seems to come from somewhere
        self.flush_input_buffer()

        return True
