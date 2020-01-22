
#include "opencv2/opencv.hpp"
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h> 
#include <string.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <queue> 

void *display(void *);
char **args;
using namespace std;
using namespace cv;
queue <Mat> resived_data;
queue <Mat> to_send_data;

///gloal variables
    char * IP;
    int rmport,srvport;
//

void clean_q(){
    while (resived_data.size() > 100 ){
        resived_data.pop();
        cout<<"cleaned\n";
    }

    while (to_send_data.size() > 100 ){
        to_send_data.pop();
        cout<<"cleaned\n";
    }
}



void * connectCamera(void * inp){
    char * serverIP = IP;
    int serverPort = rmport;

    int         sokt;
    struct  sockaddr_in serverAddr;
    socklen_t           addrLen = sizeof(struct sockaddr_in);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);
    cout<<"camera connected "<<IP<<":"<<serverPort;

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
        std::cerr << "connect() failed!" << std::endl;
    }
    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes = 0;
    int key;
    if ( ! img.isContinuous() ) { 
          img = img.clone();
    }     
    std::cout << "Image Size:" << imgSize << std::endl;

    while (key != 'q') {

        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
        }
        resived_data.push(img.clone());
        // tmp add
       

        //putText(img, "Text", Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
        
       

        // to_send_data.push(resived_data.front());
        // resived_data.pop();
        clean_q();
        if (key = cv::waitKey(10) >= 0) break;
    }   

    close(sokt);
        
}

void * connectDisplay(void * ptr){
    int socket = *(int *)ptr;
    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);   

    if (!img.isContinuous()) {
        img = img.clone();
    }

    int imgSize = img.total() * img.elemSize();
    int bytes = 0;
    int key;
    

    //make img continuos
    if ( ! img.isContinuous() ) { 
          img = img.clone();
    }
        
    std::cout << "Image Size:" << imgSize << std::endl;
    while(1) {
                
            /* get a frame from camera */
                if (to_send_data.size()>0){
                    img = to_send_data.front();
                    to_send_data.pop();
                }
                    
                cv::resize(img, img, cv::Size(640, 480),CV_8UC3);
                
                //do video processing here 
                //cvtColor(img, imgGray, CV_BGR2GRAY);
                //send processed image
                if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
                     std::cerr << "bytes = " << bytes << std::endl;
                     break;
                } 

   
    }

}

void * algorithm(void * prt){
    while (1)

    {
         clean_q();
         cout<<resived_data.size()<<"recieved \n";
         cout<<to_send_data.size()<<" tosend\n";
        /* code */
    
    
   
    if(resived_data.size() >0){
         Mat img = resived_data.front();
        resived_data.pop();

          putText(img, "Text", Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
        
         to_send_data.push(img);
        // to_send_data.push(resived_data.front());
        // resived_data.pop();
       
   }
   }
        
}

void * serverUp(void * prt){
    int localSocket, remoteSocket;                               
    struct  sockaddr_in localAddr, remoteAddr;
    pthread_t thread_id;
    int port = srvport;
           
    int addrLen = sizeof(struct sockaddr_in);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );
    cout<<"server :"<<port;
    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }
    

    listen(localSocket , 3);
    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    
    while(1){
   
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
    
        if (remoteSocket < 0) {
            perror("accept failed!");
            exit(1);
        } 
        std::cout << "Connection accepted" << std::endl;
        pthread_create(&thread_id,NULL,connectDisplay,&remoteSocket);
    }   
}



int main(int argc, char** argv){   
    string command;
    if ( argc <4){
        perror("arguments not found");
    }

    cout<<"Console Started\n";
    
    char * IPl = "0.tcp.ngrok.io";
    IP = IPl;
    rmport = 13210;
    srvport = 5001;

    rmport = atoi(argv[2]);
    srvport = atoi(argv[3]);
    IP = argv[1]; 


        if (!cv::ocl::haveOpenCL())
{
    cout << "OpenCL is not available..." << endl;
    //return;
}

cv::ocl::Context context;

if (!context.create(cv::ocl::Device::TYPE_GPU))
{
    cout << "Failed creating the context..." << endl;
    //return;
}

cout << context.ndevices() << " GPU devices are detected." << endl; //This bit provides an overview of the OpenCL devices you have in your computer
for (int i = 0; i < context.ndevices(); i++)
{
    cv::ocl::Device device = context.device(i);
    cout << "name:              " << device.name() << endl;
    cout << "available:         " << device.available() << endl;
    cout << "imageSupport:      " << device.imageSupport() << endl;
    cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << endl;
    cout << endl;
}

cv::ocl::Device(context.device(0));

    
     cin >> command;
     
        pthread_t t;
        pthread_t p;
        pthread_t alg;

        cout<<"\nQ Size:"<<resived_data.size()<<"\n>";
        pthread_create(&t,NULL,connectCamera,&t);
        pthread_create(&p,NULL,serverUp,&p);
        pthread_create(&alg,NULL,algorithm,&alg);

        cin >> command;
    
    return 0;
}
