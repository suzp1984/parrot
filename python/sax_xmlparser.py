#!/usr/bin/python 
#
import sys, string

from xml.sax import handler, make_parser

class TestHandler(handler.ContentHandler):
	def __init__(self):
		pass
	def startDocument(self):
		print "Document start..."
	
	def endDocument(self):
		print "Document end..."

	def startElement(self, name, attrs):
		print 'start tag: ', name
	
	def endElement(self, name):
		print 'end tag: ', name
	
	def characters(self, chars):
		print chars


def test():
	handler = TestHandler()
	parser = make_parser()
	parser.setContentHandler(handler)
	f = open(sys.argv[1], 'r')
	parser.parse(f)
	f.close()

if __name__ == '__main__':
	test()

		

