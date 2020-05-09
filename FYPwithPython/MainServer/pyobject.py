from queue import Queue


class FQueue:
    MaxDelay = 0
    MinDelay = 0
    
    def __init__(self):
        self.q = [0]
        self.maxlen = 10
        self.q = self.q*self.maxlen
        self.i = -1
        self.total =0

    def add(self,val):
        self.i +=1
        # print(self.i)
        if self.i >= self.maxlen :
            self.i = 0
        self.total-=self.q[self.i]
        self.q[self.i] = val
        self.total += val
        
  
    def sum(self):
        return self.total


class GPUHandeler:

    def __init__(self,gpu_list):
        self.gpu_list  = {}
        self.task_gpu  = {}
        self.task_need_to_reset  = {}
        self.task_list = {}
        
        for gpu in gpu_list:
            self.gpu_list[gpu] = 0

    def clean_task(self,task):
        del (self.task_gpu[task])

    

    def get_gpu(self, task_ip):
        if self.task_gpu.get(task_ip) :
            return self.task_gpu[task_ip]

        gpu_min = ""
        gpu_min_time = 9223372036854775807
        for i  in self.gpu_list.keys():
            if self.gpu_list[i] == 0 or self.gpu_list[i] < gpu_min_time:
                gpu_min = i 
                gpu_min_time = self.gpu_list[i]

        self.task_gpu[task_ip] = gpu_min
       
        print(self.task_gpu)
        return gpu_min

    def update_gpu_time(self, task, new_time):
        
        gpu = self.task_gpu[task] 
        if gpu in self.gpu_list:
            self.gpu_list[gpu] = (self.gpu_list[gpu]/2) + (new_time/2)
            # print(self.gpu_list)
           
        else:
            pass
            print ("GPU Not Found:" + gpu +" | ")
          

class Server_Handeler:
    def __init__(self):
        self.server_task = {}
        self.task_server = {}
        self.servers = set()
        self.server_time={}
        

    def add_server(self, url):
        self.servers.add(url)
        self.server_task[url] = set()
        self.server_time[url] = {"time":0, "count": 0}
    
    def remove_server(self, url):
        self.servers.remove(url)
        tasks = self.server_task[url].copy()
        self.server_task.pop(url)

        for task in tasks[url]:
            self.remove_task(task)
            self.add_task(task)
        return server

    def remove_task(self, task):
        if self.task_server.get(task):
            server = self.task_server[task]
            del(self.task_server[task])
            self.server_task[server].remove(task)
            self.server_time[server]['count'] -= 1

            return task
        return 0

    def add_task(self, task):
        selected_server = ""
        min_time = 113034557360
        if self.task_server.get(task):
            return 0
        for server in self.servers:
            if  self.server_time[server]['count'] ==0:
                min_time = self.server_time[server]['time']
                selected_server = server
                break
            if (self.server_time[server].get('time')):
                if min_time > self.server_time[server]['time']:
                    selected_server = server
                    min_time = self.server_time[server]['time']

        self.task_server[task] = server
        self.server_time[server]['count'] += 1
        # print('task added:',self.task_server, 'min_time: ', min_time, 'count:', self.server_time[server]['count'] )

        if not self.server_task.get(server):
            self.server_task[server]=set()
        self.server_task[server].add(task)

    def get_server_load(self,server):
        return self.server_task[server]

    def update_server_time(self,task,time):
        if self.task_server.get(task):
            server = self.task_server[task]
            self.server_time[server]['time'] = time

    def get_server_loads(self):
        # print(self.server_task)
        # print(self.server_time)
        print(self.server_task)
        map = {}
        for i in self.server_task.keys():
            map[i] = list(self.server_task[i])

        print(map)
        return map


    

    

        
