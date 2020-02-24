
import subprocess
import time
import pyobject
import threading
import random
# import ser
import socket

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

thread = Thread()
thread_stop_event = Event()

ipdata = ['CamIP1','CamIP2','CamIP3','CamIP4','CamIP5','CamIP6','CamIP7','CamIP8']
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2","GPU3"])

def split_result(result):
 
    try:
        res = result.split("'")[1].replace("\\n",'').split(' ')
        r = int(res[0])
        t = int(res[1])
        return(r,t)
    except:
        return ( 0,0)


def work(task):
    
    cmd = './a.out {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
    line = True
    start = time.time()
    elapse = 0

    while not thread_stop_event.isSet():
        myline = str(sub_process.stdout.readline())
        # print(task,gpu_handeler.get_gpu(task) ,myline)
        r,t = split_result(myline)
        val = gpu_handeler.update_gpu_time(task, t)
        elapse = time.time()
    

        if elapse - start > 5:
            start = time.time()
            sub_process.kill()
            print('killing process')

            proc = gpu_handeler.get_gpu(task)
            cmd = './a.out {} {}'.format(task,proc)
            print(cmd)
            sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)

            socketio.emit('process', {'v':val}, namespace='/test')

            


    return cmd


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


if __name__ == '__main__':
# def rr():
    socketio.run(app)
