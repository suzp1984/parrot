import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtWebKit import *
from PyQt5.QtWebKitWidgets import QWebView

class KuGou(QWidget):
    def __init__(self, parent = None):
        super(KuGou, self).__init__(parent)
        self.createLayout()
        self.createConnection()
       
      
    def createLayout(self):
        self.setWindowTitle("kugou player")
    
        #self.addressBar = QLineEdit()
        #self.goButton = QPushButton("&GO")
        #bl = QHBoxLayout()
        #bl.addWidget(self.addressBar)
        #bl.addWidget(self.goButton)
          
        self.webView = QWebView()
          
        layout = QVBoxLayout()
        layout.addWidget(self.webView)
        self.setLayout(layout)
      
    def createConnection(self):
        self.webView.load(QUrl("http://web.kugou.com"))
        #self.addressBar.returnPressed.connect(self.search)
        #self.addressBar.returnPressed.connect(self.addressBar.selectAll)
        #self.goButton.clicked.connect(self.search)
        #self.goButton.clicked.connect(self.addressBar.selectAll)
 
app = QApplication(sys.argv)
browser = KuGou()
browser.show()
sys.exit(app.exec_())
