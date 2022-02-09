from time import sleep
import serial
import pytest

MAX_SIZE_PAYLOAD = 25

CONNECTION_KWARGS = {
    'port': '/dev/ttyS2',
    'baudrate': 9600,
    'parity': serial.PARITY_NONE,
    'stopbits': serial.STOPBITS_ONE,
    'bytesize': serial.EIGHTBITS,
    'timeout': 5
}


class TestInoDAQ:

    def setup_class(self) -> None:

        try:
            self.handle = serial.Serial(
                **CONNECTION_KWARGS
            )
        except serial.serialutil.SerialException as exception:
            pytest.exit(f'An exception occurred: "{exception}"')

        if not self.handle.is_open:
            pytest.exit(f'Could not connect to {self.handle.name}')

        # A new connection will force Arduino to auto-reset (see Request To Send)
        # Wait a couple of seconds for device to reset
        sleep(2)

    def teardown_class(self) -> None:

        if self.handle.is_open:
            self.handle.close()

    @pytest.mark.parametrize(
        'command, on_msg, off_msg',
        [
            (b'D2\n',  b'2: ON',  b'2: OFF'),
            (b'D3\n',  b'3: ON',  b'3: OFF'),
            (b'D4\n',  b'4: ON',  b'4: OFF'),
            (b'D5\n',  b'5: ON',  b'5: OFF'),
            (b'D6\n',  b'6: ON',  b'6: OFF'),
            (b'D7\n',  b'7: ON',  b'7: OFF'),
            (b'D8\n',  b'8: ON',  b'8: OFF'),
            (b'D9\n',  b'9: ON',  b'9: OFF'),
            (b'D10\n', b'10: ON', b'10: OFF'),
            (b'D11\n', b'11: ON', b'11: OFF'),
            (b'D12\n', b'12: ON', b'12: OFF'),
            (b'D13\n', b'13: ON', b'13: OFF')
        ]
    )
    def test_digital_pins(self, command: bytes, on_msg: bytes, off_msg: bytes) -> None:

        self.handle.write(command)
        assert on_msg in self.handle.read(MAX_SIZE_PAYLOAD)

        self.handle.write(command)
        assert off_msg in self.handle.read(MAX_SIZE_PAYLOAD)
