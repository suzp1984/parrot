import os, sys
import Image

scale = 0.5

for infile in sys.argv[1:]:
    f, e = os.path.splitext(infile)
    outfile =  f + '_halfsize' + e

    try:
        im = Image.open(infile)
        # print "%dx%d" % im.size
        newsize = tuple(int(x * scale) for x in im.size)
        # print "%dx%d" % newsize
        im.resize(newsize, 1).save(outfile)
    except IOError:
        pass
        
