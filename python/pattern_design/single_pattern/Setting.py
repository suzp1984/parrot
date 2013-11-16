#! /usr/bin/python
# File:    Setting.py
# Author:  zxsu <suzp1984@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
# Copyright 2005 Duke University

# How to implements single instance pattern in python,
# how to use @classmethod 

# History:
## =====================
# 2013-11-13 23:41 zxsu <suzp1984@gmail.com> created.
## =====================

class Setting(object):
    instance = None
    
    def __init__(self, num=0):
        print "Setting init.." + str(num)
        pass
        
    @classmethod
    def get_instance(cls):
        """ This class is a singlton so use this method to use it. """
        if cls.instance:
            return cls.instance
                
        cls.instance = Setting()
        return cls.instance
                
                
if __name__ == "__main__":
    setting1 = Setting.get_instance()
    Setting2 = Setting.get_instance()
    Setting3 = Setting(2)
