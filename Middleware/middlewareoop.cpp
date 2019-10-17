
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
#include "ImgDiffCuda.cpp"

void *display(void *);
char **args;
using namespace std;
using namespace cv;

queue <Mat> resived_data;
queue <Mat> to_send_data;
//BufferPSNR bufferPsnr;
///gloal variables
    char * IP;
    int rmport,srvport;
//

// struct BufferPSNR                                     // Optimized CUDA versions
// {   // Data allocations are very expensive on CUDA. Use a buffer to solve: allocate once reuse later.
//     cuda::GpuMat gI1, gI2, gs, t1,t2;

//     cuda::GpuMat buf;
// };

void clean_q_r(){
    if(resived_data.size() > 150 )
    while (resived_data.size() > 100 ){
        resived_data.pop();
//        cout<<"cleaned rsd\n";
    }

}
void clea_q_s(){
    if(resived_data.size() > 150 )
    while (to_send_data.size() > 100 ){
        to_send_data.pop();
//        cout<<"cleaned send\n";

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
        //for(int i =0 ; i<10;i++)
        resived_data.push(img.clone());
        // tmp add
       

        //putText(img, "Text", Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
        
       

        // to_send_data.push(resived_data.front());
        // resived_data.pop();
        clean_q_r();
        if (key = cv::waitKey(10) >= 0) break;
    }   

    close(sokt);
        
}

void * connectDisplay(void * ptr){
    int socket = *(int *)ptr;
    Mat img;
    img = Mat::zeros(480 , 640, CV_8UC3);
    clea_q_s();
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

                    if(!img.empty()){
                        cv::resize(img, img, cv::Size(640, 480),CV_8UC3);

                    }
                
                //do video processing here 
                //cvtColor(img, imgGray, CV_BGR2GRAY);
                //send processed image
                if ((bytes = send(socket, img.data, imgSize, 0)) < 0){
                     std::cerr << "bytes = " << bytes << std::endl;
                     break;
                }
                } else{
                   // sleep(0.1);
                }

   
    }

}

void * algorithm(void * prt){
    BufferPSNR bufferPsnr;
    GPU_CAL cal = GPU_CAL();
    int result, temp = 0;
//    int TIMES = 10;
    double time_cpu, time_cuda = 0.0;

    while (1)
    {

   
    if(resived_data.size() >3){

        Mat img1 = resived_data.front();
        resived_data.pop();
        if (img1.empty())
            break;

        Mat img2 = resived_data.front();
        if (img2.empty()){
            break;
        }


        time_cuda = (double)getTickCount();
        result = cal.getPSNR_CUDA(img2, img1, bufferPsnr);
        time_cuda = 1000*((double)getTickCount() - time_cuda)/getTickFrequency();
//        cout << "CUDA : " << time_cuda << endl;
        time_cpu = (double)getTickCount();
        result = cal.getPSNR_CPU(img2, img1);
        time_cpu = 1000*((double)getTickCount() - time_cpu)/getTickFrequency();
//        cout << "CPU : " << time_cpu << endl;
//        printf("pass cuda\n");

        if(temp != result)
        {
//            printf("Detected\n");
            cout <<  time_cpu <<","<< time_cuda<< "," << "Detected" << endl;

//            printf("%lu\t%lu\tDetected\n", time_cpu, time_cuda);
            to_send_data.push(img2);
        }else
        {
//            printf("Ignore\n");
//            printf("%lu\t%lu\tIgnore\n", time_cpu, time_cuda);
            cout <<  time_cpu <<","<< time_cuda<< "," << "Ignore" << endl;

        }
        temp = result;

       
   }
   }
        
}


void * algorithm2(void * prt){
    BufferPSNR bufferPsnr;
    GPU_CAL cal = GPU_CAL();
    int result, temp = 0;
//    int TIMES = 10;
    double time_cpu, time_cuda = 0.0;

    while (1)
    {


        if(resived_data.size() >3){

            Mat img1 = resived_data.front();
            resived_data.pop();
            if (img1.empty())
                break;

            Mat img2 = resived_data.front();
            if (img2.empty()){
                break;
            }


            time_cuda = (double)getTickCount();
            result = cal.getPSNR_CUDA(img2, img1, bufferPsnr);
            time_cuda = 1000*((double)getTickCount() - time_cuda)/getTickFrequency();
//        cout << "CUDA : " << time_cuda << endl;
            time_cpu = (double)getTickCount();
            result = cal.getPSNR_CPU(img2, img1);
            time_cpu = 1000*((double)getTickCount() - time_cpu)/getTickFrequency();
//        cout << "CPU : " << time_cpu << endl;
//        printf("pass cuda\n");

            if(temp != result)
            {
//            printf("Detected\n");
                cout <<  time_cpu <<","<< time_cuda<< "," << "Detected" << endl;

//            printf("%lu\t%lu\tDetected\n", time_cpu, time_cuda);
                to_send_data.push(img2);
            }else
            {
//            printf("Ignore\n");
//            printf("%lu\t%lu\tIgnore\n", time_cpu, time_cuda);
                cout <<  time_cpu <<","<< time_cuda<< "," << "Ignore" << endl;

            }
            temp = result;


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
//        clean_q();
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


 
     cin >> command;
     
        pthread_t t;
        pthread_t p;
        pthread_t alg;
//        pthread_t alg2;

//        cout<<"\nQ Size:"<<resived_data.size()<<"\n>";
        pthread_create(&t,NULL,connectCamera,&t);
        pthread_create(&p,NULL,serverUp,&p);
        pthread_create(&alg,NULL,algorithm,&alg);
//        pthread_create(&alg2,NULL,algorithm2,&alg2);

        cin >> command;
    
    return 0;
}
