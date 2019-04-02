#include "edgeModule.h"

//construtor
edgeModule::edgeModule(cv::Mat img, cv::Mat gt, char * name){

    this->img = img.clone();
    this->gt = gt.clone();
    strcpy(this->name, name);

}

void edgeModule::run(){

    this->getEdges();
    this->compare();
    this->save();

}

void edgeModule::getEdges(){

    //implements here
}

void edgeModule::compare(){

    cv::Mat comparison;

    cv::compare(this->binary, this->gt, comparison, cv::CMP_EQ);

    comparison = comparison/255;
    int sum = cv::sum(comparison)[0];
    int total = comparison.rows*comparison.cols;

    cout << "Precisao: " << (sum*100)/total << '%' << endl;
}


void edgeModule::save(){

    cv::imwrite(this->name, this->binary);
}
