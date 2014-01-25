import sys
import os
import Image

def roll(image, delta):
    "Roll an image sideways"
    xsize, ysize = image.size

    delta = int(delta) % xsize
    if delta == 0: return image

    part1 = image.crop((0, 0, delta, ysize))
    part2 = image.crop((delta, 0, xsize, ysize))
    image.paste(part2, (0, 0, xsize-delta, ysize))
    image.paste(part1, (xsize-delta, 0, xsize, ysize))

    return image

if len(sys.argv) < 3:
    print "[Usage]: %s delta files"  % __file__
    print "Less argv"
    exit

if not sys.argv[1].isdigit():
    print "[Usage]: %s delta file" % __file__
    print "delta must be digit"
    exit

delta = sys.argv[1]

for infile in sys.argv[2:]:
    try:
        outfile = os.path.splitext(infile)[0] + ".roll" + os.path.splitext(infile)[-1]
        im = Image.open(infile)
        out_im = roll(im, delta)
        out_im.save(outfile)
    except IOError:
        pass
