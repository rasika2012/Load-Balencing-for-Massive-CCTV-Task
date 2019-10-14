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
int getPSNR_CUDA_optimized(const Mat& I1, const Mat& I2, BufferPSNR& b)
{
    b.gI1.upload(I1);
    b.gI2.upload(I2);
    int val = 0;
    b.gI1.convertTo(b.t1, CV_32F);
    b.gI2.convertTo(b.t2, CV_32F);

    cuda::absdiff(b.t1.reshape(1), b.t2.reshape(1), b.gs);
    cuda::multiply(b.gs, b.gs, b.gs);
    // imshow( "privius", I1 );
    double sse = cuda::sum(b.gs, b.buf)[0];

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double mse = sse /(double)(I1.channels() * I1.total());
        double psnr = log10((255*255)/mse);
        val = round(psnr);
        // printf("%d\n",val);
        return val;
    }
}
