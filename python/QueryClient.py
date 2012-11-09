import socket, time, threading
import os, re, string
""" Recevier to receiver reply message form server part """
SERVER_PORT = 43278; BEAT_PERIOD = 5
CLIENT_PORT = 43279
CHECK_TIMEOUT = 15
USERNAME = 'zxsu'
print 'Sending heartbeat query broadcast to port %d' % (SERVER_PORT)
print 'press Ctrl-C to stop'

def printAddr(ip):
	#print ip[0]
	print USERNAME + '@' + '%s' % ip[0]

def sshLogin(ip):
	print "login "
	os.execvp('ssh', ('ssh', USERNAME + '@' + '%s' % ip[0]))

class Recevier(threading.Thread):
	"""recevier reply from server"""
	def __init__(self, goOnEvent, recSocket, hookFunc):
		super(Recevier, self).__init__()
		self.goOnEvent = goOnEvent
		self.recSocket = recSocket
		self.hookFunc = hookFunc
	def run(self):
		while self.goOnEvent.isSet():
			try:
				data, addr = self.recSocket.recvfrom(5)
				if data == 'PyHB':
					self.hookFunc(addr)
					#print addr # Add hole func here!!
			except socket.timeout:
				pass
				

hbSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
hbSocket.settimeout(CHECK_TIMEOUT)
hbSocket.bind(('', CLIENT_PORT))

recevierEvent = threading.Event()
recevierEvent.set()
hbRecevier = Recevier(recevierEvent, hbSocket, sshLogin)
hbRecevier.start()


ip_p = re.compile(r'([1]?\d\d?|2[0-4]\d|25[0-5])\.([1]?\d\d?|2[0-4]\d|25[0-5])\.([1]?\d\d?|2[0-4]\d|25[0-5])\.([1]?\d\d?|2[0-4]\d|25[0-5])')
suspend_ips = [] 

def _add2ips(item):
	suspend_ips.append(item)

def _sendQuery(ip):
	hbSocket.sendto('PyHB', (ip, SERVER_PORT))

def filter_line(filter, str, hookFunc):
	""" filter str line match the filter rule. and add the match ip to suspend_ips"""
	ip_list = filter.findall(str)
	for ip_tuple in ip_list:
		#print string.join(list(ip_tuple), '.')
		hookFunc(string.join(list(ip_tuple), '.'))

reader = os.popen('nmap -p 43278 192.168.1.0/24')
for line in reader:
	filter_line(ip_p, line, _sendQuery)

#for ip in suspend_ips:
#	hbSocket.sendto('PyHB', (ip, SERVER_PORT))

try:
	while True:
		time.sleep(10)
except KeyboardInterrupt:
	print 'Exiting, please wait...'
	recevierEvent.clear()
	hbRecevier.join()
	print 'Finished.'
