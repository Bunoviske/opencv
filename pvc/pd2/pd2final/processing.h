#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

//Métodos que nao sao da classe
void rawCallBackFunc(int event, int x, int y, int flags, void * objeto);
void undistortCallBackFunc(int event, int x, int y, int flags, void * objeto);


class ImageProcessing{

public:

    /*
     * Variaveis
     */

    int mouseFlag; //flag que define se eh o primeiro clique do mouse ou o segundo
    int Xi,Yi,Xf,Yf, distancePixel;
    float f,cx,cy,z;


    /*
     * Métodos
     */

    ImageProcessing(float, float, float); //construtor
    void requisito1();
    void drawGrid(IplImage*);




private:

    /*
     * Variaveis
     */

    float XiReal,YiReal,XfReal,YfReal;
    float distanceReal;

    /*
     * Métodos
     */

    void setDistance();
    void drawLine(IplImage*);
    void setRealPos();
    void setRealDistance();

};
