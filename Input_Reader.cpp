class CaptureCamera{
    private:
    public:
        // get input from camera or vedio file
        void setup_camera();

        // feed from socket server
        void start_feeding(char* adr, int socket);

        // listening to socket server
        void start_listening();
};


