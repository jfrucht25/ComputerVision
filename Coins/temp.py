import cv2
import numpy as np
from matplotlib import pyplot as plt
 
img = cv2.imread('coins.jpg',0)
edges = cv2.Canny(img,2,500)
 
plt.plot(122),plt.imshow(edges,cmap = 'gray')
plt.title('Edge Image'), plt.xticks([]), plt.yticks([])
 
plt.show()
