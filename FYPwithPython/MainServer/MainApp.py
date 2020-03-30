import subprocess
import time
import pyobject
import threading
import random
# import ser
import socket
import os
import signal
# Start with a basic flask app webpage.

from flask import Flask
import json
#--
from random import random
from time import sleep
from threading import Thread, Event

app = Flask(__name__)

#random number Generator Thread
thread = Thread()
thread_stop_event = Event()
 
ipdata = ['rtsp://192.168.8.100:8080/h264_ulaw.sdp','rtsp://192.168.8.100:8080/h264_ulaw.sdp ','rtsp://192.168.8.100:8080/h264_ulaw.sdp  ']
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2"])
server_handler = pyobject.Server_Handeler()

server_handler.add_server('ser1')
server_handler.add_server('ser2')
# server_handler.add_server('ser3')

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

def work(task):
    cmd = './server {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
    line = True
    start = time.time()
    elapse = 0
   


    while not thread_stop_event.isSet():
        myline = str(sub_process.stdout.readline())
        r,t = split_result(myline)
        # print(r,t)
        gpu_handeler.update_gpu_time(task, t)
        elapse = time.time()
        # print ("Task:", task, " GPU:", gpu_handeler.get_gpu(task),"Line",myline, " Time:", t, " Result:", r)
        
        if r ==1:
            server_handler.add_task(task)
        else:
            server_handler.remove_task(task)

        server_handler.get_server_loads()
        server_handler.update_server_time(task,t)
        # if server_handler.task_server.get(task):
        #     print("Task:->",server_handler.task_server[task]," : ",server_handler.get_server_load(server_handler.task_server[task]))

        # print("Time",elapse - start, random()*100>90,sub_process.pid)
        # print(server_handler.get_server_loads())
        # if random()*100>90 and elapse - start > 13:
        #     # server_handler.remove_task(task)
        #     start = time.time()
        #     sub_process.kill()
        #     # subprocess.call("kill -9 "+str(sub_process.pid))
        #     # print('killing process',sub_process.pid)
        #     # os.killpg(os.getpgid(sub_process.pid), signal.SIGTERM)
            

        #     proc = gpu_handeler.get_gpu(task)
        #     cmd = './server {} {}'.format(task,proc)
        #     print(cmd)
        #     sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
            
        #     # socketio.emit('process', {'camera': task,'weight':proc}, namespace='/test')
        # #     #
            


    return cmd
    
def pr():
    thread_array = []
    # ser.app.run()

    # y =  threading.Thread(target=ser.app.run )
    # y.start()
    # ser.hello()
    for task in ipdata:
        x = threading.Thread(target=work, args=(task,))
        thread_array.append(x)
        x.start()
        time.sleep(5)


    # inp = input("Press Enter to continue...")

    # for t in thread_array :
    #     t._stop()
    # exit()




@app.route('/load')
def index():
    #only by sending this page first will the client be connected to the socketio instance
    return json.dumps(server_handler.get_server_loads())

@app.route('/ips')
def ips():
    return json.dumps(ipdata)



if __name__ == '__main__':
    pr()
    Flask.run(app,port=9000)

# if __name__ == '__main__':
# # def rr():
#     socketio.run(app)