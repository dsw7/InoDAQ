import logging
from os import EX_OK
from subprocess import run

def test_help_menu_long(pytestconfig):
    command = [pytestconfig.getoption('binary'), '--help']

    logging.debug('Running command: "%s"', ' '.join(command))
    process = run(command, capture_output=True, check=True)

    logging.debug('The stdout was:\n%s', process.stdout.decode())
    logging.debug('The stderr was:\n%s', process.stderr.decode())

    assert process.returncode == EX_OK

def test_help_menu_short(pytestconfig):
    command = [pytestconfig.getoption('binary'), '-h']

    logging.debug('Running command: "%s"', ' '.join(command))
    process = run(command, capture_output=True, check=True)

    logging.debug('The stdout was:\n%s', process.stdout.decode())
    logging.debug('The stderr was:\n%s', process.stderr.decode())

    assert process.returncode == EX_OK
