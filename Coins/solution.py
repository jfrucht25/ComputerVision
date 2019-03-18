import cv2
import numpy as np

img = cv2.imread('coins.jpg',0)
img = cv2.medianBlur(img,5)
cimg = cv2.cvtColor(img,cv2.COLOR_GRAY2BGR)

circles = cv2.HoughCircles(img,cv2.cv.CV_HOUGH_GRADIENT,1,20,
                            param1=100,param2=80,minRadius=0,maxRadius=0)

circles = np.uint16(np.around(circles))
penny=0
nickel=0
quarter=0
small = min(circles[0],key= lambda x:x[2])[2]
print(small)
for i in circles[0]:
	ratio = float(i[2])/small
	if ratio<1.12:
		penny+=1
    		cv2.circle(cimg,(i[0],i[1]),i[2],(0,255,0),2)
	elif ratio<1.22:
		nickel+=1
    		cv2.circle(cimg,(i[0],i[1]),i[2],(0,0,255),2)
	else:
		quarter+=1
    		cv2.circle(cimg,(i[0],i[1]),i[2],(255,0,0),2)
	# draw the outer circle
    # draw the center of the circle
    	cv2.circle(cimg,(i[0],i[1]),2,(0,0,255),3)

print(penny, nickel, quarter)
print('total dollar ammount: $'+str(0.1*penny+0.05*nickel+0.25*quarter))
cv2.imwrite('traced.jpg', cimg)
