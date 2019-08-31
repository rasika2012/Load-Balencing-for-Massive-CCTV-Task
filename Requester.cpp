class RequestHandler{
    
    private:
        // last time to complete task
        int process_time[]; 

        // current task ehich not processed by unit
        int que_lenght[]; 

        //this process image using open cv
        void process_imge(int ** mat);

    public:

        void setup(int port);

        // listening for  captured feed and forward to least process time subprocess
            // it soud be calculated by least value of sum (que length * process time)

        // this should reply input reader with processed images
        void start_listening(); 
};