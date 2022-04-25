from cmath import atan
import cv2
import numpy as np
import time
from ball_detect import *
from matplotlib import pyplot as plt
import math
import triangulation as tri
import calibration
import serial



import time

#640 X 480
#FLIP y BUT NOT X

# Open both cameras
cap_right = cv2.VideoCapture(1, cv2.CAP_DSHOW)                    
cap_left =  cv2.VideoCapture(0, cv2.CAP_DSHOW)


B = 15.0            #Cm distance between cameras
f = 8              #Focal length
alpha = 78.0       #Camera FOV




#TBD
depth_cm_per_pix = 1.71

laser_x_cm = 0.0
laser_y_cm = 0.0

x_max_cm = 35.0
y_max_cm = 35.0


y_max = 479.0

y_pixel_count = 480.0
x_pixel_count = 648.0

last_sent_pitch = 0.0
last_sent_yaw = 90.0
last_sent_abs_x = 0.0
last_sent_depth = 0.0

x_cm_per_pix = x_max_cm / x_pixel_count
y_cm_per_pix = y_max_cm/y_pixel_count

#pitch= up-down
laser_pitch_angle_offset = 0.0
laser_yaw_angle_offset = 90.0
with serial.Serial("COM7", 9600, timeout=1) as arduino:
    with serial.Serial("COM8", 9600, timeout = 1) as arduino_lcd:

        time.sleep(15.0)


        while(cap_right.isOpened() and cap_left.isOpened()):


            success_right, frame_right = cap_right.read()
            success_left, frame_left = cap_left.read()

        ################## CALIBRATION #########################################################

            frame_right, frame_left = calibration.undistortRectify(frame_right, frame_left)
            

        ########################################################################################

            if not success_right or not success_left:                    
                break

            else:

                center_l, radius_l = ball_track(frame_left)

                center_r, radius_r = ball_track(frame_right)



      

                if radius_l is not None and radius_r is not None:

                    depth = tri.find_depth(center_r, center_l, frame_right, frame_left, B, f, alpha)
                    if depth is None:
                        continue
                    # cv2.putText(frame_right, f'{int(depth)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'{int(depth)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_right, "Distance: " + str(round(depth,1)), (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0),3)
                    # cv2.putText(frame_left, "Distance: " + str(round(depth,1)), (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0),3)

                    
                    #Approx 1:1 ratio
                    depth_cm = depth * depth_cm_per_pix
                    x_max_cms = 1.0 * depth_cm + 4.0
                    y_max_cms = 1.0 * depth_cm + 4.0

                    x_cm_per_pix = x_max_cm / x_pixel_count
                    y_cm_per_pix = y_max_cm/y_pixel_count

                    angle_x, angle_y = center_l
                    angle_y = y_max - angle_y #flip y coordinates so zero is on bottom

                    laser_x_cm = 0.3 * depth_cm + 6.31

                    laser_y_cm = 0.0801 * depth_cm + 1.45

                    laser_y = laser_y_cm * 1.0 / y_cm_per_pix
                    laser_x = laser_x_cm * 1.0 / x_cm_per_pix

                    #offset for laser

                    offset_y = angle_y - laser_y
                    offset_x = angle_x - laser_x


                    angle_y_cm = offset_y * y_cm_per_pix
                    angle_x_cm = offset_x * x_cm_per_pix

                    absolute_x_cm = angle_x * x_cm_per_pix
                    


                    #tan(yaw) = x / depth, left-right
                    #tan(pitch) = y / (x^2 + depth^2), up-down
                    yaw_sign = math.copysign(1, angle_x_cm)
                    pitch_sign = math.copysign(1, angle_y_cm)
                    yaw_deg = math.degrees(math.atan(angle_x_cm / depth_cm))  * -1.0
                    pitch_deg = math.degrees(math.atan(angle_y_cm / math.sqrt(angle_x_cm ** 2 + depth_cm ** 2)))


                    relative_yaw = int(yaw_deg + laser_yaw_angle_offset)
                    relative_pitch = int(pitch_deg + laser_pitch_angle_offset)

                    # cv2.putText(frame_right, f'{int(angle_x_cm)},{int(angle_y_cm)},{int(depth_cm)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'{int(angle_x_cm)},{int(angle_y_cm)},{int(depth_cm)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)

                    # cv2.putText(frame_right, f'{int(angle_x)},{int(angle_y)},{int(depth)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'{int(angle_x)},{int(angle_y)},{int(depth)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)

                
                    # cv2.putText(frame_right, f'p/y: {int(signed_pitch)}, {int(signed_yaw)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'p/y: {int(signed_pitch)}, {int(signed_yaw)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)

                
                    # cv2.putText(frame_right, f'y/p: {int(yaw_deg)},{int(pitch_deg)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'y/p: {int(yaw_deg)},{int(pitch_deg)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)

                    if arduino.isOpen():
                        pitch_send = (f"P{int(relative_pitch)}\n").encode()
                        yaw_send = (f"Y{int(relative_yaw)}\n").encode()
                   
                        
                        if abs(last_sent_pitch - relative_pitch) >= 5.0 or abs(last_sent_yaw - relative_yaw) >= 2.0:
                            arduino.write(pitch_send)
                            arduino.write(yaw_send)   


                            last_sent_pitch = relative_pitch
                            last_sent_yaw = relative_yaw



                    if arduino_lcd.isOpen():

                        abs_x_send = (f"X{int(absolute_x_cm)}\n").encode()
                        depth_send = (f"D{int(depth_cm)}\n").encode()

                        if abs(last_sent_abs_x - absolute_x_cm >= 3.0) or abs(last_sent_depth - depth_cm) >= 3.0:
                            arduino_lcd.write(abs_x_send)
                            arduino_lcd.write(depth_send)
                            last_sent_abs_x = int(absolute_x_cm)
                            last_sent_depth = int(depth_cm)


                    cv2.putText(frame_right, f'p/y: {int(relative_pitch)}, {int(relative_yaw)}, {int(absolute_x_cm)},{int(depth_cm)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    cv2.putText(frame_left, f'p/y: {int(relative_pitch)}, {int(relative_yaw)}, {int(absolute_x_cm)},{int(depth_cm)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)

                    # cv2.putText(frame_right, f'p/y: {last_sent_pitch}, {int(last_sent_yaw)}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)
                    # cv2.putText(frame_left, f'p/y: {last_sent_pitch}, {last_sent_yaw}', (20,450), cv2.FONT_HERSHEY_SIMPLEX, 1.2, (0,255,0), 2)



                # Show the frames
                cv2.imshow("frame right", frame_right) 
                cv2.imshow("frame left", frame_left)


                # Hit "q" to close the window
                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
                
cap_right.release()
cap_left.release()

cv2.destroyAllWindows()