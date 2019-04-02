#include "optFlow.h"

//global
optFlow opticalFlow = optFlow();

//construtor
optFlow::optFlow(){

}


void optFlow::run(IplImage * prev, IplImage * curr){

    double lambda = 1;
    IplImage *velx = cvCreateImage(cvGetSize(prev),32,1), *vely = cvCreateImage(cvGetSize(prev),32,1);
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 100, 1e-6);

    cvCalcOpticalFlowHS(prev,curr,0,velx,vely,lambda, criteria);

    cv::Mat x = cv::cvarrToMat(velx,true);
    cv::Mat y = cv::cvarrToMat(vely,true);
    cv::Mat img = cv::cvarrToMat(curr,true);

    cv::Mat optMap;
    cv::magnitude(x,y,optMap);

    optMap.convertTo(optMap,CV_8UC1);
    //cv::imshow("opt", optMap);

    cv::Mat filtered;

    //filtro adaptativo
    cv::adaptiveBilateralFilter(optMap,filtered,cv::Size(3,3),20);

    //limiar a partir do histograma (otsu)
    double otsu = cv::threshold(filtered,filtered,10,255,cv::THRESH_BINARY | cv::THRESH_OTSU);
    //cout << otsu << endl;

    //operacoes morfologicas

    cv::erode(filtered,filtered,cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));
    cv::dilate(filtered,filtered,cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)));

    //cv::morphologyEx(filtered,filtered,MORPH_OPEN,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)));

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

//    findContours( filtered, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE );
//    if( contours.size() == 0 )
//        return;

//    Mat dst;
//    img.copyTo(dst,filtered);

    cv::imshow("OptFlow", filtered);

}

