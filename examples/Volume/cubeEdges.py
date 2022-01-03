import cv2
import numpy as np


def findPoints(contour):
    topy, topx, boty, botx, lefty, leftx, righty, rightx = 500, 0, 0, 0, 0, 500, 0, 0
    for cnt in contour:
        if cv2.contourArea(cnt) > 5:
            for arr in cnt:
                for x, y in arr:
                    if y <= topy:
                        topy = y
                        topx = x
                    elif y >= boty:
                        boty = y
                        botx = x
                    if x <= leftx:
                        leftx = x
                        lefty = y
                    elif x >= rightx:
                        rightx = x
                        righty = y
    tmpx = topx - leftx
    tmpy = lefty - topy
    midx = rightx - tmpx
    midy = righty + tmpy
    botx = midx
    cv2.circle(img, (topx, topy), 3, (0, 255, 0), 3)
    cv2.circle(img, (botx, boty), 3, (0, 255, 0), 3)
    cv2.circle(img, (rightx, righty), 3, (0, 255, 0), 3)
    cv2.circle(img, (leftx, lefty), 3, (0, 255, 0), 3)
    cv2.circle(img, (midx, midy), 3, (0, 255, 0), 3)


path = r'C:\Users\zigak\DepthAI\depthai-python\examples\volume\cube.jpg'
img = cv2.imread(path)
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = cv2.bilateralFilter(gray, 15, 75, 75)
'''
removeNoise = cv2.bilateralFilter(gray, 15, 75, 75)
threshImg = cv2.adaptiveThreshold(removeNoise, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
invertedDialetedThreshImg = cv2.bitwise_not(threshImg)
contours, hierarchy = cv2.findContours(invertedDialetedThreshImg,
                                       cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
findPoints(contours)
'''
corners = cv2.goodFeaturesToTrack(gray, 25, 0.01, 25)
corners = np.int0(corners)
for corner in corners:
    x, y = corner.ravel()
    cv2.circle(img, (x, y), 5, (255, 0, 0), -1)
while True:

    cv2.imshow('image', img)
    key = cv2.waitKey(1)
    if key == ord('q'):
        break
