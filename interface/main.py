import asyncio

import serial_asyncio

import utils


async def main():
    serial_interface = utils.SerialInterface('COM10')
    await asyncio.gather(serial_interface.connect())

if __name__ == '__main__':
    """loop = asyncio.get_event_loop()
    reader = serial_asyncio.create_serial_connection(loop, utils.Reader, 'COM10', baudrate=115200)
    #writer = serial_asyncio.create_serial_connection(loop, utils.Writer, 'COM10', baudrate=115200)
    asyncio.ensure_future(reader)
    print('Reader scheduled')
    #asyncio.ensure_future(writer)
    #print('Writer scheduled')
    #loop.call_later(10, loop.stop)
    loop.run_forever()
    print('Done')"""


    async def main(loop):
        reader, writer = await serial_asyncio.open_serial_connection(url='COM10', baudrate=115200)
        print('Reader created')
        print('Writer created')
        messages = [b'foo\n', b'bar\n', b'baz\n', b'qux\n']
        sent = send(writer, messages)
        received = recv(reader)
        await asyncio.wait([sent, received])


    async def send(w, msgs):
        for msg in msgs:
            w.write(msg)
            print(f'sent: {msg.decode().rstrip()}')
            await asyncio.sleep(0.5)
        w.write(b'DONE\n')
        print('Done sending')


    async def recv(r):
        while True:
            msg = await r.readuntil(b'\n')
            if msg.rstrip() == b'DONE':
                print('Done receiving')
                break
            print(f'received: {msg.rstrip().decode()}')


    loop = asyncio.get_event_loop()
    serial_interface = utils.SerialInterface(port='COM10')
    serial_interface.send(["foo"])
    loop.run_until_complete(serial_interface.connect(loop))



    loop.close()
