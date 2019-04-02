#include "edgeModule.h"

class laplaciano : public edgeModule{

public:

    laplaciano(cv::Mat, cv::Mat, char*); //construtor
    ~laplaciano(){}

private:

    void getEdges();
    void getBinary();
};
