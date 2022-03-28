# pylint: disable=W0621  # Disable "Redefining name 'connection' from outer scope" since
#                        # dependency injection is how fixtures work

from os import EX_PROTOCOL, path
from uuid import uuid4
from time import sleep
from random import randint
from typing import List
import pytest
from serial_connection import SerialConnection

def map_com_port_to_device_file(port: str) -> str:

    try:
        port = int(port.strip('COM'))
    except ValueError:
        pytest.exit(f'Invalid port provided: {port}')

    return path.join('/dev', 'ttyS{}'.format(port - 1))

@pytest.fixture(scope='session')
def connection(pytestconfig):
    serial_port = pytestconfig.getoption('port')

    if 'COM' in serial_port:
        serial_port = map_com_port_to_device_file(serial_port)

    connection_handle = SerialConnection(serial_port=serial_port)
    if not connection_handle.three_way_handshake():
        pytest.exit('Failed to connect to device!', EX_PROTOCOL)

    yield connection_handle
    connection_handle.two_way_termination()

def generate_random_bytes(size: int) -> List[bytes]:

    random_bytes = []
    for _ in range(size):

        byte_array = str(uuid4()).encode()
        random_bytes.append(byte_array[:randint(1, 36)] + b'\n')

    return random_bytes

@pytest.mark.parametrize('message', generate_random_bytes(10))
def test_unknown_message(connection: SerialConnection, message: str) -> None:

    connection.send_message(message)
    assert connection.wait_for_message() == message

def test_message_buffering(connection: SerialConnection) -> None:

    connection.send_message(b'ab')
    connection.send_message(b'cd')
    connection.send_message(b'ef')
    connection.send_message(b'\n')

    assert connection.wait_for_message() == b'abcdef\n'

def test_empty_message(connection: SerialConnection) -> None:

    connection.send_message(b'')
    connection.send_message(b'')
    connection.send_message(b'\n')

    assert connection.wait_for_message() == b'\n'

@pytest.mark.skip(reason='Will fix ASAP')
def test_overflow_message(connection: SerialConnection) -> None:

    connection.send_message(b'abcdefghijabcdefghij')
    connection.send_message(b'abcdefghijabcdefghij')

    # readBytesUntil reads until either MESSAGE_TERMINATOR is received
    # or the number of bytes in buffer exceeds SIZE_MESSAGE_BUF
    connection.send_message(b'abcdefghij\n')

    assert connection.wait_for_message() == b'abcdefghijabcdefghijabcdefghijabcdefghij\n'
    assert connection.wait_for_message() == b'abcdefghij\n'

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
def test_toggle_builtin_led(connection: SerialConnection, message: bytes) -> None:

    connection.send_message(b'TEST\n')
    sleep(0.5)

    assert connection.wait_for_message() == message

@pytest.mark.parametrize(
    'command',
    [
        b'D\n',
        b'DI\n',
        b'DIG\n',
        b'DIGA\n',
        b'DIGAB\n',
        b'DIGABC\n',
    ]
)
def test_invalid_digital_pin_message(connection: SerialConnection, command: bytes) -> None:

    connection.send_message(command)
    assert connection.wait_for_message() == command

@pytest.mark.parametrize('pin', range(2, 14), ids=[f'Test digital pin {n}' for n in range(2, 14)])
def test_valid_digital_pin_message(connection: SerialConnection, pin: int) -> None:

    connection.send_message(f'DIG{pin}\n'.encode())
    assert connection.wait_for_message() == f'DIG{pin}: ON\n'.encode()

    connection.send_message(f'DIG{pin}\n'.encode())
    assert connection.wait_for_message() == f'DIG{pin}: OFF\n'.encode()
