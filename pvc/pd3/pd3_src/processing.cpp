#include "processing.h"

//construtor
ImageProcessing::ImageProcessing(cv::Mat left, cv::Mat right, char * name){

    this->left = left;
    this->right = right;
    strcpy(this->name, name);
    this->contClass = 0;
    this->point.reserve(left.cols * left.rows);
    this->cost.reserve(left.cols * left.rows);

    cv::Mat depth(left.rows, left.cols, CV_32FC1);
    this->depth = depth.clone();

}

void ImageProcessing::run(int W){

    this->W = W;
    cv::Mat Template(W, W, CV_8UC1);
    this->findPixels(&Template);
    this->depthImage();

}

void ImageProcessing::findPixels(cv::Mat * Template){

    this->contClass = 0;

    for(int i = 0; i < left.rows; i++){
        for(int j = 0; j < left.cols; j++){
            this->setTemplate(i,j,Template, 'L');
            this->compareRight(i, Template);
            this->contClass++;
        }
    }
}

void ImageProcessing::compareRight(int epiLine, cv::Mat * TemplateLeft){

    cv::Mat TemplateRight(this->W,this->W, CV_8UC1);
    cv::Mat result;

    this->setTemplate(epiLine, 0, &TemplateRight, 'R');
    cv::absdiff(*TemplateLeft, TemplateRight, result);

    cost[this->contClass] = cv::sum(result)[0];
    point[this->contClass] = cv::Point(0, epiLine);


    for(int j = 1; j < right.cols; j++){

        this->setTemplate(epiLine,j, &TemplateRight, 'R');
        cv::absdiff(*TemplateLeft, TemplateRight, result);
        float aux = cv::sum(result)[0];

        if (aux < cost[this->contClass]){
            cost[this->contClass] = aux;
            point[this->contClass] = cv::Point(j, epiLine);


        }
    }
}

void ImageProcessing::setTemplate(int i, int j, cv::Mat * Template, char img){

    int halfW = (this->W - 1)/2;
    int ii = 0, jj = 0;

    if (img == 'L'){

        for(int k = i - halfW, ii = 0; k <= i + halfW; ii++, k++){
            for(int l = j - halfW, jj = 0; l <= j + halfW; jj++, l++){

                if ((k < 0) || (l < 0) || (k >= left.rows) || (l >= left.cols)){
                    Template->at<uchar>(ii,jj) = left.at<uchar>(i,j);
                }
                else{
                    Template->at<uchar>(ii,jj) = left.at<uchar>(k,l);
                }
            }
        }
    }
    else{

        for(int k = i - halfW, ii = 0; k <= i + halfW; ii++, k++){
            for(int l = j - halfW, jj = 0; l <= j + halfW; jj++, l++){

                if ((k < 0) || (l < 0) || (k >= right.rows) || (l >= right.cols)){
                    Template->at<uchar>(ii,jj) = right.at<uchar>(i,j);
                }
                else{
                    Template->at<uchar>(ii,jj) = right.at<uchar>(k,l);
                }
            }
        }
    }
}

void ImageProcessing::depthImage(){

    this->contClass = 0;

    for (int i = 0; i < left.rows; i++){
        for (int j = 0; j < left.cols; j++){

   //         if (cost[this->contClass] < THRESHOLD) {

                depth.at<float>(i, j) = this->calculateDistance(j);

   //         }
   //         else{
   //             depth.at<float>(i, j) = 0; //se nao houver pixel correspondente, preenche com preto
   //         }

            this->contClass++;
        }
    }

    this->save();
}

float ImageProcessing::calculateDistance(int j){

//    cout << j << ' ' << point[this->contClass].x << endl;
//    getchar();
//    cin.get();

    if(j - point[this->contClass].x > 0){
        return (BASELINE*FOCO)/(j - point[this->contClass].x);
    }
    return 0;
}

void ImageProcessing::save(){

    cv::Mat final(this->depth.rows, this->depth.cols, CV_8UC1);
    depth.convertTo(final, CV_8UC1);
    //cv::imshow("depth",final);
    //cv::imshow("left",left);
    //cv::imshow("right",right);
    //cv::waitKey(0);
    cv::imwrite(this->name, final);
}
