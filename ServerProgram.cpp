// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string>
#include <arpa/inet.h> 

using namespace std;

//simple server and socket library
class Server{
    private :
        int port;
        int server_fd, new_socket, valread; 
        struct sockaddr_in address; 
        int opt = 1; 
        int addrlen = sizeof(address); 
        char buffer[1024] = {0}; 


    
    public :
        void setup(int port){
            port = port;
         
            if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
                perror("socket creation failed"); 
                exit(EXIT_FAILURE); 
            } 
        
            if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                        &opt, sizeof(opt))) { 
                perror("socket attaching failed"); 
                exit(EXIT_FAILURE); 
            } 
            
            address.sin_family = AF_INET; 
            address.sin_addr.s_addr = INADDR_ANY; 
            address.sin_port = htons( port ); 
           
            if (bind(server_fd, (struct sockaddr *)&address, 
                                        sizeof(address))<0){ 
                perror("bind failed"); 
                exit(EXIT_FAILURE); 
            } 
            if (listen(server_fd, 3) < 0) { 
                perror("listen"); 
                exit(EXIT_FAILURE); 
            } 

        }

        char* listen_request(char * send_buffer){
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                            (socklen_t*)&addrlen))<0) { 
                perror("accept"); 
                exit(EXIT_FAILURE); 
            } 
            valread = read( new_socket , buffer, 1024); 
            send(new_socket , send_buffer , strlen(send_buffer) , 0 ); 
            printf("Server : Sent \n"); 
            return buffer;
        }
};

class Client{
	private:
		int sock = 0, valread; 
		struct sockaddr_in serv_addr; 
		char buffer[1024] = {0}; 
		char * destn ;
		int portn=0;

	public:
		int setup(char * dest, int port){
			destn = dest;
			portn = port;
			
			
			
		}
		char* send_request(char* buff){
			

			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
				printf("\n Socket creation error \n"); 
				return "Null"; 
			} 
			
			serv_addr.sin_family = AF_INET; 
			serv_addr.sin_port = htons(portn); 
			
			if(inet_pton(AF_INET, destn, &serv_addr.sin_addr)<=0) { 
				printf("\nInvalid address/ Address not supported \n"); 
				return "Null"; 
			} 

			if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
				printf("\nConnection Failed \n"); 
				return "Null";
			} 
			send(sock , buff , strlen(buff) , 0 ); 
			printf("Client : message sent\n"); 
			valread = read( sock , buffer, 1024); 
			return buffer; 
		}

};


// Implementation to send image and camera id


class AdvancedClient_Reader_Req{
    private:
        Client cli;
    public:

        void setup(char* adr, int port){
            cli.setup(adr,port);
        }

        void send_image(char* image_buffer, int camera_id){
            cli.send_request((char*) camera_id);
            cli.send_request(image_buffer);
        }

};

class AdvancedServer_Reader_Req{
    private:
        Server srv;
    
    public:
        void setup(int port){

        }

        char * listen(){
            char * result_image;
            return result_image;
        }
};