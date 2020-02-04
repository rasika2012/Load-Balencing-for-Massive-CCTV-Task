
import subprocess
import time
import pyobject
import threading

ipdata = ['CamIP1','CamIP2','CamIP3','CamIP4']
gpu_handeler = pyobject.GPUHandeler(["GPU1","GPU2","GPU3"])

def split_result(result):
    
    res =  result.split("'")[1].split("\\")[0].split(" ")
    r = int(res[0])
    t = int(res[1])
    return ( r,t)


def work(task):
    cmd = './a.out {} {}'.format(task,gpu_handeler.get_gpu(task))
    print(cmd)
    my_tool_subprocess = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
    line = True

    while line:
        myline = str(my_tool_subprocess.stdout.readline())
        print(task,gpu_handeler.get_gpu(task) ,myline)
        r,t = split_result(myline)
        gpu_handeler.update_gpu_time(task, t)


for task in ipdata:
    x = threading.Thread(target=work, args=(task,))
    x.start()
    time.sleep(1)



