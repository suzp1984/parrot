import sys
import urllib2
import re

BAIDUURL = 'http://www.baidu.com'

find_re = re.compile(r'<table class="result" id=.+?<h3 class="t">.*?<a.*?href="(.+?)".*?>(.+?)</a>.*?</h3>.*?<div class="c-abstract">(.+?)</div>', re.DOTALL)

if __name__ == "__main__":
    keyword = sys.argv[1]
    if len(sys.argv) > 2:
        pages = sys.argv[2]
    else:
        pages = '1'
    print 'keyword = ' + keyword
    print 'pages = ' + pages
    pages = int(pages)
    for page in range(pages):
        urlsearch = BAIDUURL+'/s?wd='+keyword+'&pn='+str(page)+'0'
        print 'Get from url ' + urlsearch
        html = urllib2.urlopen(urlsearch).read()
        for x in find_re.findall(html):
            itemurl = x[0]
            itemtitle = re.sub(r'<em>(.*?)</em>', r'\1', x[1])
            itemdescription = re.sub(r'<em>(.*?)</em>', r'\1', x[2])
            print "------"
            print itemurl
            print itemtitle
            print itemdescription

