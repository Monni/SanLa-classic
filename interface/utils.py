import glob
import sys

import serial

SERIAL_COMMAND_SEND_MESSAGE = 'send_message'
SERIAL_COMMAND_SET_GROUP = 'set_group'
SERIAL_COMMAND_CHOICES = {
    SERIAL_COMMAND_SEND_MESSAGE: '1:',
    SERIAL_COMMAND_SET_GROUP: '2:',
}


def available_ports() -> list:
    """
     Lists serial port names.

    :raises EnvironmentError: On unsupported or unknown platforms
    :returns: Serial ports available on the system.
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result