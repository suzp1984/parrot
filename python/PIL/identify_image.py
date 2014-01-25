import sys
import Image

for infile in sys.argv[1:]:
    try:
        im = Image.open(infile)
        print infile, im.format, "%dx%d" % im.size, im.mode
    except IOError:
        pass
        
