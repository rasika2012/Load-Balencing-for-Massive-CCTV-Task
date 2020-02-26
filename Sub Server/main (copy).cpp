//
// Created by chathuranga on 2020-02-24.
//

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
#include <sstream>
#include "algorithems/cascade.cpp"
using namespace std;
using namespace cv;
using namespace std::chrono;
void licence(string ip){
    Cascade cascade;
    Mat img;
    VideoCapture cap(ip);
    while (1) {
        cap >> img;

        cascade.getLicense_gpu(img);
    }
}
void licence2(string ip){
    Cascade cascade;
    Mat img;
    img = Mat::zeros(480, 640, CV_8UC3);
//    VideoCapture cap("rtsp://10.42.0.48:8080/h264_ulaw.sdp");
    VideoCapture cap("CCTV2.mp4");
    if (!img.isContinuous()) {
        img = img.clone();
    }
    cout << "ProccessID, countOfDetection, Time(ms)"<< endl;
    while (1) {
        cap >> img;
        if(img.empty()){
            cout << " clip end\n" << endl;
            break;
        }
        cv::resize(img, img, cv::Size(640, 480),CV_8UC3);
//        cout << img.size() << "    ";
//        cv::imshow("result1", img);
        auto start = high_resolution_clock::now();
        cascade.getLicense_gpu(img);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << " "<< duration.count()  << endl;
    }
}
int main(int argc, char const *argv[])
{
    int process = std::stoi(argv[2]);
    string cam = argv[1];
    switch (process){
        case 1:
            licence(cam);
            break;
        case 2:
            licence2(cam);
            break;
        default:
            cout << "defoult" << endl;
            licence(cam);
    }

    return 0;
}
