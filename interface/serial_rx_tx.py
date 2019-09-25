import serial
import sys
import _thread


class SerialConnection:
    def __init__(self):
        self.port_name = ""
        self.baudrate = 115200
        self.timeout = None
        self.receive_callback_func = None
        self.is_open = False
        self.received_message = None
        self.serial = serial.Serial()

    def __del__(self):
        try:
            if self.serial.is_open():
                self.serial.close()
        except:
            print("Destructor error closing COM port: ", sys.exc_info()[0] )

    def register_receive_callback(self, receive_callback_func):
        self.receive_callback_func = receive_callback_func
        try:
            _thread.start_new_thread(self.serial_readline_thread, ())
        except:
            print("Error starting Read thread: ", sys.exc_info()[0])

    def serial_readline_thread(self):
        while True:
            try:
                if self.is_open:
                    self.received_message = self.serial.readline()
                    if self.received_message != "":
                        self.receive_callback_func(self.received_message)
            except:
                print("Error reading COM port: ", sys.exc_info()[0])

    def is_open(self):
        return self.is_open

    def open(self, port, baudrate):
        if not self.is_open:
            self.serial.port = port
            self.serial.baudrate = baudrate
            try:
                self.serial.open()
                self.is_open = True
            except:
                print("Error opening COM port: ", sys.exc_info()[0])

    def close(self):
        if self.is_open:
            try:
                self.serial.close()
                self.is_open = False
            except:
                print("Close error closing COM port: ", sys.exc_info()[0])

    def send(self, message):
        if self.is_open:
            try:
                # Ensure that the end of the message has both \r and \n, not just one or the other
                new_message = message.strip()
                new_message += '\r\n'
                self.serial.write(new_message.encode('utf-8'))
            except:
                print("Error sending message: ", sys.exc_info()[0] )
            else:
                return True
        else:
            return False




