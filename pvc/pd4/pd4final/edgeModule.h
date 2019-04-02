#ifndef EDGE_MODULE
#define EDGE_MODULE

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

using namespace std;


class edgeModule{

public:

    edgeModule(cv::Mat, cv::Mat, char*); //construtor
    virtual ~edgeModule(){}
    void run();


protected:

    cv::Mat img, gt, binary;
    char name[30];


    virtual void getEdges();
    void save();
    void compare();

};

#endif

