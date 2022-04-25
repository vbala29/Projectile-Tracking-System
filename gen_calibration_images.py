import cv2
import time


cap_l = cv2.VideoCapture(0)
time.sleep(2.0)
cap_r = cv2.VideoCapture(1)
time.sleep(2.0)

num = 0


while cap_l.isOpened():

    succes_l, img_l = cap_l.read()
    succes_r, img_r = cap_r.read()


    k = cv2.waitKey(5)

    if k == 27:
        break
    elif k == ord('s'):
        cv2.imwrite('images/stereoLeft/imageL' + str(num) + '.png', img_l)
        cv2.imwrite('images/stereoright/imageR' + str(num) + '.png', img_r)
        print("images saved!")
        num += 1

    cv2.imshow('Img left',img_l)
    cv2.imshow('Img right',img_r)