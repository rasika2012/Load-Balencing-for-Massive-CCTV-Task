
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
    
       
### Now you successfully configured the project

 - Run ``` cmake ``` and ```make``` on cpp cpp project and copy server program to python programs
 - run ```python3 MainApp.py``` and then run the ```python3  SubApp.py``` on main and sub servers
 
### Run Process Viewer

 - Run ``` npm i ``` on ```viewer folder``` on=
 - run ```npm start``` and provide ```main server ip```
 
