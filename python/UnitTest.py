#!/usr/bin/env python

import unittest
from widget import Widget

class WidgetTestCase(unittest.TestCase):
	def setUp(self):
		self.widget = Widget()
	def tearDown(self):
		self.widget.dispose()
		self.widget = None
	def testSize(self):
		self.assertEqual(self.widget.getSize(), (40, 40))
	def testResize(self):
		self.widget.resize(100, 100)
		self.assertEqual(self.widget.getSize(), (100, 100))

if __name__ == "__main__":
	suite = unittest.TestSuite()
	suite.addTest(WidgetTestCase("testSize"))
	suite.addTest(WidgetTestCase("testResize"))

	runner = unittest.TextTestRunner()
	runner.run(suite)

# the easy way
# if __name__ == '__main__':
# 	 unittest.main()



