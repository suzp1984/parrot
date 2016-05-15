from socket import *
import time

def benchmark(addr, nmessages):
    sock = socket(AF_INET, SOCK_STREAM)
    sock.connect(addr)
    start = time.time()
    for n in range(nmessages):
        sock.send(b'x')
        resp = sock.recv(10000)
    end = time.time()
    print(nmessages/(end-start), 'messages/sec')

benchmark(('localhost', 25000), 100000)
