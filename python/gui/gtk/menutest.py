#!/usr/bin/env python
# -*- coding: utf-8 -*

# Menu
# PyGtk Stady Notes By DawnDIY
# http://www.dawndiy.com

import pygtk
pygtk.require('2.0')
import gtk

# 菜单条
class Mb(gtk.MenuBar):
    def __init__(self):
        super(Mb,self).__init__()

        # 普通菜单
        filemenu = gtk.Menu()
        filem = gtk.MenuItem("File_1")
        filem.set_submenu(filemenu)

        new = gtk.MenuItem("New")
        filemenu.append(new)

        open = gtk.MenuItem("Open")
        filemenu.append(open)

        sep = gtk.SeparatorMenuItem()
        filemenu.append(sep)

        exit = gtk.MenuItem("Exit")
        exit.connect("activate", gtk.main_quit)
        filemenu.append(exit)

        # 带图标的菜单
        imagemenu = gtk.Menu()
        filei = gtk.MenuItem("File_2")
        filei.set_submenu(imagemenu)

        newi = gtk.ImageMenuItem(gtk.STOCK_NEW)
        newi.set_label("New")
        imagemenu.append(newi)

        openi = gtk.ImageMenuItem(gtk.STOCK_OPEN)
        openi.set_label("Open")
        imagemenu.append(openi)

        sep2 = gtk.SeparatorMenuItem()
        imagemenu.append(sep2)

        exiti = gtk.ImageMenuItem(gtk.STOCK_QUIT)
        exiti.set_label("Exit")
        exiti.connect("activate", gtk.main_quit)
        imagemenu.append(exiti)

        # 带复选框的菜单项
        checkmenu = gtk.Menu()
        filec = gtk.MenuItem("File_3")
        filec.set_submenu(checkmenu)

        newc = gtk.CheckMenuItem("New")
        newc.set_active(True)       # 激活复选框
        checkmenu.append(newc)

        openc = gtk.CheckMenuItem("Open")
        openc.set_active(False) 
        checkmenu.append(openc)

        sep3 = gtk.SeparatorMenuItem()
        checkmenu.append(sep3)

        exitc = gtk.CheckMenuItem("Exit")
        checkmenu.append(exitc)

        # 多级菜单
        topmenu = gtk.Menu()    
        files = gtk.MenuItem("File_4")
        files.set_submenu(topmenu)

        submenu = gtk.Menu()    # 上层

        news = gtk.MenuItem("New")
        news.set_submenu(submenu)

        sub1 = gtk.MenuItem("Sub1")     # 子层
        sub2 = gtk.MenuItem("Sub2")
        sub3 = gtk.MenuItem("Sub3")

        submenu.append(sub1)
        submenu.append(sub2)
        submenu.append(sub3)

        topmenu.append(news)

        self.append(filem)
        self.append(filei)
        self.append(filec)
        self.append(files)
class Win:
    def __init__(self):
        self.win = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.win.set_title("Menu")
        self.win.set_size_request(300,250)
        self.win.set_position(gtk.WIN_POS_CENTER)

        mb = Mb()
        vbox = gtk.VBox(False, 2)       
        vbox.pack_start(mb, False, False, 0)

        self.win.add(vbox)

        self.win.connect("destroy", gtk.main_quit)
        self.win.show_all()

if __name__ == "__main__":
    win = Win()
    gtk.main()
