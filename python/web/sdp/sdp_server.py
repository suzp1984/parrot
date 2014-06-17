import tornado.ioloop
import tornado.web
import socket
import urlparse
import sys
import json

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(('www.baidu.com', 80))
    return s.getsockname()[0]

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world!")
    def post(self):
        request_obj = urlparse.parse_qs(self.request.body)
        hook_name = request_obj.get("req")
        if type(hook_name) is list:
            hook_name = hook_name.pop()
        try:
            hook = getattr(sys.modules[__name__], hook_name)
        except AttributeError:
            return self.write(json.dumps({'status' : 1}))
        #self.response.headers['Content-Type'] = "application/json"
        if callable(hook):
            self.write(json.dumps({'status' : 0, 'ret' : hook()}))
        else:
            self.write(json.dumps({'status' : 1}))

if __name__ == "__main__":
    application = tornado.web.Application([
        (r"/", MainHandler),
        ])
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
    
