#!/usr/bin/python

import Image
import sys

if len(sys.argv) < 2:
    print "Less argv"
    exit
    
im = Image.open(sys.argv[1])
im.show()
