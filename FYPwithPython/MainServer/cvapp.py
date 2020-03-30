import cv2
import numpy as np
import time


cap = cv2.VideoCapture('rtsp://192.168.1.101:8080/h264_ulaw.sdp')
ret, premat = cap.read()
grpremat = cv2.cvtColor(premat, cv2.COLOR_BGR2GRAY)
while(True):
    ret, frame = cap.read()
    
    t1 = time.time()
    grframe = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    img = grpremat - grframe
    cv2.imshow('frame',img)
    t2 = time.time()

    if 300 < (sum(sum((img)))):
        print( '1' + ' ' +str(int((t2-t1)*10000)))
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    grpremat = grframe

cap.release()
cv2.destroyAllWindows()