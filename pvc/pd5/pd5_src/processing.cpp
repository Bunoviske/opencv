#include "processing.h"

//global
ImageProcessing  texture = ImageProcessing();

//construtor
ImageProcessing::ImageProcessing(){
    this->glcm = cv::Mat(256,256,CV_32FC1, cv::Scalar(0));
}

void ImageProcessing::run(cv::Mat* img){

    this->img = img->clone();
    cv::Mat show(256,256, CV_8UC1);


    this->findPixels(0,1); //parametros representam offset - o primeiro eh row_offset e o segundo col_offset

    int sum = cv::sum(this->glcm)[0];
    this->glcm = this->glcm/sum;

    this->save("offset[0 1].xml");

    cv::normalize(glcm,show,0,255,CV_MINMAX);
    cv::imshow("0 grau",show);


    this->glcm = cv::Scalar(0);
    this->findPixels(-1,1);

    sum = cv::sum(this->glcm)[0];
    this->glcm = this->glcm/sum;

    this->save("offset[-1 1].xml");

    cv::normalize(glcm,show,0,255, CV_MINMAX);
    cv::imshow("45 graus",show);
    cv::waitKey(0);



}


void ImageProcessing::findPixels(int row_offset, int col_offset){

    for(int i = 0; i < this->img.rows; i++){
        for(int j = 0; j < this->img.cols; j++){

            int pixelIntensity = (int)img.at<uchar>(i,j);

            //cout << " pixel " << pixelIntensity << endl;

            if (i + row_offset >= 0 && i + row_offset < img.rows &&
                j + col_offset >= 0 && j + col_offset < img.cols){

                int neighborIntensity = (int)img.at<uchar>(i+row_offset,j+col_offset);

                this->glcm.at<float>(pixelIntensity, neighborIntensity)++;

                //cout << neighborIntensity << endl;

            }

        }
    }

    //cout << this->glcm << endl;
}


void ImageProcessing::save(string name){



    cv::FileStorage file(name, cv::FileStorage::WRITE);
    file << "GLCM" << this->glcm;
    file.release();

//    cv::FileStorage read(name, cv::FileStorage::READ);
//    read["GLCM"] >> teste;
//    read.release();




}


