import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Form(QDialog):
    def __init__(self, parent=None):
        super(Form, self).__init__(parent)
        grid = QGridLayout()
        self.principal_label = QLabel("Principal:")
        self.rate_label = QLabel("Rate:")
        self.years_label = QLabel("Years:")
        self.amount_label = QLabel("Amount:")
        
        self.principal = QDoubleSpinBox()
        self.principal.setRange(0.0, 100000000.0)
        self.principal.setValue(1.00)

        self.rate = QDoubleSpinBox()
        self.rate.setRange(0.0, 100)
        self.rate.setValue(0.0)

        self.years = QComboBox()

        self.amount = QLabel()

        grid.addWidget(self.principal_label, 0, 0)
        grid.addWidget(self.principal, 0, 1)
        grid.addWidget(self.rate_label, 1, 0)
        grid.addWidget(self.rate, 1, 1)
        grid.addWidget(self.years_label, 2, 0)
        grid.addWidget(self.years, 2, 1)
        grid.addWidget(self.amount_label, 3, 0)
        grid.addWidget(self.amount, 3, 1)
        
        self.setLayout(grid)
        
