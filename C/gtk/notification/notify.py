import pynotify

pynotify.init("notifiy")
n = pynotify.Notification("name", "Hello world")
n.show()
