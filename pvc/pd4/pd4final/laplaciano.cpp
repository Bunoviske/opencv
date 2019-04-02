#include "laplaciano.h"

//construtor
laplaciano::laplaciano(cv::Mat img, cv::Mat gt, char * name): edgeModule(img, gt, name){

}

void laplaciano::getBinary(){

    int otsu = cv::threshold( this->binary, this->binary, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY_INV );
    //cv::adaptiveThreshold(this->binary, this->binary, 255,
    //                       cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);
    //cout << "OTSU: " << otsu << endl;
}

void laplaciano::getEdges(){

    int scale = 1;
    int delta = 0;
    int ddepth = CV_8U;

    cv::GaussianBlur( this->img, this->img, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    //cv::blur( this->img, this->img, cv::Size(3,3));

    cv::Laplacian( this->img, this->binary, ddepth, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( this->binary, this->binary );

//    cv::imshow("laplace", this->binary);
//    cv::waitKey(0);

    this->getBinary();
}


