//
// Created by chathuranga on 2019-10-11.
//
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct BufferPSNR                                     // Optimized CUDA versions
{   // Data allocations are very expensive on CUDA. Use a buffer to solve: allocate once reuse later.
    cuda::GpuMat gI1, gI2, gs, t1,t2;

    cuda::GpuMat buf;
};
class GPU_CAL{
public:
    cuda::GpuMat s1;
    int getPSNR_CUDA(const Mat& I1, const Mat& I2, BufferPSNR& b)
    {
       
        b.gI1.upload(I1);
        b.gI2.upload(I2);
        int val = 0;
        //b.gI1.convertTo(b.t1, CV_32F);
        //b.gI2.convertTo(b.t2, CV_32F);

        cuda::absdiff(b.gI1.reshape(1), b.gI2.reshape(1), s1);
        
        s1.convertTo(s1, CV_32F);
        cuda::multiply(s1, s1,s1);
        // imshow( "privius", I1 );
        double sse = cuda::sum(s1, b.buf)[0];

        if( sse <= 1e-10) // for small values return zero
            return 0;
        else
        {
            double mse = sse /(double)(I1.channels() * I1.total());
            double psnr = 5.0*log10((255*255)/mse);
            val = round(psnr);
            // printf("%d\n",val);
            return val;
        }
    }
    int getPSNR_CPU(const Mat& I1, const Mat& I2)
    {
        int val = 0;
        Mat s1;
        absdiff(I1, I2, s1);       // |I1 - I2|
        s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
        s1 = s1.mul(s1);           // |I1 - I2|^2

        Scalar s = sum(s1);         // sum elements per channel

        double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

        if( sse <= 1e-10) // for small values return zero
            return 0;
        else
        {
            double  mse =sse /(double)(I1.channels() * I1.total());
            double psnr = 5.0*log10((255*255)/mse);
            val = round(psnr);
            return val;
        }
    }
};
