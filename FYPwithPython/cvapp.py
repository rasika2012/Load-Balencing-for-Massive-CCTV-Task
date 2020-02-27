import cv2
import numpy as np
import time


cap = cv2.VideoCapture('rtsp://192.168.8.100:8080/h264_ulaw.sdp')
ret, premat = cap.read()
while(True):
    ret, frame = cap.read()
    t1 = time.time()
    img = premat - frame
    cv2.imshow('frame',img)
    t2 = time.time()

    if 300 < (sum(sum(sum(img)))):
        print '1' + ' ' +str(int((t2-t1)*10000))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    premat = frame

cap.release()
cv2.destroyAllWindows()