import cv2
import numpy as np
import argparse
from pathlib import Path
import glob


def visualizeSeteroFeaturesAlign(left_frame,right_frame):

    sift = cv2.SIFT_create()
    kp1, des1 = sift.detectAndCompute(left_frame,None)
    kp2, des2 = sift.detectAndCompute(right_frame,None)

    FLANN_INDEX_KDTREE = 1
    index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 2)
    search_params = dict(checks=10)
    flann = cv2.FlannBasedMatcher(index_params,search_params)
    matches = flann.knnMatch(des1,des2,k=2)
    ptsLeft = []
    ptsRight = []
    for i,(m,n) in enumerate(matches):
        if m.distance < 0.3*n.distance:
            ptsLeft.append ((int(kp1[m.queryIdx].pt[0]), int(kp1[m.queryIdx].pt[1])))
            ptsRight.append((int(kp2[m.trainIdx].pt[0]), int(kp2[m.trainIdx].pt[1])))
            

    # minKeypoints = 20
    # if len(pts1) < minKeypoints:
    #     raise Exception(f'Need at least {minKeypoints} keypoints!')
    left_img  = cv2.drawKeypoints(left_frame, kp1,  left_frame, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    right_img = cv2.drawKeypoints(right_frame, kp2, right_frame, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

    res = left_frame.shape
    
    hConcatFrame = cv2.hconcat([left_img, right_img])
    vConcatFrame = cv2.vconcat([left_img, right_img])
    print(f'Resolution: {res}')
    for leftPt, rightPt in zip(ptsLeft, ptsRight):
        end_point = (rightPt[0] + res[0], rightPt[1])
        hConcatFrame = cv2.line(hConcatFrame, leftPt, end_point, (255, 0, 0), 1)
        
        end_point = (rightPt[0], rightPt[1] + res[1])
        vConcatFrame = cv2.line(vConcatFrame, leftPt, end_point, (255, 0, 0), 1)
    
    scaleSize = 0.7
    width = int (vConcatFrame.shape[1] * scaleSize)
    height = int(vConcatFrame.shape[0] * scaleSize)
    dim = (width, height)
    print(dim)
    vConcatFrame = cv2.resize(vConcatFrame, dim, interpolation = cv2.INTER_AREA)
    width = int (hConcatFrame.shape[1] * scaleSize)
    height = int(hConcatFrame.shape[0] * scaleSize)
    dim = (width, height)
    hConcatFrame = cv2.resize(hConcatFrame, dim, interpolation = cv2.INTER_AREA)
    cv2.imshow("Vertical combine", vConcatFrame)
    cv2.imshow("horizontal", hConcatFrame)
    
    # img=cv2.drawKeypoints(frame1, kp1, frame1, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # cv2.imshow("Left", img)
    # img2=cv2.drawKeypoints(frame2, kp2, frame2, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    # cv2.imshow("Right", img2)

    # return R_est, t_est, R1, R2, P1, P2, Q




rectifiedLeftDir = str((Path(__file__).parent.parent/ 'wide_data' / 'rectifiedLeft').resolve())
rectifiedRightDir = str((Path(__file__).parent.parent/ 'wide_data' / 'rectifiedRight').resolve())
print(rectifiedLeftDir)
print(rectifiedRightDir)
rectifiedLeftList = glob.glob(rectifiedLeftDir + '/*.png')
rectifiedRightList = glob.glob(rectifiedRightDir + '/*.png')

rectifiedLeftList.sort()
rectifiedRightList.sort()
print(len(rectifiedLeftList))
print(len(rectifiedRightList))


for rectifiedLeft, rectifiedRight in zip(rectifiedLeftList, rectifiedRightList):
    rectifiedLeftImage = cv2.imread(rectifiedLeft, 0)
    rectifiedRightImage = cv2.imread(rectifiedRight, 0)
    visualizeSeteroFeaturesAlign(rectifiedLeftImage, rectifiedRightImage)
    k = cv2.waitKey(0)
    if k == ord('q'):
        break











# print(str(currDir))