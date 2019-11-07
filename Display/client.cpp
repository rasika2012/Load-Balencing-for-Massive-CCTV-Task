/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>


using namespace cv;
static int weght = 20*40;
static int hight = 20*30;

int main(int argc, char** argv){
    Test test = Test();
    int64 time_gpu;
    int64 t = getTickCount();

    //--------------------------------------------------------
    //networking stuff: socket , connect
    //--------------------------------------------------------
    int sokt;
    char* serverIP;
    int serverPort;

    if (argc < 3) {
        std::cerr << "Usage: cv_video_cli <serverIP> <serverPort> " << std::endl;
    }

    serverIP   = argv[1];
    serverPort = atoi(argv[2]);

    struct  sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    if ((sokt = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "socket() failed" << std::endl;
    }

    serverAddr.sin_family = PF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if (connect(sokt, (sockaddr*)&serverAddr, addrLen) < 0) {
        std::cerr << "connect() failed!" << std::endl;
    }


    //----------------------------------------------------------
    //OpenCV Code
    //----------------------------------------------------------

    Mat img,imgGray;
    Mat frame, gray, frameDelta, thresh, firstFrame;

    img = Mat::zeros(hight , weght, CV_8UC3);
    firstFrame = Mat::zeros(hight , weght, CV_8UC3);
    frameDelta = Mat::zeros(hight , weght, CV_8UC3);

    imgGray = Mat::zeros(hight , weght, CV_8UC1);
    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes;
    int key;

    //make img continuos
    if ( ! img.isContinuous() ) {
        img = img.clone();
    }

    std::cout << "Image Size:" << imgSize << std::endl;
    namedWindow("CV Video Client",1);
    const char key1 = (char)waitKey(30);

    while (key != 'q') {

        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
        }
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
//        GaussianBlur(imgGray, imgGray, Size(21, 21), 0);
        // absdiff(firstFrame, img, frameDelta);
        //compute difference between first frame and current frame

        img = test.matConvert(img,'g');
        cv::imshow("CV Video Client", img);
        firstFrame = img.clone();
        time_gpu = 1000*((double)getTickCount() - t)/getTickFrequency();
        cout << time_gpu << endl;

        if (key = cv::waitKey(10) >= 0) break;
    }
    close(sokt);
    return 0;
}	