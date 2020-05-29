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
 
ipdata = ['rtsp://192.168.8.100:8080/h264_ulaw.sdp1','rtsp://192.168.8.100:8080/h264_ulaw.sdp 2','rtsp://192.168.8.100:8080/h264_ulaw.sdp 3 ']
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
    cmd = '../cpp/server {} {}'.format(task,gpu_handeler.get_gpu(task))
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
            print(task)
        else:
            server_handler.remove_task(task)

        ##############
        # print(server_handler.get_server_loads())
    
        #  server_handler.update_server_time(task,t)
        #######################


    return cmd
    
def pr():
    thread_array = []
    for task in ipdata:
        x = threading.Thread(target=work, args=(task,))
        thread_array.append(x)
        x.start()
        time.sleep(5)




@app.route('/load')
def index():
    #only by sending this page first will the client be connected to the socketio instance
    return json.dumps(server_handler.get_server_loads())

@app.route('/subserver/<server>/<time>')
def sub_server(server,time):
    #only by supdate
    print(server,time)
    server_handler.update_server_time_by_server(server,int(time))

    # Remove when muliple servers runinig
    server_handler.update_server_time_by_server('ser2',int(time)+2)
    return json.dumps(server_handler.get_server_loads())

@app.route('/ips')
def ips():
    return json.dumps(ipdata)


@app.route('/timing')
def timing():
    return json.dumps(server_handler.get_server_times())


if __name__ == '__main__':
    pr()
    Flask.run(app,port=9000)
