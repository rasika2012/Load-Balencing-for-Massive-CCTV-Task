// #ifndef INCLUDE_FeedManager_HPP
// #define INCLUDE_FeedManager_HPP
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

class FeedManager
{
private:
    /* data */
public:

 int server(/* args */)
    {
        int sock, listener;
        struct sockaddr_in addr;

        if( (listener = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("[server] socket() failed");
            exit(1);
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(3425);
        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("[server] binding faild!");
            exit(2);
        }

        listen(listener, 1);


        int num_of_recv_bytes;
        VideoWriter outputVideo;
        Size S = Size((int) 640,(int) 480);

        int imgSize = 480*640*3;
        Mat frame = Mat::zeros(480, 640, CV_8UC3);
        uchar *iptr = frame.data;
        int key;

        int cnt=0;
        while(1){
            cout << ++cnt<<endl;
            sock = accept(listener, NULL, NULL);
            if(sock < 0){
                perror("[server] accept() faild!");
                exit(3);
            }

            while(key != 'q') {
                if( num_of_recv_bytes = recv(sock, iptr, imgSize, MSG_WAITALL) == -1 ) {
                    cerr << "recv failed, received bytes = " << num_of_recv_bytes << endl;
                }

                imshow("server", frame);
                if (key = waitKey(100) >= 0) break;
            }
            close(sock);
            break;
        }
        return 0;
    }

    int client(/* args */)
    {
        int sock;
        struct sockaddr_in addr;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0){
            perror("socket");
            exit(1);
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(3425);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
            perror("connect");
            exit(2);
        }

        int bbytee;
        cout << "before open the cam" << endl;

        VideoCapture cap("/media/rasika/Other/Dodama/Sequence 03.avi");

        if(!cap.isOpened()) {
            cout<< "Could not open the camera" <<  endl;
            close(sock);
            return -1;
        }

        Mat frame;
        frame = Mat::zeros(480, 640, CV_8UC3);
        int imgSize = frame.cols*frame.rows*3;

        int cnt=0;
        //Mat frame;
        while(1) {
            cap >> frame;
            cv::resize(frame, frame, cv::Size(640, 480));
            if(frame.empty()) {
                cerr<<"[client] VideoCapture(0) error!"<<endl;
            }

            cout<< ++cnt << ":"<< frame.isContinuous()<<"," <<frame.size()<<endl;;

            if( (bbytee = send(sock, frame.data, imgSize, 0)) < 0 ) {
                cerr<< "bytes = " << bbytee << endl;
                break;
            }

            cv::imshow("client", frame);
            if(cv::waitKey(100) == 'q') {
                break;
            }
        }
        close(sock);
        return 0;
    }
};


// #endif