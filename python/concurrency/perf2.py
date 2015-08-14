# perf2.py

from threading import Thread
from socket import *
import time
import sys

sock = socket(AF_INET, SOCK_STREAM)
sock.connect(('localhost', 25000))

n = 0
quit = False

def moniter():
    global n
    while not quit:
        time.sleep(1)
        print(n, ' reqs/second')
        n = 0

    
th = Thread(target=moniter)
th.deamon = True
th.start()

while True:
    try:
        sock.send(b'3')
        resp = sock.recv(100)
        n += 1
    except (KeyboardInterrupt):
        print "get keyboard Interrupt"
        quit = True
        sock.close()
        break

th.join()
