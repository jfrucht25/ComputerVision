from PIL import Image
import sys

filename = sys.argv[1]

im = Image.open(filename)

size = im.width, im.height
newsize = size[0]//5, size[1]//5

im.thumbnail(newsize, Image.ANTIALIAS)
pix=im.load()

s='P3 %d %d 255\n' % (im.width, im.height)

for i in range(im.height):
	for j in range(im.width):
		for k in range(3):
			s+='%d ' % pix[j,i][k]
	s=s[:-1]+'\n'

f = open(filename[:-3]+'ppm', 'w')
f.write(s)
f.close()
