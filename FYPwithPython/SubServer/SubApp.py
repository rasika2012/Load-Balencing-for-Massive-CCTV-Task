import requests
import json
import time
import subprocess
import time
import pyobject
import threading
import random
# import ser
import socket
import os
import signal

ips = []
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2"])

r = requests.get("http://localhost:9000/ips")
print("IP info taken\n", r.content)
ips =  json.loads(r.content)

## Start Sub process programes
def work(task):
    cmd = './server {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
    line = True
    start = time.time()
    elapse = 0
   


    while True:
        myline = str(sub_process.stdout.readline())
        print(myline)



thread_array = []
for task in ips:
    x = threading.Thread(target=work, args=(task,))
    thread_array.append(x)
    x.start()
    time.sleep(5)

while True:
    r = requests.get("http://localhost:9000/load")
    print (r.content)
    time.sleep(0.1)

inp = input("Press Enter to continue...")

for t in thread_array :
    t._stop()
exit()