# Load-Balancing-for-Massive-CCTV-Task
Final Year Project - 2019 Department of Computer Engineering, University of Peradeniya

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
    
### Project Configurations on /cpp/server.cpp

 - For single ip camera
     ```
   39 VideoCapture vcap("<ip>:<port>");
    ```
 - For many ip cameras
      ```
   39 // VideoCapture vcap("<ip>:<port>"); or delete this line
    ``` 
   
    
  
### Project Configurations on /MainServer/MainApp.py

 - Main App controll the main filter program and sub servers insert sub server infomation
     ```
    ipdata = [<ip camera ips/ if one camera dummy ips]
    gpu_handeler = pyobject.GPUHandeler([<GPU IDs to selectt GPU for sub program>])
    server_handler = pyobject.Server_Handeler()

    server_handler.add_server(<server-id1>)
    server_handler.add_server(<server-id2>) # add many servers as you want and config this nem on sub server program
    ```           
    
### Project Configurations on /SubServer/SubApp.py

 - Main App controll the main filter program and sub servers insert sub server infomation
     ```
    15:gpu_handeler = pyobject.GPUHandeler([<GPU Ids>])
    17:r = requests.get("<Main Server IP>/ips")
    19:server_name = <Sub Server id userd in Main Server Configuration>
    ```           
    - replace ``` localhost:9000``` with server ip and port
    
       
### Now you successfully configured the project

 - Run ``` cmake ``` and ```make``` on cpp cpp project and copy server program to python programs
 - run ```mainApp.py``` and then run the ```SubApp.py
 
### Run Process Viewer

 - Run ``` npm i ``` on ```viewer folder``` on=
 - run ```npm start``` and provide ```main server ip```
 

