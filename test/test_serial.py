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
        self.handle.close()

    def test_me(self) -> None:
        assert True
