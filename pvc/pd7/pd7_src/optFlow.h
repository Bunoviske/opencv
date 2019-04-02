#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <time.h>
#include <opencv2/legacy/legacy.hpp>



using namespace std;
using namespace cv;



class optFlow{

public:

    /*
     * Variaveis
     */


    /*
     * Métodos
     */

    optFlow(); //construtor
    void run(IplImage * prev, IplImage * curr);



private:

    /*
     * Variaveis
     */



    /*
     * Métodos
     */



};

extern optFlow  opticalFlow;
