import cv2
import imutils
import time
import math
lower_bound = (20, 100, 100)
upper_bound = (40, 255, 255)


def ball_track(frame):
    if frame is None:
        return None

    blurred = cv2.GaussianBlur(frame, (11, 11), 0)
    width, height = frame.shape[:2]
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, lower_bound, upper_bound)
    mask_erode = cv2.erode(mask, None, iterations=2) 
    mask_dilate = cv2.dilate(mask, None, iterations=2) # check this, was on originally mask
    cnts = cv2.findContours(mask_dilate.copy(), cv2.RETR_EXTERNAL,
                            cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    center = None
    radius = None

    if len(cnts) > 0:
        c = max(cnts, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)
        center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))
        

        if radius > 10:
            cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 5)
            cv2.imwrite("circled_frame.png", cv2.resize(frame, (int(height / 2), int(width / 2))))
            cv2.circle(frame, center, 5, (0, 0, 255), -1)

    return center, radius

def test_2d_cap():
    vs = cv2.VideoCapture(0)
    time.sleep(2.0)

    while True:
        _, frame = vs.read()

        if frame is None:
            break

        blurred = cv2.GaussianBlur(frame, (11, 11), 0)
        width, height = frame.shape[:2]
        hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
        mask = cv2.inRange(hsv, lower_bound, upper_bound)
        mask_erode = cv2.erode(mask, None, iterations=2)
        mask_dilate = cv2.dilate(mask, None, iterations=2)
        cnts = cv2.findContours(mask_dilate.copy(), cv2.RETR_EXTERNAL,
                                cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        center = None

        if len(cnts) > 0:
            c = max(cnts, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            M = cv2.moments(c)
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))



            if radius > 10:
                cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 5)

                cv2.circle(frame, center, 5, (0, 0, 255), -1)

        cv2.imshow("Frame", frame)
        cv2.imshow("mask", mask)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    vs.release()
    cv2.destroyAllWindows()

