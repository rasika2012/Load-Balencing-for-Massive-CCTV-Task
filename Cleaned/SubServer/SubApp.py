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
server_time = 0
global count


print("IP info taken\n", r.content)
ips =  json.loads(r.content)
server_tasks = {}

def split_result(result):
    # res=(result.replace("\\n",'').split("'")[1].replace("\\n",'').split(' '))
    # r = res[0]
    # t = res[2]
    # print("rt",res)
    try:
        res = result.replace("\\n",'').split("'")[1].replace("\\n",'').split(' ')
        r = int(res[0])
        t = int(res[2])
        
        return(r,t)
    except:
        return ( 0,0)

## Start Sub process programes
def work(task):
    cmd = '../subapp/server {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE,stdin=subprocess.PIPE)
    line = True
    start = time.time()
    elapse = 0
   
    myline = ""

    while True:
        myline = str(sub_process.stdout.readline())
        sub_process.stdout.flush()
        
        result,t = (split_result(myline))
        requests.get("http://localhost:9000/subserver/"+server_name+'/'+str(t))

        try:
            
            if (task not in server_tasks[server_name]):
                os.kill(sub_process.pid +1 , signal.SIGSTOP)
                print("PAUSED")
            
                print("RESTARTED")
               
                while (task not in server_tasks[server_name]):
                    time.sleep(0.1)
                os.kill(sub_process.pid +1 , signal.SIGCONT)
            else:
                os.kill(sub_process.pid +1 , signal.SIGCONT)
                print(server_tasks[server_name],task)
                print("CONT",sub_process.pid)
        except:
            print("failed slept for ip updates")
            try:
                os.kill(sub_process.pid +1, signal.SIGSTOP)
                while not server_tasks.get(server_name):
                    time.sleep(0.1)

                os.kill(sub_process.pid +1, signal.SIGCONT)
            except:
                pass       
        
       
        
    


thread_array = []
for task in ips:
    x = threading.Thread(target=work, args=(task,))
    thread_array.append(x)
    x.start()
    time.sleep(2)

while True:
    r = requests.get("http://localhost:9000/load")
    # print ('content',json.loads(str(r.content).split("'")[1]))
    server_tasks = json.loads(str(r.content).split("'")[1])
    time.sleep(0.1)

inp = input("Press Enter to continue...")

for t in thread_array :
    t._stop()
exit()