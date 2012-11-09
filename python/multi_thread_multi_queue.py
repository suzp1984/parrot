#!/usr/bin/env python
from threading import  Thread
import subprocess
from Queue import Queue
import re

num_ping_threads = 3
num_arp_threads = 3
in_queue = Queue()
out_queue = Queue()
ips = ["192.168.1.1","192.168.1.2","192.168.1.3","192.168.1.4"]

def pinger(i, iq, oq):
	""" pings subnet"""
	while True:
		ip = iq.get()
		print "Thread %s: Pinging %s" % (i, ip)
		ret = subprocess.call("ping -c 1 %s" % ip,
				shell=True,
				stdout=open('/dev/null', 'w'),
				stderr=subprocess.STDOUT)
		if ret == 0:
			oq.put(ip)
		else:
			print "%s: did not respond" % ip
		iq.task_done()

def arping(i, oq):
	"""grabs a valid IP address from a queue and gets macaddr"""
	while True:
		ip = oq.get()
		p = subprocess.Popen("arping -c 1 %s" % ip,
				shell=True,
				stdout=subprocess.PIPE)
		out = p.stdout.read()
		result = out.split()
		pattern = re.compile(":")
		macaddr = None
		for item in result:
			if re.search(pattern, item):
				macaddr = item
		print "IP Address: %s | Mac Address: %s " % (ip, macaddr)
		oq.task_done()

#Place ip address into in queue
for ip in ips:
	in_queue.put(ip)

#spawn pool of ping threads
for i in range(num_ping_threads):
	worker = Thread(target=pinger, args=(i, in_queue, out_queue))
	worker.setDaemon(True)
	worker.start()

#spawn pool arping threads
for i in range(num_arp_threads):
	worker = Thread(target=arping, args=(i, out_queue))
	worker.setDaemon(True)
	worker.start()

print "Main Thread Waiting"
in_queue.join()
out_queue.join()
print "Done"
