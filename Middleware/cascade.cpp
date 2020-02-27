#if defined _MSC_VER && _MSC_VER >= 1400
#pragma warning(disable : 4100)
#endif
#include <iostream>
#include <iomanip>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"
using namespace std;
using namespace cv;
using namespace cv::cuda;
class Cascade {
    cv::CascadeClassifier cascade_cpu;
    vector <Rect> faces;
    Mat frame, frame_cpu, gray_cpu, resized_cpu, frameDisp;
    Mat image;

    GpuMat frame_gpu, gray_gpu, resized_gpu, facesBuf_gpu;
    Ptr <cuda::CascadeClassifier> cascade_gpu;
    double scaleFactor = 1.2;
    bool filterRects = true;
    bool findLargestObject = true;
    string cascadeName = "../haarcascade_licence_plate_rus_16stages.xml";
public:
    void getLicense_gpu(Mat frame);

    Cascade();

    static void convertAndResize(const Mat &src, Mat &gray, Mat &resized, double scale) {
        if (src.channels() == 3) {
            cv::cvtColor(src, gray, COLOR_BGR2GRAY);
        } else {
            gray = src;
        }
        Size sz(cvRound(gray.cols * scale), cvRound(gray.rows * scale));
        if (scale != 1) {
            cv::resize(gray, resized, sz);
        } else {
            resized = gray;
        }
    }

    static void convertAndResize(const GpuMat &src, GpuMat &gray, GpuMat &resized, double scale) {
        if (src.channels() == 3) {
            cv::cuda::cvtColor(src, gray, COLOR_BGR2GRAY);
        } else {
            gray = src;
        }
        Size sz(cvRound(gray.cols * scale), cvRound(gray.rows * scale));
        if (scale != 1) {
            cv::cuda::resize(gray, resized, sz);
        } else {
            resized = gray;
        }
    }
};
 void Cascade::getLicense_gpu(Mat frame) {
    double fps = 0.0;
    (image.empty() ? frame : image).copyTo(frame_cpu);
    convertAndResize(frame_cpu, gray_cpu, resized_cpu, scaleFactor);

    frame_gpu.upload(frame);
    convertAndResize(frame_gpu, gray_gpu, resized_gpu, scaleFactor);
    cascade_gpu->setFindLargestObject(findLargestObject);
    cascade_gpu->setScaleFactor(1.2);
    cascade_gpu->setMinNeighbors((filterRects || findLargestObject) ? 1 : 0);
    cascade_gpu->detectMultiScale(resized_gpu, facesBuf_gpu);
    cascade_gpu->convert(facesBuf_gpu, faces);
    for (size_t i = 0; i < faces.size(); ++i) {
        rectangle(resized_cpu, faces[i], Scalar(255));
    }


    //print detections to console
    cout << setfill(' ') << setprecision(2);
    cout << setw(6) << fixed << fps << " FPS, " << faces.size() << " det";
    if ((filterRects || findLargestObject) && !faces.empty()) {
        for (size_t i = 0; i < faces.size(); ++i) {
            cout << ", [" << setw(4) << faces[i].x
                 << ", " << setw(4) << faces[i].y
                 << ", " << setw(4) << faces[i].width
                 << ", " << setw(4) << faces[i].height << "]";
        }
    }
    cout << endl;
    cv::cvtColor(resized_cpu, frameDisp, COLOR_GRAY2BGR);
//        displayState(frameDisp, helpScreen, useGPU, findLargestObject, filterRects, fps);
    imshow("result", frameDisp);

}
Cascade::Cascade(void) {
    cascade_gpu = cuda::CascadeClassifier::create(cascadeName);
    if (!cascade_cpu.load(cascadeName)) {
        cerr << "ERROR: Could not load cascade classifier \"" << cascadeName << "\"" << endl;
    }
 }