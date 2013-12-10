import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class NumberFormatDlg(QDialog):
    def __init__(self, format, parent=None):
        super(NumberFormatDlg, self).__init__(format, parent)
        
