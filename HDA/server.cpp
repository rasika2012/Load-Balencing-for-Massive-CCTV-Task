#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main( int argc, const char** argv )
{
    /// Use the cmdlineparser to process input arguments
    // CommandLineParser parser(argc, argv,
    //     "{ help h            |      | show this message }"
    //     "{ video v           |      | (required) path to video }"
    // );

    // /// If help is entered
    // if (parser.has("help")){
    //     parser.printMessage();
    //     return 0;
    // }

    // /// Parse arguments
    // string video_location(parser.get<string>("video"));
    // if (video_location.empty()){
    //     parser.printMessage();
    //     return -1;
    // }

    /// Create a videoreader interface
    VideoCapture cap(argv[1]);
    Mat current_frame;

    /// Set up the pedestrian detector --> let us take the default one
    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    Size size(640,480);
    /// Set up tracking vector
    vector<Point> track;
    clock_t first_time = clock();
    clock_t second_time = clock();
    while(true){
        /// Grab a single frame from the video
        cap >> current_frame;

        /// Check if the frame has content
        if(current_frame.empty()){
            cerr << "Video has ended or bad frame was read. Quitting." << endl;
            return 0;
        }

        /// run the detector with default parameters. to get a higher hit-rate
        /// (and more false alarms, respectively), decrease the hitThreshold and
        /// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).

        ///image, vector of rectangles, hit threshold, win stride, padding, scale, group th
        Mat img = current_frame.clone();
        resize(img,img,size);

        vector<Rect> found;
        vector<double> weights;

        first_time = clock();
        hog.detectMultiScale(img, found, weights);
        second_time = clock();

        /// draw detections and store location
        for( size_t i = 0; i < found.size(); i++ )
        {
            Rect r = found[i];
            rectangle(img, found[i], cv::Scalar(0,0,255), 3);
            stringstream temp;
            temp << weights[i];
            putText(img, temp.str(),Point(found[i].x,found[i].y+50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255));
            track.push_back(Point(found[i].x+found[i].width/2,found[i].y+found[i].height/2));
        }
      
        cout<<found.size()<<" "<<" "<< second_time - first_time << " " << 4 <<endl;


        /// plot the track so far
        for(size_t i = 1; i < track.size(); i++){
            line(img, track[i-1], track[i], Scalar(255,255,0), 2);
        }

        /// Show
        imshow("detected person", img);
        waitKey(1);
    }

    return 0;
}