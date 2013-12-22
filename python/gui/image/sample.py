#!/usr/bin/python
# -*- coding: utf-8 -*-
#written by Sebastian Stetter in 2010


import sys
from PIL import Image
from PIL.ImageQt import ImageQt

from PyQt5.QtGui import *
from PyQt5.QtCore import *



def PILimageToQImage(pilimage):
    """converts a PIL image to QImage"""
    imageq = ImageQt(pilimage) #convert PIL image to a PIL.ImageQt object
    qimage = QImage(imageq) #cast PIL.ImageQt object to QImage object -that´s the trick!!!
    return qimage


if __name__ == "__main__":
    #Testcode
    app = QApplication(sys.argv)

    pim = Image.open(unicode(QFileDialog().getOpenFileName()))
    pim.show() #show pil image

    qim = PILimageToQImage(pim)
    pm = QPixmap(qim)
    lbl = QLabel()
    lbl.setPixmap(pm)
    lbl.show() #show label with qim image

    sys.exit(app.exec_())
