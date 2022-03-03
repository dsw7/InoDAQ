from os import EX_PROTOCOL
from uuid import uuid4
from time import sleep
from random import randint
from typing import List
import pytest
from serial_connection import SerialConnection

def generate_random_bytes(size: int) -> List[bytes]:

    random_bytes = []
    for _ in range(size):

        byte_array = str(uuid4()).encode()
        random_bytes.append(byte_array[:randint(1, 36)] + b'\n')

    return random_bytes


class TestSerial:

    def setup_class(self) -> None:

        self.serial_obj = SerialConnection()

        if not self.serial_obj.three_way_handshake():
            pytest.exit('Failed to connect to device!', EX_PROTOCOL)

    def teardown_class(self) -> None:
        self.serial_obj.two_way_termination()

    @pytest.mark.parametrize('message', generate_random_bytes(10))
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

    def test_overflow_message(self) -> None:

        self.serial_obj.send_message(b'abcdefghijabcdefghij')
        self.serial_obj.send_message(b'abcdefghijabcdefghij')

        # readBytesUntil reads until either MESSAGE_TERMINATOR is received
        # or the number of bytes in buffer exceeds SIZE_MESSAGE_BUF
        self.serial_obj.send_message(b'abcdefghij\n')

        assert self.serial_obj.receive_message() == b'abcdefghijabcdefghijabcdefghijabcdefghij\n'
        assert self.serial_obj.receive_message() == b'abcdefghij\n'

    @pytest.mark.parametrize(
        'message',
        [
            b'Built in LED is ON\n',
            b'Built in LED is OFF\n',
            b'Built in LED is ON\n',
            b'Built in LED is OFF\n'
        ],
        ids=[
            '(1) Test built in LED is turned ON',
            '(2) Test built in LED is turned OFF',
            '(3) Test built in LED is turned ON',
            '(4) Test built in LED is turned OFF'
        ]
    )
    def test_toggle_builtin_led(self, message: bytes) -> None:

        self.serial_obj.send_message(b'TEST\n')
        sleep(0.5)
        assert self.serial_obj.receive_message() == message
