#include <iostream> 
#include <string> 
#include "FeedManager.cpp"
using namespace std; 

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
                pclose(stream);
    }

    return data;
}

int main(int argc, char const *argv[])
{
    int a = 0;
    FeedManager feedManager;
    // Thread 1
    feedManager.server();
    // Thread 2
    feedManager.client();
    a = stoi(GetStdoutFromCommand("nproc"));
    printf("%d\n",a);

    return 0;
}
