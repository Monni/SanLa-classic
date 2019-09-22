import curses
from curses import panel
from utils import serial_ports
from cursesmenu import *
from cursesmenu.items import *

class Menu(object):

    def __init__(self, items, stdscreen):
        self.screen = stdscreen
        self.window = stdscreen.subwin(0,0)
        self.window.keypad(1)
        self.panel = panel.new_panel(self.window)
        self.panel.hide()
        panel.update_panels()

        self.position = 0
        self.items = items
        self.items.append(('exit','exit', {}))

    def navigate(self, n):
        self.position += n
        if self.position < 0:
            self.position = 0
        elif self.position >= len(self.items):
            self.position = len(self.items)-1

    def display(self):
        height, width = self.screen.getmaxyx()
        title = "SanLa Classic"[:width - 1]
        subtitle = "Written by Miika Avela and Atro Lähdemäki"[:width - 1]

        self.panel.top()
        self.panel.show()
        self.window.clear()

        while True:
            self.window.refresh()
            curses.doupdate()

            for index, item, in enumerate(self.items):
                kwargs = item[2]

                if index == self.position:
                    mode = curses.A_REVERSE
                else:
                    mode = curses.A_NORMAL

                msg = '%d. %s' % (index, item[0])
                self.window.addstr(1+index, 1, msg, mode)

            key = self.window.getch()

            if key in [curses.KEY_ENTER, ord('\n')]:
                if self.position == len(self.items)-1:
                    break
                else:
                    self.items[self.position][1](**kwargs) if kwargs else self.items[self.position][1]()

            elif key == curses.KEY_UP:
                self.navigate(-1)

            elif key == curses.KEY_DOWN:
                self.navigate(1)

        self.window.clear()
        self.panel.hide()
        panel.update_panels()
        curses.doupdate()



class Interface(object):

    def registerPort(self, port):
        print("Foo")
        raise Exception
        pass

    def __init__(self, stdscreen):
        self.port = None
        self.screen = stdscreen

        # Initialization
        self.screen.clear()
        height, width = self.screen.getmaxyx()
        curses.curs_set(0)

        # Turning on attributes for title
        self.screen.attron(curses.color_pair(2))
        self.screen.attron(curses.A_BOLD)

        # Welcome screen string declarations.
        title = "SanLa Classic"[:width - 1]
        subtitle = "Written by Miika Avela and Atro Lähdemäki"[:width - 1]
        serial_list = serial_ports()

        # Centering calculations
        start_x_title = int((width // 2) - (len(title) // 2) - len(title) % 2)
        start_y = int((height // 2) - 2)

        # Rendering title
        self.screen.addstr(start_y, start_x_title, title)

        ports = [(port, self.registerPort, {'port': port}) for port in serial_ports()]
        menu_select_device = Menu(ports, self.screen)


        submenu_items = [
                ('beep', curses.beep, {}),
                ('flash', curses.flash, {})
                ]
        submenu = Menu(submenu_items, self.screen)

        main_menu_items = [
            ('Select device', menu_select_device.display, {}),
            ('Send a message', menu_select_device.display, {}),
            ('Read messages', menu_select_device.display, {}),
        ]
        main_menu = Menu(main_menu_items, self.screen)
        main_menu.display()

if __name__ == '__main__':
    curses.wrapper(Interface)