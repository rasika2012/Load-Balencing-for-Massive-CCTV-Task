#ifndef INCLUDE_CAPTURECAMERA_HPP
#define INCLUDE_CAPTURECAMERA_HPP
class CaptureCamera{
    private:
    public:
        // get input from camera or vedio file
        virtual ~setup_camera(){};

        // feed from socket server
        virtual ~start_feeding(char* adr, int socket){};

        // listening to socket server
        virtual ~start_listening(){};
};

#endif
