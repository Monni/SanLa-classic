import tkinter as tk
import tkinter.scrolledtext as tkscrolledtext
from tkinter import *
from tkinter import filedialog

import serial

import serial_rx_tx
import _thread
import time
import webbrowser
from tkinter import messagebox

import utils

"""def OpenCommand():
    if button_openclose.cget("text") == 'Open COM Port':
        comport = com_var.get()
        baudrate = baudrate_edit.get()
        serialPort.open(comport, baudrate)
        button_openclose.config(text='Close COM Port')
        message_box.insert('1.0', "COM Port Opened\r\n")
    elif button_openclose.cget("text") == 'Close COM Port':
        if button_replaylog.cget('text') == 'Stop Replay Log':
            message_box.insert('1.0', "Stop Log Replay first\r\n")
        else:
            serialPort.close()
            button_openclose.config(text='Open COM Port')
            message_box.insert('1.0', "COM Port Closed\r\n")"""


# globals
serialPort = serial_rx_tx.SerialConnection()
logFile = None

root = tk.Tk()
root.title("TERMINAL - SANLA CLASSIC")
root.configure(background='#525446')


# set up the window size and position
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()
window_width = screen_width/2
window_height = screen_width/3
window_position_x = screen_width/2 - window_width/2
window_position_y = screen_height/2 - window_height/2
root.geometry('%dx%d+%d+%d' % (window_width, window_height, window_position_x, window_position_y))

frame_control = tk.LabelFrame(root, text='Control', borderwidth=10)
frame_control.grid(row=0, sticky='EW', padx=10, pady=10)

frame_message = tk.LabelFrame(root, text='Message', borderwidth=10, highlightbackground='#565747', highlightthickness=10)
frame_message.grid(row=1, sticky='EW')
message_box = tk.Entry(master=frame_message, width=root.winfo_screenwidth(), justify='center')
#message_box = tkscrolledtext.ScrolledText(master=frame_message, wrap='word', width=root.winfo_screenwidth(), height=10)
message_box.config(font="bold")
message_box.grid(row=0, sticky='EW')

frame_debug = tk.LabelFrame(root, text='Debug')
frame_debug.grid(row=10)

foo = tk.LabelFrame(frame_debug, text='asd')
foo.grid(row=1)

message_box = tkscrolledtext.ScrolledText(master=frame_debug, wrap='word', width=root.winfo_screenwidth(), height=18)
message_box.config(font="bold")
message_box.grid(row=2)

# COM Port
label_com = Label(frame_control, width=10, height=2, text="Port:")
label_com.grid(row=0, column=0)
label_com.config(font="bold")
available_ports = utils.available_ports()
com_var = StringVar(frame_control)
com_var.set(available_ports[0])
com_edit = OptionMenu(frame_control, com_var, *available_ports)
#com_edit.place(x=100, y=36)
com_edit.grid(row=0, column=1)
com_edit.config(font="bold")
com_button = Button(root, text="Open COM Port", width=20, ) # command=OpenCommand
com_button.config(font="bold")
com_button.place(x=210,y=30)

#Baud Rate
label_baud = Label(frame_control, width=10, height=2, text="Baud Rate:")
label_baud.grid(row=0, column=2)
baudrate_edit = Entry(root,width=10)
baudrate_edit.grid(row=0, column=3)
baudrate_edit.config(font="bold")
baudrate_edit.insert(END,"115200")



# serial data callback function
def OnReceiveSerialData(message):
    str_message = message.decode("utf-8")
    message_box.insert('1.0', str_message)

# Register the callback above with the serial port object
serialPort.register_receive_callback(OnReceiveSerialData)

def sdterm_main():
    root.after(200, sdterm_main)  # run the main loop once each 200 ms

#
#  commands associated with button presses
#

"""

def ClearDataCommand():
    message_box.delete('1.0', END)

def SendDataCommand():
    message = senddata_edit.get()
    if serialPort.is_open():
        message += '\r\n'
        serialPort.send(message)
        message_box.insert('1.0', message)
    else:
        message_box.insert('1.0', "Not sent - COM port is closed\r\n")



def ReplayLogFile():
    try:
      if logFile != None:
        readline = logFile.readline()
        global serialPort
        serialPort.send(readline)
    except:
      print("Exception in ReplayLogFile()")

def ReplayLogThread():
    while True:
        time.sleep(1.0)
        global logFile
        if serialPort.is_open():
            if logFile != None:
                ReplayLogFile()

def OpenLogFile():
    if not serialPort.is_open():
        message_box.insert('1.0', "Open COM port first\r\n")
    else:
        if button_replaylog.cget('text') == 'Replay Log':
            try:
                root.filename = filedialog.askopenfilename(initialdir="/", title="Select file",
                                                           filetypes=(("log files", "*.log"), ("all files", "*.*")))
                global logFile
                logFile = open(root.filename,'r')
                _thread.start_new_thread(ReplayLogThread, ())
                button_replaylog.config(text='Stop Log Replay')
                message_box.insert('1.0', "Sending to open COM port from: " + root.filename + "\r\n")
            except:
                message_box.insert('1.0', "Could not open log file\r\n")
        else:
            button_replaylog.config(text='Replay Log')
            message_box.insert('1.0', "Stopped sending messages to open COM port\r\n")
            logFile = None"""

def DisplayAbout():
    tk.messagebox.showinfo(
    "About",
    "Written by Dale Gambill (same as 'Software Guy For You' on Youtube)\r\n\r\n" 
    "SDTERM demonstrates event-handling of serial COM port data as follows:\r\n\r\n" 
    "1 - getting messages from a COM port via a callback function\r\n" 
    "2 - sending messages from a file to the COM port, one at a time\r\n" 
    "3 - sending log file messages and receiving messages at the same time\r\n\r\n" 
    "Source code at: Github URL: https://github.com/dalegambill/PythonTerminal\r\n")

def TutorialsWebPage():
    webbrowser.open("https://www.youtube.com/channel/UCouhHzMMU9c-Qh-TkZl5GDg",
                    new=1, autoraise=True)

# COM Port open/close button
"""button_openclose = Button(root,text="Open COM Port",width=20,command=OpenCommand)
button_openclose.config(font="bold")
button_openclose.place(x=210,y=30)

#Clear Rx Data button
button_cleardata = Button(root,text="Clear Rx Data",width=20,command=ClearDataCommand)
button_cleardata.config(font="bold")
button_cleardata.place(x=210,y=72)

#Send Message button
button_senddata = Button(root,text="Send Message",width=20,command=SendDataCommand)
button_senddata.config(font="bold")
button_senddata.place(x=420,y=72)

#Replay Log button
button_replaylog = Button(root,text="Replay Log",width=20,command=OpenLogFile)
button_replaylog.config(font="bold")
button_replaylog.place(x=420,y=30)

#About button
button_about = Button(root,text="About",width=16,command=DisplayAbout)
button_about.config(font="bold")
button_about.place(x=620,y=30)

#Tutorials
button_tutorials = Button(root,text="Tutorials",width=16,command=TutorialsWebPage)
button_tutorials.config(font="bold")
button_tutorials.place(x=780,y=30)

#
# data entry labels and entry boxes
#

#Send Data entry box
senddata_edit = Entry(root,width=34)
senddata_edit.place(x=620,y=78)
senddata_edit.config(font="bold")
senddata_edit.insert(END,"Message")



"""

#
# The main loop
#
root.after(200, sdterm_main)
root.mainloop()
#


