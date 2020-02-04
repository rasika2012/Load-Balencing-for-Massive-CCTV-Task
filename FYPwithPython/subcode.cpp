#include <iostream> 
#include <random>
#include <unistd.h>

using namespace std; 
  
int main(int argc, char* argv[]) 
{ 
    if(argc>2){
        char* IP = argv[1];
        char* GPU = argv[2];
    }
    else{
        cout<<"INPUT ERROR: [IP or Path] [GPU CODE]"<<endl;
        return -1;
    }
  
    while (1){
        if (std::rand()/617770673 == 0){
            std::cout << "1 "<< std::rand()<< std::endl;
        }else{
            std::cout << "0 "<<std::rand() << std::endl;
        }
        usleep(100000);
        
    }
    return 0; 
} 