#include "edgeModule.h"

class sobel: public edgeModule {

public:

    sobel(cv::Mat, cv::Mat, char*); //construtor
    ~sobel() { }

private:

    void getEdges();
    void getBinary();

};
