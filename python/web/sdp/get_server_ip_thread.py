import json
import urllib
import urllib2
import ipaddress
import time

start_time = time.time()

from threading import Thread
from Queue import Queue

init_ip = u'192.168.1.1'

post_params = {
    'req' : 'get_ip'
    }

queue = Queue()
num_threads = 50 
url='http://localhost:8888'
params = urllib.urlencode(post_params)
ip = ipaddress.ip_address(init_ip)

def isServer(ip):
    if type(ip) is not str:
        return False
    url = 'http://' + ip + ':8888'
    try:
        response = urllib2.urlopen(url, params, timeout=0.5)
        json_response = json.load(response)
        if json_response.has_key('status') and json_response.get('status') == 0:
            print '*******************' + json_response.get('ret')
            print time.time() - start_time
            return True
            #exit(0)
    except urllib2.URLError:
        return False
    except urllib2.HTTPError:
        return False
    return False

def runner(i, q):
    while True:
        ip = q.get()
        if isServer(ip):
            pass
        q.task_done()
    
for i in range(num_threads):
    worker = Thread(target=runner, args=(i, queue))
    worker.setDaemon(True)
    worker.start()

    
for i in range(253):
    queue.put(str(ip+i))

print "Main threading waiting"
queue.join()
print "Done"
    
