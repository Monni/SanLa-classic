import npyscreen
import utils


class SanlaInterface(npyscreen.NPSAppManaged):
    port = None

    def onStart(self):
        self.addForm("MAIN", MainForm, name="MAIN")
        self.addForm("DEVICES", DeviceForm, name="DEVICES")

    def change_form(self, name):
        self.switchForm(name)
        self.resetHistory()

    def set_port(self, port):
        npyscreen.notify_confirm(port, title="Woot!", wrap=True, wide=True, editw=1)
        self.port = port


class DeviceButton(npyscreen.ButtonPress):
    def whenPressed(self):
        self.parent.parentApp.setNextForm("DEVICES")


class MainForm(npyscreen.ActionFormV2WithMenus):

    def activate(self):
        self.edit()

    def create(self):
        self.how_exited_handers[npyscreen.wgwidget.EXITED_ESCAPE] = self.exit_application

        self.add(npyscreen.TitleFixedText, name="SanLa Classic")
        self.add(npyscreen.TitleFixedText, name="Port: ", value=self.parentApp.port)
        self.add(npyscreen.TitleText, name="Written by:", value="Miika Avela and Atro Lähdemäki")
        self.add(DeviceButton, name="Devices")

        self.m1 = self.add_menu(name="Device Menu")
        self.m1.addItemsFromList((port, self.register_port, None, None, (port,)) for port in utils.serial_ports())

        self.button = self.add(npyscreen.Button, name="Devices2", value_changed_callback=self.buttonPress)

        # Since we are inheriting the npyscreen.FormWithMenus class we can use menus, this will add an option to the menu to exit the program
        self.menu = self.new_menu(name="Main Menu", shortcut='^M')
        self.menu.addItem("Exit Program", self.exit_application, "^Q")

    def buttonPress(self, widget):
        npyscreen.notify_confirm("BUTTON PRESSED!", title="Woot!", wrap=True, wide=True, editw=1)
        self.parentApp.switchForm('DEVICES')

    def exit_application(self):
        self.parentApp.setNextForm(None)
        self.editing = False

    def register_port(self, port):
        self.port = port


class DeviceForm(npyscreen.ActionFormV2):

    def create(self):
        port = self.add(npyscreen.TitleSelectOne, max_height=10, name="Port", values=utils.serial_ports(), scroll_exit=False)

    def on_ok(self):
        self.parentApp.set_port(self.port.value)
        self.parentApp.switchForm("MAIN")

    def on_cancel(self):
        self.parentApp.switchForm("MAIN")

