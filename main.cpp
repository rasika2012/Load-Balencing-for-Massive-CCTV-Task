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
    

    pid_t pid = fork();

    if (pid == 0)
    {
     // Thread 1
        feedManager.server();
        
    }
    else if (pid > 0)
    {
          // Thread 2
        sleep(1);
        feedManager.client();
    }

    gpuCount = stoi(gpuManager.GetStdoutFromCommand("nproc"));
    printf("%d\n",gpuCount);

    return 0;
}
