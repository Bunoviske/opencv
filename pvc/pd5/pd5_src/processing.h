#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

using namespace std;


class ImageProcessing{

public:

    /*
     * Variaveis
     */


    /*
     * Métodos
     */

    ImageProcessing(); //construtor
    void run(cv::Mat*);


private:

    /*
     * Variaveis
     */

    cv::Mat img, glcm;

    /*
     * Métodos
     */

    //parametros representam offset - o primeiro eh row_offset e o segundo col_offset
    void findPixels(int row_offset, int col_offset);

    void save(string);



};

extern ImageProcessing  texture;
