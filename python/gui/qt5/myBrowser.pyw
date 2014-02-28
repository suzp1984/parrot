import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtWebKit import *
from PyQt5.QtWebKitWidgets import *

class MyBrowser(QWidget):
    def __init__(self, parent = None):
        super(MyBrowser, self).__init__(parent)
        self.createLayout()
        self.createConnection()
       
    def search(self):
        address = str(self.addressBar.text())
        if address:
            if address.find('://') == -1:
                address = 'http://' + address
            url = QUrl(address)
            self.webView.load(url)
      
    def createLayout(self):
        self.setWindowTitle("keakon's browser")
    
        self.addressBar = QLineEdit()
        self.goButton = QPushButton("&GO")
        bl = QHBoxLayout()
        bl.addWidget(self.addressBar)
        bl.addWidget(self.goButton)
          
        self.webView = QWebView()
          
        layout = QVBoxLayout()
        layout.addLayout(bl)
        layout.addWidget(self.webView)
        self.setLayout(layout)
      
    def createConnection(self):
        self.addressBar.returnPressed.connect(self.search)
        self.addressBar.returnPressed.connect(self.addressBar.selectAll)
        self.goButton.clicked.connect(self.search)
        self.goButton.clicked.connect(self.addressBar.selectAll)
 
app = QApplication(sys.argv)
browser = MyBrowser()
browser.show()
sys.exit(app.exec_())
