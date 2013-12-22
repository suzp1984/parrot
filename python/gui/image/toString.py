
import sys
import time
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PIL import Image
import StringIO

output = StringIO.StringIO()

img = Image.open("./myself-new.png")
#print img.tostring()
