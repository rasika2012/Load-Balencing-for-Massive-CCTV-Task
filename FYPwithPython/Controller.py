from multiprocessing.pool import ThreadPool
import subprocess
import time
import pyobject


timeLaps = {}
ipdata = [['CamIP1','GPU1'],['CamIP2','GPU1'],['CamIP3','GPU2'],['CamIP4','GPU2']]

def work(arg):
    cmd = './a.out {} {}'.format(arg[0],arg[1])
    my_tool_subprocess = subprocess.Popen(cmd,shell=True, stdout=subprocess.PIPE)
    line = True
    start = time.time()
    
    q = pyobject.FQueue()
    while line:
        end = time.time()
        
        q.add(end-start)
        timeLaps[arg[0]] = q.sum()
        print(timeLaps)
        
        start = end
        myline = my_tool_subprocess.stdout.readline()


num = 4  # set to the number of workers you want (it defaults to the cpu count of your machine)
tp = ThreadPool(num)


for iparg in ipdata:
    tp.apply_async(work, (iparg,))

# while(1):
#      print("Main Thread:")


tp.close()
tp.join()