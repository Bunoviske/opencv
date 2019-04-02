#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

using namespace std;

#define FOCO 25
#define BASELINE 120
#define THRESHOLD 5000

class ImageProcessing{

public:

    /*
     * Variaveis
     */


    /*
     * Métodos
     */

    ImageProcessing(cv::Mat, cv::Mat, char*); //construtor
    void run(int);


private:

    /*
     * Variaveis
     */

    cv::Mat left, right, depth;
    char name[30];
    int W, contClass;
    vector<cv::Point> point;
    vector<float> cost;

    /*
     * Métodos
     */

    void findPixels(cv::Mat * Template);
    void compareRight(int epiLine, cv::Mat * TemplateLeft);
    void setTemplate(int i, int j, cv::Mat * Template, char c);
    void depthImage();
    float calculateDistance(int);
    void save();



};
