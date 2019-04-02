#include "optFlow.h"

class backSub{

public:

    /*
     * Variaveis
     */


    /*
     * Métodos
     */

    backSub(); //construtor
    void run(cv::Mat*);



private:

    /*
     * Variaveis
     */

    cv::BackgroundSubtractorMOG2 mog2;



    /*
     * Métodos
     */



};

extern backSub backgroundSub;
