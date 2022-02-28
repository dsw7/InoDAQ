import logging
from time import sleep
from json import dumps
from uuid import uuid4
from random import randint
from typing import List
from platform import uname
import serial
from pytest import mark

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
MESSAGE_SYN_ACK = b'SYN-ACK\n'


class Serial:

    def __init__(self) -> None:

        self.serial_port_obj = None

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
        message_received = False

        while not message_received:

            while self.serial_port_obj.in_waiting < len(MESSAGE_ACK):
                pass

            logging.debug('Ready to read data')
            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default

            logging.debug('Received message: %s', bytes_from_dev)

            if bytes_from_dev == MESSAGE_SYN:
                logging.debug('Accepted SYN message')
                message_received = True
            else:
                logging.debug('Received unknown bytes %s', bytes_from_dev)

    def send_ack(self) -> None:

        logging.debug('Sending ACK message')
        self.serial_port_obj.write(MESSAGE_ACK)

    def wait_for_syn_ack(self) -> None:

        logging.debug('Waiting to receive SYN-ACK message')
        message_received = False

        while not message_received:

            while self.serial_port_obj.in_waiting < len(MESSAGE_SYN_ACK):
                pass

            logging.debug('Ready to read data')
            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default

            logging.debug('Received message: %s', bytes_from_dev)

            if bytes_from_dev == MESSAGE_SYN_ACK:
                logging.debug('Accepted SYN-ACK message')
                message_received = True
            else:
                logging.debug('Received unknown bytes %s', bytes_from_dev)

        # Extra newline seems to come from somewhere
        self.flush_input_buffer()

    def send_message(self, message: bytes) -> None:

        logging.debug('Sending message: "%s"', message)
        logging.debug('Sent %i bytes', self.serial_port_obj.write(message))

    def receive_message(self) -> bytes:
        message = self.serial_port_obj.read_until()
        logging.debug('Received message: "%s"', message)

        return message


def generate_random_bytes(size: int) -> List[bytes]:

    random_bytes = []
    for _ in range(size):

        byte_array = str(uuid4()).encode()
        random_bytes.append(byte_array[:randint(1, 36)] + b'\n')

    return random_bytes


class TestSerial:

    def setup_class(self) -> None:

        self.serial_obj = Serial()
        self.serial_obj.open_connection()
        self.serial_obj.wait_for_syn()
        self.serial_obj.send_ack()
        self.serial_obj.wait_for_syn_ack()

    def teardown_class(self) -> None:
        self.serial_obj.close_connection()

    @mark.parametrize('message', generate_random_bytes(10))
    def test_unknown_message(self, message: str) -> None:

        self.serial_obj.send_message(message)
        assert self.serial_obj.receive_message() == message

    def test_message_buffering(self) -> None:

        self.serial_obj.send_message(b'ab')
        self.serial_obj.send_message(b'cd')
        self.serial_obj.send_message(b'ef')
        self.serial_obj.send_message(b'\n')

        assert self.serial_obj.receive_message() == b'abcdef\n'

    def test_empty_message(self) -> None:

        self.serial_obj.send_message(b'')
        self.serial_obj.send_message(b'')
        self.serial_obj.send_message(b'\n')

        assert self.serial_obj.receive_message() == b'\n'
