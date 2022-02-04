import serial
import pytest


CONNECTION_KWARGS = {
    'port': '/dev/ttyS2',
    'baudrate': 9600,
    'parity': serial.PARITY_NONE,
    'stopbits': serial.STOPBITS_ONE,
    'bytesize': serial.EIGHTBITS,
    'timeout': 1
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

        for _ in range(10):

            self.handle.write(b'foobar\n')
            retval = self.handle.readlines()

            if not retval:
                continue

            if retval[0] == b'foobar\r\n':
                break
        else:
            pytest.exit(f'Could not connect to {self.handle.name} in time')

    def teardown_class(self) -> None:

        if self.handle.is_open:
            self.handle.close()

    @pytest.mark.parametrize(
        'command, on_msg, off_msg',
        [
            (b'D2\n',  b'2: ON\r\n',  b'2: OFF\r\n'),
            (b'D3\n',  b'3: ON\r\n',  b'3: OFF\r\n'),
            (b'D4\n',  b'4: ON\r\n',  b'4: OFF\r\n'),
            (b'D5\n',  b'5: ON\r\n',  b'5: OFF\r\n'),
            (b'D6\n',  b'6: ON\r\n',  b'6: OFF\r\n'),
            (b'D7\n',  b'7: ON\r\n',  b'7: OFF\r\n'),
            (b'D8\n',  b'8: ON\r\n',  b'8: OFF\r\n'),
            (b'D9\n',  b'9: ON\r\n',  b'9: OFF\r\n'),
            (b'D10\n', b'10: ON\r\n', b'10: OFF\r\n'),
            (b'D11\n', b'11: ON\r\n', b'11: OFF\r\n'),
            (b'D12\n', b'12: ON\r\n', b'12: OFF\r\n'),
            (b'D13\n', b'13: ON\r\n', b'13: OFF\r\n')
        ]
    )
    def test_digital_pins(self, command: bytes, on_msg: bytes, off_msg: bytes) -> None:

        self.handle.write(command)
        assert self.handle.readlines()[0] == on_msg

        self.handle.write(command)
        assert self.handle.readlines()[0] == off_msg
