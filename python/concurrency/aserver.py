# aserver.py
#

from socket import *
from fib import fib
from collections import deque
from select import select
from concurrent.futures import ProcessPoolExecutor as Pool

pool = Pool(5)

tasks = deque()
recv_wait = {} # Mapping sockets -> tasks (generator)
send_wait = {}
future_wait = {}

future_notify, future_event = socketpair()

class AsyncSocket(object):
    def __init__(self, sock):
        self.sock = sock
        
    def recv(self, maxsize):
        yield 'recv', self.sock
        return self.sock.recv(maxsize)

    def send(self, data):
        yield 'send', self.sock
        return self.sock.send(data)

    def accept(self):
        yield 'recv', self.sock
        client, addr = self.sock.accept()
        return AsyncSocket(client), addr

    def __getattr__(self, name):
        return getattr(self.sock, name)
        
def future_done(future):
    tasks.append(future_wait.pop(future))
    future_notify.send(b'x')

def future_monitor():
    while True:
        yield 'recv', future_event
        future_event.recv(100)
    
tasks.append(future_monitor())

def run():
    while any([tasks, recv_wait, send_wait]):
        while not tasks:
            # no active tasks to run
            # wait for I/O
            can_recv, can_send, [] = select(recv_wait, send_wait, [])
            for s in can_recv:
                tasks.append(recv_wait.pop(s))
            for s in can_send:
                tasks.append(send_wait.pop(s))
                
        task = tasks.popleft()
        try:
            why, what = next(task)
            if why == 'recv':
                # must go wait somewhere
                recv_wait[what] = task
            elif why == 'send':
                send_wait[what] = task
            elif why == 'future':
                future_wait[what] = task
                what.add_done_callback(future_done)
            else:
                raise RuntimeError("ARG!")

        except StopIteration:
            pass
            

        
def fib_server(address):
    sock = AsyncSocket(socket(AF_INET, SOCK_STREAM))
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    sock.bind(address)
    sock.listen(5)

    while True:
        client, addr = yield from sock.accept()  # blocking
        print('Client Connection: ', addr)
        tasks.append(fib_handler(client))
        

def fib_handler(client):
    while True:
        req = yield from client.recv(100)  # blocking
        if not req:
            break
            
        n = int(req)
        future = pool.submit(fib, n)
        yield 'future', future
        resp = future.result() # Block

        resp = str(resp).encode('ascii') + b'\n'
        yield from client.send(resp)  # blocking
    #client.close()
        
                            
if __name__ == '__main__':
    tasks.append(fib_server(('', 25000)))
    run()
    

