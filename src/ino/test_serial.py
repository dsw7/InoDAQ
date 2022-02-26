from time import sleep
from platform import uname
from json import dumps
import logging
import serial

logging.basicConfig(level=logging.DEBUG)

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
MESSAGE_ACK = 'ACK'


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

        logging.debug('Resetting input buffer')
        #self.serial_port_obj.reset_input_buffer()

        return True, None

    def close_connection(self) -> None:

        if self.serial_port_obj is None:
            logging.debug('Not closing connection. Connection was never opened!')
            return

        logging.debug('Closing connection!')

        if self.serial_port_obj.is_open:
            self.serial_port_obj.close()

    def wait_for_syn(self):

        logging.debug('Waiting to receive SYN message')
        syn_received = False

        while (not syn_received) and (self.run):

            while self.serial_port_obj.in_waiting < len(MESSAGE_ACK):
                pass

            logging.debug('Read to read data')
            bytes_from_dev = self.serial_port_obj.read_until()  # Reads until \n by default

            logging.debug('Received message: %s', bytes_from_dev)

            if bytes_from_dev == MESSAGE_SYN:
                logging.debug('Accepted SYN message')
                syn_received = True
            else:
                logging.debug('Received unknown bytes %s', bytes_from_dev)




s = Serial()
s.open_connection()
s.wait_for_syn()
s.close_connection()
