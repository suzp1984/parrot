#!/usr/bin/env python

import sys

from xml.etree import ElementTree as ET
e = ET.parser('file.xml')

if __name__ == '__main__':
	for d in e.findall('/element/path'):
		if d.find('string').text == 'match_name':
			sp_data = d.find('array').find('dict')
			break

