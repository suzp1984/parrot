import socket, threading, time
UDP_PORT = 43278; CHECK_PERIOD = 20; CHECK_TIMEOUT = 15

class Receiver(threading.Thread):
	"""receiver query packet and if the data is PyHB then reply PyHB"""
	def __init__(self, goOnEvent):
		super(Receiver, self).__init__()
		self.goOnEvent = goOnEvent
		self.recSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.recSocket.settimeout(CHECK_TIMEOUT)
		self.recSocket.bind(('', UDP_PORT))
	def run(self):
		while self.goOnEvent.isSet():
			try:
				data, addr = self.recSocket.recvfrom(5)
				print addr
				if data == 'PyHB':
					self.recSocket.sendto('PyHB', addr)
			except socket.timeout:
				pass

def main():
	queryEvent = threading.Event()
	queryEvent.set()
	queryServer = Receiver(queryEvent)
	queryServer.start()

if __name__ == '__main__':
	main()
