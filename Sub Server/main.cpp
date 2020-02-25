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
using namespace std;
using namespace cv;
int main(int argc, char const *argv[])
{
    cout<< argc << endl;
    if(argc!=3){
        cout << "2,0" << endl;
        exit();
    }
    string camera_path = argc[1];
    
    return 0;
}
