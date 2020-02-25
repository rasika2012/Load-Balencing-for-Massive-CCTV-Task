
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
static int weght = 20*40;
static int hight = 20*30;
//BufferPSNR bufferPsnr;
///gloal variables
char * IP;
int PORT, DEVICE;



void * cameraRead(){
    int sokt;
    struct      sockaddr_in serverAddr;
    socklen_t   addrLen = sizeof(struct sockaddr_in);
    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout<< "2 " << 0.0000 << std::endl;
        return  0;
    }
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP);
    serverAddr.sin_port = htons(PORT);
    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
        std::cout<< "2 " << 0.0000 << std::endl;
        return 0;
    }



    cv::Mat img, img2;
//    BufferPSNR bufferPsnr;
    GPU_CAL cal = GPU_CAL();
    img = Mat::zeros(hight , weght, CV_8UC3);
    img2 = Mat::zeros(hight , weght, CV_8UC3);
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes = 0;
    int key;
    if ( ! img.isContinuous() ) {
        img = img.clone();
    }
    bool isFirst = true;
    double result,temp = 0.0;
    double time_cuda = 0.0;
    char windowName[20];
    while (key != 'q') {

        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
        }
        //for(int i =0 ; i<10;i++)
        resived_data.push(img.clone());
        // tmp add
        if (isFirst){
//            bufferPsnr.gI1.upload(img);
            img2 = img;
            isFirst = false;
        } else{
            time_cuda = (double)getTickCount();
//            bufferPsnr.gI2.upload(img);
//            cv::imshow("windowName", img);
//            cv::imshow("windowName 2", img2);

            result = cal.getPSNR_CPU(img,img2);
            img2 = img.clone();
//            result = cal.getPSNR_CUDA(img, bufferPsnr);
            time_cuda = 1000*((double)getTickCount() - time_cuda)/getTickFrequency();

        }
//        sprintf(windowName, "%s  %f", "results: ", result);

        //putText(img, "Text", Point(5,100), FONT_HERSHEY_DUPLEX, 1, Scalar(0,143,143), 2);
//        cv::destroyAllWindows();
        if(temp!=result){
            std::cout<< "1 " << time_cuda << std::endl;
        } else{
            std::cout<< "0 " << time_cuda << std::endl;
        }
//        std::cout<< result << std::endl;

        // to_send_data.push(resived_data.front());
        // resived_data.pop();
        temp = result;
        if (key = cv::waitKey(10) >= 0) break;
    }
    std::cout<< "2 " << 0.0000 << std::endl;

    close(sokt);
    return  0;
}

int main(int argc, char** argv){
    if(argc<4){
        perror("arguments not found");
        std::cout << "usage: ./middilware 127.0.0.1 5000 1" << std::endl;
        return -1;
    }
    IP = argv[1];
    PORT = atoi(argv[2]);
    DEVICE = atoi(argv[3]);
//    std::cout << "IP "<< IP << std::endl;
//    std::cout << "PORT "<< PORT << std::endl;
//    std::cout << "DEVICE "<< DEVICE << std::endl;

//    pthread_t connectCam;


    cameraRead();
    return 0;

}
