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

server_name = 'ser1'
server_time = 10

print("IP info taken\n", r.content)
ips =  json.loads(r.content)

## Start Sub process programes
def work(task):
    cmd = './server {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE,stdin=subprocess.PIPE)
    line = True
    start = time.time()
    elapse = 0
   


    while True:
        myline = str(sub_process.stdout.readline())
        # sub_process.stdin.writelines('4')
        print('myLine',myline)
        server_time = random.randint(100,300)
        # print(server_time)
        r = requests.get("http://localhost:9000/subserver/"+server_name+'/'+str(server_time))
        



thread_array = []
for task in ips:
    x = threading.Thread(target=work, args=(task,))
    thread_array.append(x)
    x.start()
    time.sleep(5)

while True:
    r = requests.get("http://localhost:9000/load/")
    print ('content',r.content)
    time.sleep(0.1)

inp = input("Press Enter to continue...")

for t in thread_array :
    t._stop()
exit()