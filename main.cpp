#include <iostream> 
#include <string> 
#include "FeedManager.cpp"
#include <pthread.h>
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

void *t1(void *display_id) {
    cout<<"SERVER STARTED\n";
    
    FeedManager feedManager;
    // Thread 1
    feedManager.server();
}

void *t2(void *display_id) {
    cout<<"CLIENT STARTED\n";
    sleep(1);
    FeedManager feedManager;
    feedManager.client();
}

int main(int argc, char const *argv[])
{
    int a = 0;
    
    // Thread 2
    

    pthread_t threads[2];
    int rc;
    int i = 0;

    rc = pthread_create(&threads[0], NULL, t1, (void *)i);
        
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
    }
    rc = pthread_create(&threads[1], NULL, t2, (void *)i);
    if (rc) {
        cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
    }

    pthread_exit(NULL);
    
    a = stoi(GetStdoutFromCommand("nproc"));
    printf("%d\n",a);

    return 0;
}
