from datetime import datetime
import re
import tkinter as tk
import tkinter.scrolledtext as tkscrolledtext
import serial_tx_rx

import utils


class Terminal(tk.Frame):
    def __init__(self, master) -> None:
        tk.Frame.__init__(self, master)
        self.pack()
        self.master = master
        self.master.title("TERMINAL - SANLA CLASSIC")
        self.configure(background='#525446')

        self.serialPort = serial_tx_rx.SerialConnection()
        self.serialPort.register_receive_callback(self.on_receive_serial_data)

        self.control = Control(master=self)
        self.content = tk.Frame(self)
        self.content.pack(fill='both', padx=10)
        self.messages = Messages(master=self.content)
        self.debug = Debug(master=self.content)

    def sdterm_main(self) -> None:
        self.master.after(200, self.sdterm_main)  # run the main loop once each 200 ms

    def on_receive_serial_data(self, message) -> None:
        str_message = message.decode("utf-8")
        prefix = '[0-9]:'

        action = None
        if re.match(prefix, str_message):
            action = str_message[:1]
            str_message = str_message[2:]

        # Insert possible functions here.
        switcher = {
            '1': self.messages.messages_box,
        }
        func = switcher.get(action)

        if func is not None:
            func.insert(tk.END, f'{datetime.now()} {str_message}')
            func.see(tk.END)
        self.debug.debug_box.insert(tk.END, f'{datetime.now()} -- {str_message}')
        self.debug.debug_box.see(tk.END)


class Control(tk.Frame):
    def __init__(self, master) -> None:
        tk.Frame.__init__(self, master)
        self.pack(fill='both')
        self.master = master

        self.frame_control = tk.Frame(self.master)
        self.frame_control.pack(fill=tk.BOTH)

        self.serial_control = tk.LabelFrame(self.frame_control, text='Serial control')
        self.serial_control.grid(row=0, column=0)
        self._com()
        self._baud_rate()
        self._debug()

        self._device_control()

    def _device_control(self) -> None:
        device_control = tk.LabelFrame(self.frame_control, text='SanLa Control')
        device_control.grid(row=0, column=1, sticky='NSEW')

        group_label = tk.Label(device_control, text='Group: ')
        group_label.grid(row=0, column=0)

        self.group_input = tk.Entry(device_control, width=10)
        self.group_input.insert(0, '65535')
        self.group_input.grid(row=0, column=1)

        group_button = tk.Button(device_control, text='Set', command=self._command_set_group)
        group_button.grid(row=0, column=2)

    def _command_set_group(self) -> None:
        message = f'{utils.SERIAL_COMMAND_CHOICES[utils.SERIAL_COMMAND_SET_GROUP]}{self.group_input.get()}\n'
        self.master.serialPort.send(message)

    def _com(self) -> None:
        label_com = tk.Label(self.serial_control, width=10, height=2, text="Port:")
        label_com.grid(row=0, column=0)
        label_com.config(font="bold")
        available_ports = utils.available_ports()
        self.com_var = tk.StringVar(self.serial_control)
        self.com_var.set(available_ports[0])
        com_edit = tk.OptionMenu(self.serial_control, self.com_var, *available_ports)
        com_edit.grid(row=0, column=1)
        com_edit.config(font="bold")
        self.com_button = tk.Button(self.serial_control, text="Open COM Port", width=15, command=self._command_com)
        self.com_button.config(font="bold")
        self.com_button.grid(row=0, column=2)

    def _baud_rate(self) -> None:
        label_baud = tk.Label(self.serial_control, width=10, height=2, text="Baud Rate:")
        label_baud.grid(row=1, column=0)
        self.baudrate_edit = tk.Entry(self.serial_control, width=10)
        self.baudrate_edit.grid(row=1, column=1)
        self.baudrate_edit.insert(tk.END, "115200")

    def _debug(self) -> None:
        self.show_debug = tk.IntVar()
        self.debug_button = tk.Checkbutton(self.serial_control, width=10, height=2, text='Debug',
                                           variable=self.show_debug, command=self._command_toggle_debug)
        self.debug_button.grid(row=1, column=2)

    def _command_com(self) -> None:
        if self.com_button.cget("text") == 'Open COM Port':
            comport = self.com_var.get()
            baudrate = self.baudrate_edit.get()
            self.master.serialPort.open(comport, baudrate)
            self.com_button.config(text='Close COM Port')
            self.master.debug.debug_box.insert('1.0', "COM Port Opened\r\n")
        elif self.com_button.cget("text") == 'Close COM Port':
            self.master.serialPort.close()
            self.com_button.config(text='Open COM Port')
            self.master.debug.debug_box.insert('1.0', "COM Port Closed\r\n")

    def _command_toggle_debug(self) -> None:
        if self.show_debug.get():
            self.master.debug.pack(fill=tk.BOTH)
        else:
            self.master.debug.pack_forget()


class Messages(tk.LabelFrame):
    def __init__(self, master):
        tk.LabelFrame.__init__(self, master, text='Messages', highlightbackground='#565747', highlightthickness=10)
        self.master = master
        self.pack(fill='x')
        self.columnconfigure(0, weight=4)
        self.columnconfigure(1, weight=1)

        self._sub_messages()
        self._sub_logo()

    def _sub_messages(self) -> None:
        sub_frame_messages = tk.Frame(self)
        sub_frame_messages.grid(row=0, column=0)

        self.messages_box = tkscrolledtext.ScrolledText(master=sub_frame_messages, wrap='word', height=18)
        self.messages_box.config(font="bold")
        self.messages_box.pack(fill='x')

        send_frame = tk.Frame(sub_frame_messages)
        send_frame.pack(fill=tk.X)
        self.message_input = tk.Entry(send_frame, width=100)
        self.message_input.pack(side=tk.LEFT, fill=tk.BOTH)
        button_send_message = tk.Button(send_frame, text="Send", width=20, command=self._command_send_message)
        button_send_message.config(font="bold")
        button_send_message.pack(side=tk.RIGHT)

    def _sub_logo(self) -> None:
        sub_frame_logo = tk.Frame(self)
        sub_frame_logo.grid(row=0, column=1)

        title = tk.Label(master=sub_frame_logo, text='SanLa Classic', justify=tk.CENTER)
        title.grid(row=0)

        sub_title = tk.Label(master=sub_frame_logo, text='Written by Miika Avela and Atro Lähdemäki')
        sub_title.grid(row=1)

    def _command_send_message(self) -> None:
        message = self.message_input.get()
        if self.master.master.serialPort.is_open():
            message = f'{utils.SERIAL_COMMAND_CHOICES[utils.SERIAL_COMMAND_SEND_MESSAGE]}{message}\n'

            self.master.master.serialPort.send(message)
            self.master.master.debug.debug_box.insert(tk.END, message)
            self.master.master.debug.debug_box.see(tk.END)
        else:
            self.master.master.debug.debug_box.insert(tk.END, 'Not sent - COM port is closed\n')
            self.master.master.debug.debug_box.see(tk.END)
        self.message_input.delete(0, tk.END)


class Debug(tk.LabelFrame):
    def __init__(self, master):
        tk.LabelFrame.__init__(self, master, text='Debug', highlightbackground='#565747', highlightthickness=10)
        self.master = master
        self.debug_box = None

        self._debug_content()
        self._debug_control()

    def _debug_content(self) -> None:
        debug_content = tk.Frame(self)
        debug_content.pack(fill='x')
        self.debug_box = tkscrolledtext.ScrolledText(master=debug_content, wrap='word', height=30)
        self.debug_box.config(font="bold")
        self.debug_box.pack(fill='both')

    def _debug_control(self) -> None:
        debug_control = tk.Frame(self)
        debug_control.pack(fill='x')

        button_clear = tk.Button(debug_control, width=20, height=2, text="Clear data", command=self._command_clear_data)
        button_clear.grid(row=0, column=0)
        button_clear.config(font="bold")

    def _command_clear_data(self) -> None:
        self.debug_box.delete('1.0', tk.END)


if __name__ == '__main__':
    root = tk.Tk()
    terminal = Terminal(root)
    terminal.master.after(200, terminal.sdterm_main)
    terminal.master.mainloop()
