#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <time.h>
#include <fstream>
#include <opencv2/nonfree/features2d.hpp>


using namespace std;



class ImageProcessing{

public:

    /*
     * Variaveis
     */

    int flag, turn;
    cv::Point point1, point2;



    /*
     * Métodos
     */

    ImageProcessing(); //construtor
    void runVideo(cv::Mat*);
    void process(cv::Mat*);
    void setImage(cv::Mat);
    int isInsideRect(int,int);
    static void CallBackFunc(int event, int x, int y, int flags, void * img);


private:

    /*
     * Variaveis
     */

    cv::Mat imgTemplate,show;
    int offset,contGlobal;


    /*
     * Métodos
     */

    void setInterface();
    void drawMatches(cv::Mat *compImage,vector<cv::KeyPoint>& key1, vector<cv::KeyPoint>& key2,
                     std::vector<cv::DMatch>& matches, string name);
    void writeFile(float time,int keysSize, bool flag,std::vector<cv::DMatch>& matches);




};

extern ImageProcessing  keypoints;
