#!/usr/bin/env python
# -*- coding: utf-8 -*

# Table layout container
# PyGtk Study Notes By DawnDIY
# http://www.dawndiy.com

import pygtk
pygtk.require('2.0')
import gtk

class TableLC:
    def __init__(self):
        self.win = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.win.set_title("Table Layout Container")
        self.win.set_size_request(300,250)
        self.win.set_position(gtk.WIN_POS_CENTER)

        table = gtk.Table(4, 3, True)   # 4行3列
        table.attach(gtk.Button("1"), 0, 1, 0, 1)
        table.attach(gtk.Button("2"), 1, 2, 0, 1)
        table.attach(gtk.Button("3"), 2, 3, 0, 1)
        table.attach(gtk.Button("4"), 0, 1, 1, 2)
        table.attach(gtk.Button("5"), 1, 2, 1, 2)
        table.attach(gtk.Button("6"), 2, 3, 1, 2)
        table.attach(gtk.Button("7"), 0, 1, 2, 3)
        table.attach(gtk.Button("8"), 1, 2, 2, 3)
        table.attach(gtk.Button("9"), 2, 3, 2, 3)
        table.attach(gtk.Button("0"), 1, 2, 3, 4)

        self.win.add(table)
        self.win.connect("destroy", gtk.main_quit)
        self.win.show_all()

if __name__ == "__main__":
    tab = TableLC()
    gtk.main()
