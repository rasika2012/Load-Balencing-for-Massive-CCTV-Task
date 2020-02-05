
import subprocess
import time
import pyobject
import threading
import random





ipdata = ['CamIP1','CamIP2','CamIP3','CamIP4']
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

    while line:
        myline = str(sub_process.stdout.readline())
        # print(task,gpu_handeler.get_gpu(task) ,myline)
        r,t = split_result(myline)
        gpu_handeler.update_gpu_time(task, t)
        if random.randrange(100) > 98 :
            sub_process.kill()
            cmd = './a.out {} {}'.format(task,gpu_handeler.get_gpu(task))
            print(cmd)
            sub_process = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
           
   
    

thread_array = []

for task in ipdata:
    x = threading.Thread(target=work, args=(task,))
    thread_array.append(x)
    x.start()
    time.sleep(1)



inp = input("Press Enter to continue...")

for t in thread_array :
    t._stop()
exit()