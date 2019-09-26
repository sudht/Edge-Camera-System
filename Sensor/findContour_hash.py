import cv2 as cv
import numpy as np
import copy
from PIL import Image
import imagehash

def setLabel(image, str, contour):

   fontface = cv.FONT_HERSHEY_SIMPLEX
   scale = 0.6
   thickness = 2

   size = cv.getTextSize(str, fontface, scale, thickness)
   text_width = size[0][0]
   text_height = size[0][1]

   x, y, width, height = cv.boundingRect(contour)

   pt = (x + int((width - text_width) / 2), y + int((height + text_height) / 2))
   cv.putText(image, str, pt, fontface, scale, (255, 255, 255), thickness, 8)

def showim(image):
   cv.imshow("result", image)
   cv.waitKey(0)


img_color = cv.imread('a.jpg')
showim(img_color)


img_gray = cv.cvtColor(img_color, cv.COLOR_BGR2GRAY)
showim(img_gray)

#ret, img_binary = cv.threshold(img_gray, 127, 255, 0)
blur = cv.GaussianBlur(img_gray,(5,5),0)
ret, img_binary = cv.threshold(img_gray, 0, 255, cv.THRESH_BINARY+cv.THRESH_OTSU)
showim(img_binary)

#cv.imshow("resu", img_binary)
#cv.waitKey(0)


contours, hierarchy = cv.findContours(img_binary, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)

img_tmp = copy.deepcopy(img_color)

for cnt in contours:
    cv.drawContours(img_tmp, [cnt], 0, (255, 0, 0), 3)  # blue

showim(img_tmp)
i=0

for cnt in contours:
    x, y, w, h = cv.boundingRect(cnt)
    if w <50 or h<50 or w>500 or h>500:
          continue
    i = i+1
    cv.rectangle(img_color, (x, y), (x + w, y + h), (0, 255, 0), 2)
    img_trim = img_color[y:y+h, x:x+w]
    file_name = str(i) + '.jpg'
    cv.imwrite(file_name,img_trim)
    tmp = imagehash.average_hash(Image.open(file_name))
    setLabel(img_color, str(tmp), cnt)

showim(img_color)
