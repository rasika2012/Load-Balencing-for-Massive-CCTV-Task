
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <iomanip>
using namespace std;
using namespace cv;
class Detector
{
    enum Mode { Default, Daimler } m;
    HOGDescriptor hog, hog_d;
    //gpu
    cv::Ptr<cv::cuda::HOG> gpu_hog;
    //
public:
    Detector() : m(Default), hog(), hog_d(Size(48, 96), Size(16, 16), Size(8, 8), Size(8, 8), 9)
    {
        hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
        hog_d.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());

        //gpu
        gpu_hog = cv::cuda::HOG::create(Size(64, 128),Size(16, 16),Size(8, 8),Size(8, 8),9);
        Mat detector = gpu_hog->getDefaultPeopleDetector();
        gpu_hog->setSVMDetector(detector);
        //
    }
    void toggleMode() { m = (m == Default ? Daimler : Default); }
    string modeName() const { return (m == Default ? "Default" : "Daimler"); }
    vector<Rect> detect(InputArray img)
    {
        // Run the detector with default parameters. to get a higher hit-rate
        // (and more false alarms, respectively), decrease the hitThreshold and
        // groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
        vector<Rect> found;
        if (m == Default)
            hog.detectMultiScale(img, found, 0, Size(8,8), Size(), 1.05, 2, false);
        else if (m == Daimler)
            hog_d.detectMultiScale(img, found, 0, Size(8,8), Size(), 1.05, 2, true);
        return found;
    }
    std::vector<Rect> detect_gpu(cuda::GpuMat gpu_img)
    {

        std::vector<Rect> found;
        double time = 0.0;
        gpu_hog->setNumLevels(2);
        //    gpu_hog->setHitThreshold(0.5);
        //    gpu_hog->setWinStride(Size(16, 16));
        gpu_hog->setScaleFactor(1.05);
        //    gpu_hog->setMinObjectSize()
        //    gpu_hog->setGroupThreshold(20);
        // gpu_hog->setFindLargestObject(true);
        // gpu_hog->setScaleFactor(1.1);
        // gpu_hog->setMinNeighbors(4);
        // gpu_hog->setMinObjectSize(cv::Size(40,40));
        // gpu_hog->setMaxObjectSize(image_src.size());
        gpu_hog->detectMultiScale(gpu_img, found);
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


class UseD{
int detecter(VideoCapture cap)
{

    cout << "Press 'q' or <ESC> to quit." << endl;
    cout << "Press <space> to toggle between Default and Daimler detector" << endl;
    Detector detector;
    Mat frame;
    for (;;)
    {
        cap >> frame;
        if (frame.empty())
        {
            cout << "Finished reading: empty frame" << endl;
            break;
        }
        int64 t = getTickCount();
        vector<Rect> found = detector.detect(frame);
        t = getTickCount() - t;

        // show the window
        {
            ostringstream buf;
            buf << "Mode: " << detector.modeName() << " ||| "
                << "FPS: " << fixed << setprecision(1) << (getTickFrequency() / (double)t);
            putText(frame, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
        }
        for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
        {
            Rect &r = *i;
            detector.adjustRect(r);
            rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }
        imshow("People detector", frame);

        // interact with user
        const char key = (char)waitKey(1);
        if (key == 27 || key == 'q') // ESC
        {
            cout << "Exit requested" << endl;
            break;
        }
        else if (key == ' ')
        {
            detector.toggleMode();
        }
    }
    return 0;
}
};