import json
import urllib
import urllib2
import ipaddress

import time

start_time = time.time()

init_ip = u'192.168.1.1'

post_params = {
    'req' : 'get_ip'
    }

url='http://localhost:8888'
params = urllib.urlencode(post_params)
ip = ipaddress.ip_address(init_ip)
i = 0
while i < 253:
    #print str(ip)
    i = i + 1
    url = u'http://' + str(ip) + u':8888'
    ip = ip + 1
    print url
    try:
        response = urllib2.urlopen(url, params, timeout=0.1)
        json_response = json.load(response)
        if (json_response.has_key('status') and json_response.get('status') == 0):
            print '**************' + json_response.get('ret')
            print time.time() - start_time
            exit(0)
            break
    except urllib2.HTTPError, err:
        continue
    except urllib2.URLError:
        continue

    

        
                               
#response = urllib2.urlopen(url, params)
#print response.read()
#json_response = json.loads(response.read())

