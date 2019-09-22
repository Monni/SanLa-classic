import aioserial
import asyncio
import time
from asyncio import transports
from typing import Optional

import serial_asyncio

import utils


class SerialInterface:
    def __init__(self, port, baudrate=115200):
        #self.loop = loop
        self.port = port
        self.baudrate = baudrate
        self.serial = aioserial.AioSerial(port=port)
        self.messages = []

    async def send(self, message):
        await self.serial.write_async(bytearray(message.encode()))
        print("ding")

    async def receive(self):
        while True:
            self.messages.append(await self.serial.read_async())

    async def is_open(self):
        while self.serial.is_open:
            time.sleep(1)
        return





    """async def connection_status(self):
        
        while True:
            time.sleep(1)
        return

    async def run(self):
        self.reader, self.writer = await serial_asyncio.open_serial_connection(
            loop=self.loop,
            url=self.port,
            baudrate=self.baudrate
        )

        await asyncio.wait([self.connection_status()])

    async def receive(self):
        while True:
            message = await self.reader.readuntil(b'\n')
            print(f'Received: {message.rstrip().decode()}')

    async def send(self, message):
        self.writer.write(f'{message}\n')
        print(f'Sent: {message.decode().rstrip()}')"""


if __name__ == '__main__':
    print(utils.SerialInterface.available_ports())
    #loop = asyncio.get_event_loop()
    serial_interface = SerialInterface(port='COM10')
    #loop.run_until_complete(serial_interface.is_open())

    asyncio.run(serial_interface.send('Foo'))
    asyncio.run(serial_interface.receive())

    #loop.run_until_complete(serial_interface.run())


    print('bar')
    try:
        pass
    except KeyboardInterrupt:
        pass
    finally:
        #loop.close()
        pass
