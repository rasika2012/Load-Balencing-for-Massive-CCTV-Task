/**
 * OpenCV video streaming over TCP/IP
 * Server: Captures video from a webcam and send it to a client
 * by Isaac Maia
 */
#include <opencv2/imgproc/imgproc.hpp>
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

#define URL "rtsp://192.16.8.100:8080/h264_ulaw.sdp" 

int capDev = 0;
cv::Mat image;
cv::Mat image1;
cv::Mat image2;
cv::Mat df;
//"rtsp://192.168.8.100:8080/h264_ulaw.sdp"
// open the default camera "CCTV.mp4"

//    VideoCapture cap(0);
   


int main(int argc, char** argv)

{   
    cout<<URL;
    // VideoCapture vcap("rtsp://192.168.8.100:8080/h264_ulaw.sdp");
    VideoCapture vcap("CCTV.mp4");
    //  if(argc >1 )
    //     VideoCapture vcap(argv[1]); 
     
    // VideoCapture vcap;
     cout<<URL;
    char cmd[] = "cmd";
    bool t = 0;
    int count = 0;
    clock_t begin_time = clock();
    clock_t second_time = clock();

    clock_t fr1 = clock();
    clock_t fr2 = clock();

    for(;;) {
        begin_time = clock();
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }else if (t){
            fr2 = clock();
            
            absdiff(image1, image, df);
            cv::cvtColor(df, df, cv::COLOR_BGR2GRAY);

            cv::imshow("Output Window", df);

            // count++;
            // if(count>100)break;
            if(cv::waitKey(1) >= 0) break;
            image2 = image;
            image = image1;
            image1 = image2;
            second_time = clock();

            //cout<< float(cv::norm(df))<<" ";
            if (float(cv::norm(df)) > 600.0)
            cout<< "1";
            else
            cout<< "0";
            cout<<" "<<" "<< second_time - begin_time<<" "<< float(cv::norm(df)) <<endl;
            // cout<<fr2-fr1 <<",";
            // cin>>cmd;
            fr1 = clock();
            // system("pause");
            
        }else if (!t)
        {
             image1 = image.clone();
             t=1;
        }
        
    }   
    cout<<endl;
}

