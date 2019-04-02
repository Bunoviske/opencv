#include "canny.h"

//construtor
canny::canny(cv::Mat img, cv::Mat gt, char * name): edgeModule(img, gt, name){

}

void canny::getEdges(){

    int ratio = 3;
    int lowThreshold = 25;
    cv::Mat detected_edges;

    cv::blur( this->img, detected_edges, cv::Size(3,3));
    //cv::GaussianBlur( this->img, detected_edges, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );

    cv::Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio);

    cv::bitwise_not(detected_edges, this->binary );

//    cv::imshow("Binary", this->binary);

//    cv::waitKey(0);
}


