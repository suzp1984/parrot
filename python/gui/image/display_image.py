
import sys
import time
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

app = QApplication(sys.argv)

#label.setWindowFlags(Qt.SplashScreen)
#label.show()

img = QImage("./myself-new.png")

pm = QPixmap(img)

lbl = QLabel()
lbl.setPixmap(pm)
lbl.show()

app.exec_()

        
