#!/usr/bin/env python
# -*- coding: utf-8 -*

# Alignment layout container
# PyGtk Study Notes By DawnDIY
# http://www.dawndiy.com

import pygtk
pygtk.require('2.0')
import gtk

class AlignLC:
    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("Alignment Layout Container")
        self.window.set_size_request(300,250)
        self.window.set_position(gtk.WIN_POS_CENTER)

        btn1 = gtk.Button("btn1")
        btn2 = gtk.Button("btn2")
        btn3 = gtk.Button("btn3")

        vbox = gtk.VBox(True,0)
        vbox.add(btn1)
        vbox.add(btn2)
        vbox.add(btn3)

        align = gtk.Alignment(0.5,1,0.7,0)  # 子部件左边空余50%的空闲空间（居中），子部件以最大高度占满垂直空间，子部件宽度为水平空间的70%

        align.add(vbox)
        self.window.add(align)
        self.window.connect("destroy",gtk.main_quit)
        self.window.show_all()

if __name__ == "__main__":
    al = AlignLC()
    gtk.main()
