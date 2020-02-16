//
// Created by chathuranga on 2019-10-11.
//
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//struct BufferPSNR                                     // Optimized CUDA versions
//{   // Data allocations are very expensive on CUDA. Use a buffer to solve: allocate once reuse later.
//    cuda::GpuMat gI1, gI2, gs, t1,t2;
//
//    cuda::GpuMat buf;
//};
class GPU_CAL{
public:
//    int getPSNR_CUDA(const Mat& I1, BufferPSNR& b)
//    {
//
//        int val = 0;
//        b.gI1.convertTo(b.t1, CV_32F);
//        b.gI2.convertTo(b.t2, CV_32F);
//
//
//        cuda::absdiff(b.t1.reshape(1), b.t2.reshape(1), b.gs);
//        cuda::multiply(b.gs, b.gs, b.gs);
////        imshow( "privius", I1 );
//        double sse = cuda::sum(b.gs, b.buf)[0];
//        if( sse <= 1e-5) // for small values return zero
//            return 0;
//        else
//        {
//            double mse = sse /(double)(I1.channels() * I1.total());
//            double psnr = 50.0*log10((255*255)/mse);
//            val = round(psnr);
////             printf("%d\n",val);
//            return val;
//        }
//    }
    int getPSNR_CPU(const Mat& I1, const Mat& I2)
    {
        Mat s1;
        double val = 0;
        absdiff(I1, I2, s1);       // |I1 - I2|

        s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
        s1 = s1.mul(s1);           // |I1 - I2|^2

        Scalar s = sum(s1);         // sum elements per channel

        double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

        if( sse <= 1e-10) {
            return 0;
        }// for small values return zero
         else
        {
            double  mse =sse /(double)(I1.channels() * I1.total());
            double psnr = 10.0*log10((255*255)/mse);
            val = round(psnr);
            return val;
        }
    }
};
