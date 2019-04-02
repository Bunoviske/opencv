#include "backSub.h"

//global
backSub backgroundSub = backSub();

//construtor
backSub::backSub(){

    mog2 = cv::BackgroundSubtractorMOG2(0,10,true);

}

void backSub::run(cv::Mat* img){

    cv::Mat mask;

    mog2.operator ()(*img, mask);

    cv::erode(mask,mask,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    cv::dilate(mask,mask,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    //cv::dilate(mask,mask,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));
    //cv::morphologyEx(mask,mask,MORPH_OPEN,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv::imshow("BackSub", mask);

    //    findContours( mask, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
    //    if( contours.size() == 0 )
    //        return;

    //    //drawContours( *img, contours, 0,Scalar(255,0,0),10);
    //    Mat dst;
    //    img->copyTo(dst,mask);



}
