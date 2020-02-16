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
        print(self.i)
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
          