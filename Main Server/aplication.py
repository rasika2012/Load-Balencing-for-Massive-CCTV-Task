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
import base64
from flask import Flask
from flask import request
import json
import csv
from flask_cors import CORS
from datetime import date
#--
from random import random
from time import sleep
from threading import Thread, Event

app = Flask(__name__)
CORS(app)


#random number Generator Thread
thread = Thread()
thread_stop_event = Event()
results = {}
ipdata = []
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2"])
server_handler = pyobject.Server_Handeler()

# server_handler.add_server('ser1')
# server_handler.add_server('ser2')
# server_handler.add_server('ser3')

 # Load Saved Configurations
conf = open("./config.json","r") 
lines = json.loads(str(conf.read()))
ipdata = (lines['configs']['ips'])
print(ipdata)
for server in lines['configs']['servers']:
    server_handler.add_server(server)
    print("ADD SERVER:",server)
gpu_handeler = pyobject.GPUHandeler(lines['configs']['gpus'])


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
    cmd = '../Filter/server {} {}'.format(task,gpu_handeler.get_gpu(task))
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
            # server_handler.remove_task(task)
            server_handler.add_task(task)
            print(task)
        else:
            server_handler.remove_task(task)

        ##############
        # print(server_handler.get_server_loads())
    
        #  server_handler.update_server_time(task,t)
        #######################


    return cmd
    



def programe():
   

    print(ipdata)
    thread_array = []
    for task in ipdata:
        x = threading.Thread(target=work, args=(task,))
        thread_array.append(x)
        x.start()
        time.sleep(1)




@app.route('/load')
def index():
    #only by sending this page first will the client be connected to the socketio instance
    return json.dumps(server_handler.get_server_loads())

@app.route('/subserver/<server>/<time>/<task>/<result>')
def sub_server(server,time,task,result):
    #only by supdate
    # print("---------",server,time, base64.urlsafe_b64decode(task.encode('ascii')).decode(),result)
    decoded_task = base64.urlsafe_b64decode(task.encode('ascii')).decode()
    results[decoded_task] = int(result)
    server_handler.update_server_time_by_server(server,int(time))

    # Remove when muliple servers runinig
    # server_handler.update_server_time_by_server('ser2',int(time)+2)
    return json.dumps(server_handler.get_server_loads())

@app.route('/ips')
def ips():
    return json.dumps(ipdata)

@app.route('/results')
def get_results():
    return json.dumps(results)

@app.route('/timing')
def timing():
    return json.dumps(server_handler.get_server_times())

@app.route('/data', methods=['GET'])
def readFile():
    wholeFile = []
    with open('DataFile.csv','r') as csv_file: #Opens the file in read mode
        csv_reader = csv.reader(csv_file) # Making use of reader method for reading the file
    
    
        for line in csv_reader: #Iterate through the loop to read line by line
            wholeFile.append(line)
            
    return json.dumps(wholeFile)


@app.route('/submitData', methods=['POST'])
def submit():
    print('checked')
    # language = request.args['ip']
    content = request.json
    id = 'CAM0' + str(content['id'])
    ip = content['ip']
    comments = content['comments']
    add = date.today()

    with open('DataFile.csv', 'a+', newline='') as file:
        writer = csv.writer(file)
        writer.writerow([id, ip, add, 'running', comments])

    # print (language['ip'])
    return json.dumps('true')



if __name__ == '__main__':
    programe()
    Flask.run(app,port=9000)
