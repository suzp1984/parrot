import os

vars=os.environ

key='zpcat'
if vars.has_key(key):
	print vars[key]
else:
	print "unknow key ", key
