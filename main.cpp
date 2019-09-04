#include <iostream> 
#include <string> 
#include "FeedManager.cpp"
#include "GPU/GpuManager.cpp"
using namespace std; 

int main(int argc, char const *argv[])
{
    int gpuCount = 0;
    GpuManager gpuManager;
    FeedManager feedManager;
    // Thread 1
    feedManager.server();
    // Thread 2
    feedManager.client();


    gpuCount = stoi(gpuManager.GetStdoutFromCommand("nproc"));
    printf("%d\n",gpuCount);

    return 0;
}
