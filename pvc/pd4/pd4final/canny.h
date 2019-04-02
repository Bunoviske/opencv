#include "edgeModule.h"

class canny : public edgeModule{

public:

    canny(cv::Mat, cv::Mat, char*); //construtor
    ~canny(){};

private:

    void getEdges();

};
