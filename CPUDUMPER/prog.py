from __future__ import print_function
import psutil
import time

outF = open("cpu_dump.txt", "w")
print(str(time.gmtime().tm_hour) , ":", str(time.gmtime().tm_min),":",str(time.gmtime().tm_sec))
while True:
    print(psutil.cpu_percent(interval=1, percpu=True)) 
    
    outF.write(str(time.gmtime().tm_hour) + ":"+ str(time.gmtime().tm_min)+":"+str(time.gmtime().tm_sec))
    outF.write(", ")

    for line in psutil.cpu_percent(interval=1, percpu=True):
    # write line to output file
        outF.write(str(line))
        outF.write(", ")
    outF.write("\n")
outF.close()
