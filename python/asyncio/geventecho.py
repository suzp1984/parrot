from gevent.server import StreamServer

def echo(socket, address):
    print('New connection from %s:%s' % address)
    while True:
        data = socket.recv(10000)
        if not data:
            break
        socket.sendall(b'Got:' + data)
    socket.close()

if __name__ == '__main__':
    server = StreamServer(('', 25000), echo)
    server.server_forever()
