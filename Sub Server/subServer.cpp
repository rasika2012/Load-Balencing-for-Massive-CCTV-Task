//
// Created by chathuranga on 2020-02-23.
//



//git clone https://github.com/Microsoft/vcpkg.git
//cd vcpkg
//    ./bootstrap-vcpkg.sh
//    ./vcpkg integrate install
//vcpkg install jsoncpp
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
#include <json/json.h>

char **args;
using namespace std;
using namespace cv;
queue <Mat> hdQ;
queue <Mat> lpdQ;

string json = "{\"call\": \"KF6GPE\",\"type\":\"l\",\"time\":\"1399371514\",\"lasttime\":\"1418597513\",\"lat\": 37.17667,\"lng\": -122.14650,\"result\":\"ok\"}";
string json_file = "{ \"CAM\": [{\"IP\": \"127.0.0.1\", \"PORT\" : \"5000\",\"PROCESS_ID\": 1,\"IS_START\": \"True\"},{\"IP\": \"127.0.0.1\",\"PORT\" : \"5000\",\"PROCESS_ID\": 2,\"IS_START\": \"True\"}]}";
void* humanDetect(void * prt){
    cout<<"humanDetect"<<endl;
}
void* licenPlateDetect(void * prt){
    cout<<"licenPlateDetect"<<endl;
}
int main(int argc, char const *argv[])
{
    pthread_t HD;
    pthread_t LPD;

    pthread_create(&HD,NULL,humanDetect,&HD);
    pthread_create(&LPD,NULL,licenPlateDetect,&LPD);
//
    Json::Reader reader;
    Json::Value root;
    bool parseSuccess = reader.parse(json_file, root, false);

    if (parseSuccess)
    {
        const Json::Value resultValue = root["CAM"];
        const Json::Value val = resultValue[0]["IP"];
        string s = val.asString();
        cout << "Result is " << resultValue. << "\n";
    }



    return 0;
}
