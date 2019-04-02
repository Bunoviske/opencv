#include "sobel.h"

//construtor
sobel::sobel(cv::Mat img, cv::Mat gt, char * name) : edgeModule(img, gt, name){

}

void sobel::getBinary(){

    int otsu = cv::threshold( this->binary, this->binary, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY_INV );
    //cv::adaptiveThreshold(this->binary, this->binary, 255,
    //                       cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);

    //cout << "OTSU: " << otsu << endl;
}

void sobel::getEdges(){

    int scale = 1;
    int delta = 0;
    int ddepth = CV_8U;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    cv::GaussianBlur( this->img, this->img, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    //cv::blur( this->img, this->img, cv::Size(3,3));

    // Gradient X
    //Scharr( this->img, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT );
    cv::Sobel( this->img, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_x, abs_grad_x );

    // Gradient Y
    //Scharr( this->img, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT );
    cv::Sobel( this->img, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    cv::convertScaleAbs( grad_y, abs_grad_y );

    // Total Gradient (approximate)
    cv::addWeighted( abs_grad_x, 1, abs_grad_y, 1, 0, this->binary );

//    cv::imshow("sobel", this->binary);
//    cv::waitKey(0);

    this->getBinary();

}
