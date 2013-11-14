#!/usr/bin/env python
# -*- coding: utf-8 -*

# VBox layout container
# PyGtk Study Notes By DawnDIY
# http://www.dawndiy.com

import pygtk
pygtk.require('2.0')
import gtk

class VBoxLC:
    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_title("VBox Layout Container")
        self.window.set_size_request(300,250)
        self.window.set_position(gtk.WIN_POS_CENTER)

        self.window.connect("destroy", gtk.main_quit)

        vbox = gtk.VBox(False, 5)    # 建立 VBox 布局容器，空间不均等分配，子部件间隔 5 像素
        btn1 = gtk.Button("small")
        btn2 = gtk.Button("Big")
        btn2.set_size_request(300,200)

        vbox.add(btn1)
        vbox.add(btn2)
        self.window.add(vbox)

        self.window.show_all()

    def main(self):
        gtk.main()

if __name__ == "__main__":
    vbox = VBoxLC()
    vbox.main()
