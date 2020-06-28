
# Load-Balancing-for-Massive-CCTV-Task
 2019 Department of Computer Engineering, University of Peradeniya

> In the surveillance systems, the CCTVs are mostly used for security purposes and also other monitoring purposes. With expanding the number of cameras connected and the objectives the CCTV cameras are used, increase the complexity of the overall system. When real-time decisions are demanded, then the system is going to be more complicated. The manual monitoring might not be efficient with the involved systems.
The ultimate purpose of the idea is to reduce human involvement in the system and make the complete system automated. The research is leading to handling the CCTV cameras and developing a central server with a load-balancing algorithm. The multiple servers are connected to the central server as the sub servers which are capable of handling CCTV live feeds and operate algorithms on them. In the overall process, decisive factors such as networking, servers’ capabilities, CCTVs feed qualities should be considered. To improve the efficiency of the complete system, the algorithms that are performing on GPUs are used. Briefly, the research is experimenting on a distributed system on CCTVs and developing an efficient system that renders real-time outputs.

# Load Balance and Load Management tool form CCTV Network!

Undergrads project for massive size CCTV Load management. This project can used to distribute load between multiple servers and GPU Cards.


# Files

###  Wrapper
Wrapper program is  the algorithm management which has 2 types. 
* Filter Wrapper (Wrapper program for filter program)
* HDA Wrapper (Wrapper Program for High End Algorithm)

## Filter Program
Filter program is simple algorithm that filter unwanted load from feeds.



# How to Run

### Run Project

Please follow following steps for to run project.
1. install opencv cuda/without cuda. [cuda installation help](https://medium.com/@bnarasapur/compile-opencv-with-cuda-from-the-source-1b98e9108a59)
2.  setup IP cameras to local network/ use [IP Camera simulator](https://play.google.com/store/apps/details?id=com.pas.webcam&hl=en) 
3.  Configure following files on cleaned folder
    
### Project Configurations on /Main Server/config.json

 - Edit this file to add new cameras and new Sub Servers
     
   
    ```
	{
		"configs": {
			"ips": ["camera1:ip","camera2:ip"...],
			"servers": ["sub_server_name1","sub_server_name2"..],
			"gpus": ["gpuid1", "gpuid2"..]
		}
	}
    ``` 
   
    
  
### Project Configurations on /Sub Server/config.json

 - edit following pram to update configurations
     ```
	{
		"configs": {
			"name": "server name",
			"ip":"Main Server IP :Port",
			"gpus": ["gpuid1", "gpuid2"..]
		}
	}
    ```           
    
       
``` Now you successfully configured the project```

## Make C++ Filter programe and HDA Programe
Configure ```CMakeLists.txt``` and replace opencv locations of the computer that you installed.
default path is ``` /home/ {USER_NAME} /opencv/build``` and replace your user name.
```
7   project( server )
8   find_package( OpenCV REQUIRED PATHS "/home/ {USER_NAME} /opencv/build")
9   add_executable(server server.cpp)
```
 - Run ``` cmake ``` and ```make``` on cpp cpp project and copy server program to python programs
 
## Run Program
 - run ```python3 Main Server/application.py``` and then run the ```python3  Sub Server/application.py``` on main and sub servers.
 - if you are followed the process you should see all cameras on sub server screen and some of them are sleep.
 
### Run Process Viewer

 - Run ``` npm i ``` on ```React``` folder
 - run ```npm start``` and provide ```main server ip``` to get info from server
 
