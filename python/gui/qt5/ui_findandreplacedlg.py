# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'find_replace.ui'
#
# Created: Wed Dec  4 21:32:13 2013
#      by: PyQt5 UI code generator 5.1.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_FindandReplaceDlg(object):
    def setupUi(self, FindandReplaceDlg):
        FindandReplaceDlg.setObjectName("FindandReplaceDlg")
        FindandReplaceDlg.resize(565, 360)
        self.line = QtWidgets.QFrame(FindandReplaceDlg)
        self.line.setGeometry(QtCore.QRect(310, 30, 20, 201))
        self.line.setFrameShape(QtWidgets.QFrame.VLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.layoutWidget = QtWidgets.QWidget(FindandReplaceDlg)
        self.layoutWidget.setGeometry(QtCore.QRect(390, 30, 110, 301))
        self.layoutWidget.setObjectName("layoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.layoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.findButton = QtWidgets.QPushButton(self.layoutWidget)
        self.findButton.setObjectName("findButton")
        self.verticalLayout.addWidget(self.findButton)
        self.replaceButton = QtWidgets.QPushButton(self.layoutWidget)
        self.replaceButton.setObjectName("replaceButton")
        self.verticalLayout.addWidget(self.replaceButton)
        self.replaceAllButton = QtWidgets.QPushButton(self.layoutWidget)
        self.replaceAllButton.setObjectName("replaceAllButton")
        self.verticalLayout.addWidget(self.replaceAllButton)
        spacerItem = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem)
        self.closeButton = QtWidgets.QPushButton(self.layoutWidget)
        self.closeButton.setObjectName("closeButton")
        self.verticalLayout.addWidget(self.closeButton)
        self.layoutWidget1 = QtWidgets.QWidget(FindandReplaceDlg)
        self.layoutWidget1.setGeometry(QtCore.QRect(0, 10, 291, 331))
        self.layoutWidget1.setObjectName("layoutWidget1")
        self.verticalLayout_3 = QtWidgets.QVBoxLayout(self.layoutWidget1)
        self.verticalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout_3.setObjectName("verticalLayout_3")
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setObjectName("verticalLayout_2")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label = QtWidgets.QLabel(self.layoutWidget1)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.findLineEdit = QtWidgets.QLineEdit(self.layoutWidget1)
        self.findLineEdit.setObjectName("findLineEdit")
        self.horizontalLayout.addWidget(self.findLineEdit)
        self.verticalLayout_2.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label_2 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_2.addWidget(self.label_2)
        self.replaceLineEdit = QtWidgets.QLineEdit(self.layoutWidget1)
        self.replaceLineEdit.setObjectName("replaceLineEdit")
        self.horizontalLayout_2.addWidget(self.replaceLineEdit)
        self.verticalLayout_2.addLayout(self.horizontalLayout_2)
        self.verticalLayout_3.addLayout(self.verticalLayout_2)
        self.horizontalLayout_3 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_3.setObjectName("horizontalLayout_3")
        self.checkBox = QtWidgets.QCheckBox(self.layoutWidget1)
        self.checkBox.setObjectName("checkBox")
        self.horizontalLayout_3.addWidget(self.checkBox)
        self.checkBox_2 = QtWidgets.QCheckBox(self.layoutWidget1)
        self.checkBox_2.setObjectName("checkBox_2")
        self.horizontalLayout_3.addWidget(self.checkBox_2)
        self.verticalLayout_3.addLayout(self.horizontalLayout_3)
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.label_3 = QtWidgets.QLabel(self.layoutWidget1)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout_4.addWidget(self.label_3)
        self.syntaxComboBox = QtWidgets.QComboBox(self.layoutWidget1)
        self.syntaxComboBox.setObjectName("syntaxComboBox")
        self.syntaxComboBox.addItem("")
        self.syntaxComboBox.addItem("")
        self.horizontalLayout_4.addWidget(self.syntaxComboBox)
        self.verticalLayout_3.addLayout(self.horizontalLayout_4)
        spacerItem1 = QtWidgets.QSpacerItem(20, 78, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_3.addItem(spacerItem1)

        self.retranslateUi(FindandReplaceDlg)
        self.closeButton.clicked.connect(FindandReplaceDlg.reject)
        QtCore.QMetaObject.connectSlotsByName(FindandReplaceDlg)

    def retranslateUi(self, FindandReplaceDlg):
        _translate = QtCore.QCoreApplication.translate
        FindandReplaceDlg.setWindowTitle(_translate("FindandReplaceDlg", "Find and Replace"))
        self.findButton.setText(_translate("FindandReplaceDlg", "&Find"))
        self.replaceButton.setText(_translate("FindandReplaceDlg", "&Replace"))
        self.replaceAllButton.setText(_translate("FindandReplaceDlg", "Replace &All"))
        self.closeButton.setText(_translate("FindandReplaceDlg", "Close"))
        self.label.setText(_translate("FindandReplaceDlg", "Find &what"))
        self.label_2.setText(_translate("FindandReplaceDlg", "Replace w&ith"))
        self.checkBox.setText(_translate("FindandReplaceDlg", "&Case sensitive"))
        self.checkBox_2.setText(_translate("FindandReplaceDlg", "Wh&ole words"))
        self.label_3.setText(_translate("FindandReplaceDlg", "&Syntax:"))
        self.syntaxComboBox.setItemText(0, _translate("FindandReplaceDlg", "Literal Text"))
        self.syntaxComboBox.setItemText(1, _translate("FindandReplaceDlg", "Regular Expression"))

