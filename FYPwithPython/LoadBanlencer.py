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
from flask_socketio import SocketIO, emit
from flask import Flask, render_template, url_for, copy_current_request_context
from random import random
from time import sleep
from threading import Thread, Event


app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
app.config['DEBUG'] = True

#turn the flask app into a socketio app
socketio = SocketIO(app, async_mode=None, logger=True, engineio_logger=True)

#random number Generator Thread
thread = Thread()
thread_stop_event = Event()

ipdata = ['CamIP1','CamIP2','Cam']
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2"])
server_handler = pyobject.Server_Handeler()

server_handler.add_server('ser1')
server_handler.add_server('ser2')
server_handler.add_server('ser3')

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
        print(r,t)
        gpu_handeler.update_gpu_time(task, t)
        elapse = time.time()
        print ("Task:", task, " GPU:", gpu_handeler.get_gpu(task), " Time:", t, " Result:", r)
        
        if r ==1:
            server_handler.add_task(task)
        else:
            server_handler.remove_task(task)
        server_handler.get_server_loads()
        server_handler.update_server_time(task,t)

        print("Time",elapse - start, random()*100>90,sub_process.pid)
        
        if random()*100>90 and elapse - start > 13:
            # server_handler.remove_task(task)
            start = time.time()
            sub_process.kill()
            subprocess.call("kill -9 "+str(sub_process.pid))
            # print('killing process',sub_process.pid)
            # os.killpg(os.getpgid(sub_process.pid), signal.SIGTERM)
            

            proc = gpu_handeler.get_gpu(task)
            cmd = './server {} {}'.format(task,proc)
            print(cmd)
            sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
            
            # socketio.emit('process', {'camera': task,'weight':proc}, namespace='/test')
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


    inp = input("Press Enter to continue...")

    for t in thread_array :
        t._stop()
    exit()




if __name__ == '__main__':
    pr()

@app.route('/')
def index():
    #only by sending this page first will the client be connected to the socketio instance
    return render_template('index.html')

@socketio.on('connect', namespace='/test')
def test_connect():
    # need visibility of the global thread object
    global thread
    print('Client connected')

    #Start the random number generator thread only if the thread has not been started before.
    if not thread.isAlive():
        print("Starting Thread")
        
        thread_array = []
        # ser.app.run()

        # y =  threading.Thread(target=ser.app.run )
        # y.start()
        # ser.hello()
        for task in ipdata:
            thread = threading.Thread(target=work, args=(task,))
            # thread = socketio.start_background_task(work, task)
            thread_array.append(thread)
            thread.start()
            time.sleep(1)

        
        inp = input("Press Enter to continue...")

        for t in thread_array :
            t._stop()
        exit()



@socketio.on('disconnect', namespace='/test')
def test_disconnect():
    print('Client disconnected')


# if __name__ == '__main__':
# # def rr():
#     socketio.run(app)