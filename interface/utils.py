import glob

import asyncio
from asyncio import transports
from typing import Optional

import serial
import sys,os
import curses
from curses.textpad import Textbox, rectangle
from math import *

import serial_asyncio


def draw_menu(stdscr):
    serial_port = None

    cmd = 0
    cursor_x = 0
    cursor_y = 0

    # Clear and refresh the screen for a blank canvas
    stdscr.clear()
    stdscr.refresh()

    # Start colors in curses
    curses.start_color()
    curses.init_pair(1, curses.COLOR_CYAN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_WHITE)

    # Loop where k is the last character pressed
    while (cmd != ord('q')):

        # Initialization
        stdscr.clear()
        height, width = stdscr.getmaxyx()

        cursor_x = max(0, cursor_x)
        cursor_x = min(width-1, cursor_x)

        cursor_y = max(0, cursor_y)
        cursor_y = min(height-1, cursor_y)

        # Welcome screen string declarations.
        title = "SanLa Classic"[:width-1]
        subtitle = "Written by Miika Avela and Atro Lähdemäki"[:width-1]
        serial_list = SerialInterface.available_ports()
        keystr = "Last key pressed: {}".format(cmd)[:width-1]
        statusbarstr = "Press 'q' to exit | STATUS BAR | Pos: {}, {}".format(cursor_x, cursor_y)
        if cmd == 0:
            keystr = "No key press detected..."[:width-1]

        # Centering calculations
        start_x_title = int((width // 2) - (len(title) // 2) - len(title) % 2)
        start_x_subtitle = int((width // 2) - (len(subtitle) // 2) - len(subtitle) % 2)
        start_x_keystr = int((width // 2) - (len(keystr) // 2) - len(keystr) % 2)
        start_y = int((height // 2) - 2)


        # Rendering some text
        whstr = "Width: {}, Height: {}".format(width, height)
        stdscr.addstr(0, 0, whstr, curses.color_pair(1))

        # Render status bar
        stdscr.attron(curses.color_pair(3))
        stdscr.addstr(height-1, 0, statusbarstr)
        stdscr.addstr(height-1, len(statusbarstr), " " * (width - len(statusbarstr) - 1))
        stdscr.attroff(curses.color_pair(3))

        # Turning on attributes for title
        stdscr.attron(curses.color_pair(2))
        stdscr.attron(curses.A_BOLD)

        # Rendering title
        stdscr.addstr(start_y, start_x_title, title)

        # Turning off attributes for title
        stdscr.attroff(curses.color_pair(2))
        stdscr.attroff(curses.A_BOLD)

        # Print rest of text
        stdscr.addstr(start_y + 1, start_x_subtitle, subtitle)
        stdscr.addstr(start_y + 3, (width // 2) - 2, '-' * 4)
        stdscr.addstr(start_y + 5, start_x_keystr, keystr)
        stdscr.move(cursor_y, cursor_x)

        if not serial_port:
            box = curses.newwin(len(serial_list), 32, 1, 1)
            box.box()

            highlightText = curses.color_pair(1)
            normalText = curses.A_NORMAL
            row_num = len(serial_list)

            key_position = 1
            for index, port in enumerate(serial_list):
                if not serial_list:
                    box.addstr(1, 1, "No devices found!", highlightText)
                else:
                    box.addstr(index, 2, str(index) + " - " + port, highlightText if index == key_position else normalText)
                if index == len(serial_list):
                    break

        # Refresh the screen
        stdscr.refresh()
        box.refresh()

        # Wait for next input
        cmd = stdscr.getch()


class Writer(asyncio.Protocol):
    def connection_made(self, transport):
        """Store the serial transport and schedule the task to send data.
        """
        self.transport = transport
        print('Writer connection created')
        asyncio.ensure_future(self.send())
        print('Writer.send() scheduled')

    def connection_lost(self, exc):
        print('Writer closed')

    async def send(self):
        """Send four newline-terminated messages, one byte at a time.
        """
        message = b'foo\nbar\nbaz\nqux\n'
        for b in message:
            await asyncio.sleep(0.5)
            self.transport.serial.write(bytes([b]))
            print(f'Writer sent: {bytes([b])}')
        self.transport.close()


class Reader(asyncio.Protocol):
    def connection_made(self, transport):
        """Store the serial transport and prepare to receive data.
        """
        self.transport = transport
        self.buf = bytes()
        self.msgs_recvd = 0
        print('Reader connection created')

    def data_received(self, data):
        """Store characters until a newline is received.
        """
        self.buf += data
        if b'\n' in self.buf:
            lines = self.buf.split(b'\n')
            self.buf = lines[-1]  # whatever was left over
            for line in lines[:-1]:
                print(f'Reader received: {line.decode()}')
                self.msgs_recvd += 1
        if self.msgs_recvd == 4:
            self.transport.close()

    def connection_lost(self, exc):
        print('Reader closed')


class SerialConnection(asyncio.Protocol):
    def connection_made(self, transport: transports.Transport) -> None:
        self.transport = transport
        print("Port opened")

    def data_received(self, data: bytes) -> None:
        print(f"Data received {repr(data)}")
        if b'\n' in data:
            self.transport.close()

    def connection_lost(self, exc: Optional[Exception]) -> None:
        print('Connection lost')
        self.transport.loop.stop()


class SerialInterface:

    def __init__(self, port, baudrate=115200) -> None:
        self.loop = asyncio.get_event_loop()
        self.port = port
        self.baudrate = baudrate
        self.callback = None

    async def connect(self, loop) -> None:
        self.reader, self.writer = await serial_asyncio.open_serial_connection(url='COM10', baudrate=115200)
        print("Connected")
        received = self.receive(self.reader)

        await asyncio.wait([received])

    async def send(self, msgs):
        for msg in msgs:
            self.writer.write(msg)
            print(f'sent: {msg.decode().rstrip()}')
            await asyncio.sleep(0.5)
        self.writer.write(b'DONE\n')
        print('Done sending')

    @staticmethod
    async def receive(reader):
        while True:
            msg = await reader.readuntil(b'\n')
            if msg.rstrip() == b'DONE':
                print('Done receiving')
                break
            print(f'received: {msg.rstrip().decode()}')

    def disconnect(self) -> None:
        self.loop.close()

    def status(self) -> bool:
        #return self.serial.is_open()
        pass

    def register_callback(self, func) -> None:
        """
        Register callback function here.

        :param func: Function used in callback.
        """
        self.callback = func



    @staticmethod
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
