
import sys
import time
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PIL import Image
import StringIO

#output = StringIO.StringIO()

img = Image.open("./myself-new.png")
#output.write(img.tostring())
#print output.getvalue()
#output.close()
print img.tostring().encode('string-escape')
#text_file = open("result.txt", 'w')
#text_file.write("%s" % img.tostring())
#text_file.close()
