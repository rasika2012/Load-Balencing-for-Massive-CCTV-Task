/**
 * OpenCV video streaming over TCP/IP
 * Server: Captures video from a webcam and send it to a client
 * by Isaac Maia
 */

#include "opencv2/opencv.hpp"
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
using namespace cv;
using namespace std;
static int weght = 20*40;
static int hight = 20*30;
void *display(void *);
int nuofClient = 0;
int capDev = 0;

//    VideoCapture cap("rtsp://192.168.8.101:8080/h264_ulaw.sdp"); // open the default camera "CCTV.mp4"

    
int main(int argc, char** argv)
{   

    //--------------------------------------------------------
    //networking stuff: socket, bind, listen
    //--------------------------------------------------------
    int                 localSocket,
                        remoteSocket,
                        port = 4090;                               

    struct  sockaddr_in localAddr,
                        remoteAddr;
    pthread_t thread_id;
    
           
    int addrLen = sizeof(struct sockaddr_in);

       
    if ( (argc > 1) && (strcmp(argv[1],"-h") == 0) ) {
          std::cerr << "usage: ./cv_video_srv [port] [capture device]\n" <<
                       "port           : socket port (4097 default)\n" <<
                       "capture device : (0 default)\n" << std::endl;

          exit(1);
    }

    if (argc == 2) port = atoi(argv[1]);

    localSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (localSocket == -1){
         perror("socket() call failed!!");
    }    

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons( port );

    if( bind(localSocket,(struct sockaddr *)&localAddr , sizeof(localAddr)) < 0) {
         perror("Can't bind() socket");
         exit(1);
    }
    
    //Listening
    listen(localSocket , 3);
    
    std::cout <<  "Waiting for connections...\n"
              <<  "Server Port:" << port << std::endl;

    //accept connection from an incoming client
    while(1){
    //if (remoteSocket < 0) {
    //    perror("accept failed!");
    //    exit(1);
    //}
       
     remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);  
      //std::cout << remoteSocket<< "32"<< std::endl;
    if (remoteSocket < 0) {
        perror("accept failed!");
        exit(1);
    } 
    std::cout << "Connection accepted" << std::endl;
    pthread_create(&thread_id,NULL,display,&remoteSocket);

     //pthread_join(thread_id,NULL);

    }
    //pthread_join(thread_id,NULL);
    //close(remoteSocket);

    return 0;
}

void *display(void *ptr){
    VideoCapture cap("CCTV.mp4");

    nuofClient = nuofClient + 1;
    int socket = *(int *)ptr;
    //OpenCV Code
    //----------------------------------------------------------

    Mat img, imgGray;
    img = Mat::zeros(hight, weght, CV_8UC3);
     //make it continuous
    if (!img.isContinuous()) {
        img = img.clone();
    }

    int imgSize = img.total() * img.elemSize();
    int bytes = 0;
    int key;
    

    //make img continuos
    if ( ! img.isContinuous() ) { 
          img = img.clone();
          imgGray = img.clone();
    }

    unsigned int microseconds = 30000;

    std::cout << "Image Size:" << imgSize << std::endl;
    while(1) {
                
            /* get a frame from camera */
//        [1280 x 720]
//        [640 x 480]
        cap >> img;
       

        if(img.empty()){
                    cout << " clip end\n" << endl;
                    nuofClient = nuofClient - 1;
                    close(socket);
                    break;
                }
                cv::resize(img, img, cv::Size(weght, hight),CV_8UC3);
                // cout << img.size() << endl;
                //do video processing here 
                //cvtColor(img, imgGray, CV_BGR2GRAY);
                //send processed image
                clock_t begin_time = clock();
                if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
                    nuofClient = nuofClient - 1;
                     std::cerr << "bytes = " << bytes << std::endl;
                     break;
                }
                if ((bytes = recv(socket, img.data, imgSize , MSG_WAITALL)) == -1) {
                    nuofClient = nuofClient - 1;
                    std::cerr << "recv failed, received bytes = " << bytes << std::endl;
                    break;
                }
                cout << nuofClient << ", "<<float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;


        usleep(microseconds);
    }

}
