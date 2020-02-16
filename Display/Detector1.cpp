
#include "opencv2/opencv.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <jmorecfg.h>


#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"
#include "opencv2/core/cuda.hpp"


using namespace cv;
using namespace std;
using namespace cv::cuda;
Size  downFrameSize(640, 480);


Ptr<cuda::CascadeClassifier> cascade_gpu_upperbody, cascade_gpu_lowerbody, cascade_gpu_fullbody;

//#include "Detector1.cpp"

int win_width = 48;
//48*96 rectangle is found for HOG
int cell_width = 8;
int nbins = 9;
int win_stride_width = 8;
int win_stride_height = win_stride_width;
int block_width = win_stride_width*2;
int num_frames = 1000; // Process 2 fps; usually video will have higher frame rate like 24 fps; We may not need to analze all
int counter_frames_detected = 0;

int hogLevels =  HOGDescriptor::DEFAULT_NLEVELS;
int hogGroupThreshold = 16;
Size win_stride(win_stride_width, win_stride_height);


/* From above these below are standard setting*/
Size win_size(win_width, win_width * 2);
Size block_size(block_width, block_width);
int block_stride_width = block_width / 2;
int block_stride_height = block_width / 2;
Size block_stride(block_stride_width, block_stride_height);
Size cell_size(cell_width, cell_width);
cv::HOGDescriptor cpu_hog(win_size, block_size, block_stride, cell_size, nbins, 1, -1,
                          HOGDescriptor::L2Hys, .2, false, hogLevels);

class Test{
    /**
Sclar - BGR value
**/
    void drawMarker(Mat img, std::vector<cv::Rect>  found, Scalar sc, int size = 2) {

        for (int i = 0; i < (int)found.size(); i++)
        {
            cv::Rect r = found[i];
            cv::rectangle(img, r, sc, size);
        }
    }


/** @function detectAndDisplay using CPU  */
public: std::vector<Rect>  detectAndDisplayHOG(Mat img, Ptr<cv::cuda::HOG> gpu_hog, bool useGPU)
    {
        Mat frame;
        std::vector<cv::Rect>  found;
        //The GroupThreshold and ScaleFactor are the two important parameters
        //decrease will get more hits, with more false positives
        int _hitThreshold = 0;// //going mad tuning this for cuda// not to be adjusted
        double _scaleFactor = 1.01;//1.05;// 20 sec --> huge impact on performance
        cv::imshow("CV Video Client", img);

        if (useGPU) {
            cv::cvtColor(img, frame, COLOR_BGR2BGRA);// COLOR_BGR2BGRA);
            GpuMat gpuFrame(frame); // cuda::GpuMat gpu_img

//            cv::cuda::GpuMat gpuFrame;
//            gpuFrame.upload(img);

            gpu_hog->setScaleFactor(_scaleFactor);
            gpu_hog->setNumLevels(hogLevels);
            gpu_hog->setWinStride(win_stride);


            //gpu_hog->setHitThreshold(0); // play with this at your own risk :)
            gpu_hog->setGroupThreshold(hogGroupThreshold);// setting it to higher will reduce false positives// give all
            gpu_hog->detectMultiScale(gpuFrame, found);
            drawMarker(img, found, Scalar(255, 0, 0), 1);//BGR
            gpu_hog->setGroupThreshold(hogGroupThreshold*3);// setting it to higher will group more
            gpu_hog->detectMultiScale(gpuFrame, found);





//            gpu_hog->setNumLevels(2);
//            //    gpu_hog->setHitThreshold(0.5);
//            //    gpu_hog->setWinStride(Size(16, 16));
//            gpu_hog->setScaleFactor(1.05);
//            //    gpu_hog->setMinObjectSize()
//            //    gpu_hog->setGroupThreshold(20);
//             gpu_hog->setFindLargestObject(true);
//             gpu_hog->setScaleFactor(1.1);
//             gpu_hog->setMinNeighbors(4);
//             gpu_hog->setMinObjectSize(cv::Size(40,40));
//             gpu_hog->setMaxObjectSize(image_src.size());
            gpu_hog->detectMultiScale(gpuFrame, found);




            drawMarker(img, found, Scalar(0, 255, 0));//BGR
        }
        else
        {
            //std::vector<DetectionROI> locations;
            //cpu_hog.detectMultiScaleROI(frame, found, locations, _hitThreshold, 0);
            cv::cvtColor(img, frame, COLOR_BGR2GRAY);//(img.type() == CV_8U || img.type() == CV_8UC3)
            cpu_hog.detectMultiScale(frame, found, _hitThreshold, win_stride, cv::Size(4, 4), _scaleFactor);
            drawMarker(img, found, Scalar(255, 0, 0));//BGR

        }
//        if (found.size() > 1) {
//            counter_frames_detected += 1;
//        }
        cv::imshow("CV Video detect", img);
        return found;
    }
 void adjustRect(Rect & r) const
    {

        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
    }
};