import BaseHTTPServer, shutil, os
from cStringIO import StringIO
class MyHTTPRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
	cmds = {'/ping' : 'ping www.baidu.com',
			'/ls'   : 'ls -all',
			}
	def do_GET(self):
		"""a get request"""
		f = self.send_head()
		if f:
			f = StringIO()
			machine = os.popen('hostname').readlines()[0]
			if self.path == '/':
				heading = "select a command to run on %s" % (machine)
				body = (self.getMenu() + 
						"<p>The screen won't update until the selected"
						"command has finished. Please be patient.")
			else:
				heading = "Execution of ''%s'' on %s" % (self.cmds[self.path], machine)

