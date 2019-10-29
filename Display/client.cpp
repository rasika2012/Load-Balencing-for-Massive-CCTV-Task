

/**
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <queue>

//#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
//#include "fun.h"
#include "PeopleDetect.cpp"
#include "../CPU_Controller/cpu_usage.cpp"
#define LOCK 1
#define UNLOCK 0

using namespace cv;
static int weght = 20*40;
static int hight = 20*30;
float cpu_totel,d_lock = 0;
bool cpu_send_data_lock = 0;
bool gpu_send_data_lock = 0;


queue <Mat> cpu_send_data;
queue <Mat> gpu_send_data;
queue <Mat> show_data;
Test test = Test();
cuda::GpuMat gpu_frame;
vector<Rect> found;
float utilization = 0.0;

void set_cpu_mat_lock(){
    if(!cpu_send_data_lock){
        cpu_send_data_lock = LOCK;
    } else{
        while (cpu_send_data_lock){
            cout << "cpu wait..\n";
        }
        cpu_send_data_lock = LOCK;
    }
}
void set_gpu_mat_lock(){
    if(!gpu_send_data_lock){
        gpu_send_data_lock = LOCK;
    } else{
        while (gpu_send_data_lock){
            cout << "gpu  wait..\n";
        }
        gpu_send_data_lock = LOCK;
    }
}

void cpu_releas(){
    cpu_send_data_lock = UNLOCK;
}

void gpu_releas(){
    gpu_send_data_lock = UNLOCK;
}
void clean_q(){
    cout<<"GPU: "<<gpu_send_data.size()<<" CPU: "<<cpu_send_data.size()<<" SHW: "<<show_data.size()<<" CPU USG: "<<utilization<<"%"<<endl;
    if (gpu_send_data.size()>50) {
        set_gpu_mat_lock();
        while (gpu_send_data.size() > 25) {
            gpu_send_data.pop();
            cout << "clening\n";
        }
        gpu_releas();
    }
    if (cpu_send_data.size()>50){
        set_cpu_mat_lock();
        while (cpu_send_data.size()>25){
            cpu_send_data.pop();
            cout<<"clening\n";
        }
        cpu_releas();
    }

    if (show_data.size()>50)
        while (show_data.size()>25){
            show_data.pop();
            cout<<"clening\n";
        }

}


void * cpu_process_pass(void * prt){
    Mat cpu_img;
    vector<Rect> found_cpu;
    Detector detector_cpu ;
    while (1){
        if(cpu_send_data.size() > 0){
            cout << "\n\n\ncpu\n\n\n"<<endl;
            set_cpu_mat_lock();
            cpu_img = cpu_send_data.front();
            cpu_send_data.pop();
            cpu_releas();
            found_cpu = detector_cpu.detect(cpu_img);
            for (vector<Rect>::iterator i = found_cpu.begin(); i != found_cpu.end(); ++i) {
                Rect &r = *i;
                detector_cpu.adjustRect(r);
                rectangle(cpu_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
            }
            show_data.push(cpu_img);
        }
    }

}
void * gpu_process_pass(void * prt){
    Mat gpu_img,temp_img;
    cuda::GpuMat gpu_mat;
    vector<Rect> found_gpu;
    Detector detector_gpu ;
    while (1){
        if(gpu_send_data.size() > 0){
            set_gpu_mat_lock();
            gpu_img = gpu_send_data.front();
            gpu_send_data.pop();
            gpu_releas();
            cvtColor(gpu_img, temp_img, COLOR_BGR2BGRA);
            gpu_mat.upload(temp_img);
            found_gpu = detector_gpu.detect_gpu(gpu_mat);
            for (vector<Rect>::iterator i = found_gpu.begin(); i != found_gpu.end(); ++i) {
                Rect &r = *i;
                detector_gpu.adjustRect(r);
                rectangle(gpu_img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
            }
            show_data.push(gpu_img);
        }
    }

}
void * show_image(void * prt){
    Mat show_img;

    while (1){
        //sleep(0.3);
        //cout<<"show size:"<<show_data.size()<<endl;

        clean_q();
        if(show_data.size() > 0){
            show_img = show_data.front();
            show_data.pop();
            cv::imshow("CV Video Client", show_img);
            //sleep(0.03);
        }
    }

}
void * set_cpu_status(void * prt){
        size_t previous_idle_time=0, previous_total_time=0;
        for (size_t idle_time, total_time; get_cpu_times(idle_time, total_time); sleep(1)) {
            const float idle_time_delta = idle_time - previous_idle_time;
            const float total_time_delta = total_time - previous_total_time;
            utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
//        std::cout << utilization << '%' << std::endl;
            previous_idle_time = idle_time;
            previous_total_time = total_time;
            cpu_totel = utilization;

        }
}






int main(int argc, char** argv){
    int64 time_gpu;
    int64 t = getTickCount();
    pthread_t cpu_t;
    pthread_t cpu_proess;
    pthread_t gpu_proess;
    pthread_t show_proess;
    char cpu_gpu = 'g';
    namedWindow("CV Video Client",1);
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

    Mat img;
    Mat frame, gray, frameDelta, thresh, firstFrame;

    img = Mat::zeros(hight , weght, CV_8UC3);
   // firstFrame = Mat::zeros(hight , weght, CV_8UC3);
   // frameDelta = Mat::zeros(hight , weght, CV_8UC3);

    int imgSize = img.total() * img.elemSize();
    uchar *iptr = img.data;
    int bytes;
    int key;


    pthread_create(&cpu_t,NULL,set_cpu_status,&cpu_t);
    pthread_create(&cpu_proess,NULL,cpu_process_pass,&cpu_proess);
    pthread_create(&gpu_proess,NULL,gpu_process_pass,&gpu_proess);
    pthread_create(&show_proess,NULL,show_image,&show_proess);



    //make img continuos
    if ( ! img.isContinuous() ) {
        img = img.clone();
    }

//    std::cout << "Image Size:" << imgSize << std::endl;

    const char key1 = (char)waitKey(30);

    while (key != 'q') {
        cpu_gpu = 'g';


        if ((bytes = recv(sokt, iptr, imgSize , MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
        }
        if(utilization < 90){
            cout << "\nprocess in cpu\n" << endl;
            set_cpu_mat_lock();
            cpu_send_data.push(img.clone());
            cpu_releas();
//            if(gpu_send_data.size()>=cpu_send_data.size()){
//                cout << "\nprocess in cpu\n" << endl;
//                set_cpu_mat_lock();
//                cpu_send_data.push(img.clone());
//                cpu_releas();
//            }else{
//                cout<<"MSG: CPU Load is high\n";
//            }


        } else{
            set_gpu_mat_lock();
            gpu_send_data.push(img.clone());
            gpu_releas();
            //sleep(1);

        }

        if (key = cv::waitKey(10) >= 0) break;
    }
    close(sokt);
    return 0;
}