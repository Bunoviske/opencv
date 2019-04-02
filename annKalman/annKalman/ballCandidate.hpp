#include <fstream>
#include <time.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

typedef struct ballCandidate {
    cv::Rect rect;
    cv::Mat roi;
    cv::Point center;

    ballCandidate(cv::Rect rect, cv::Mat image) {
        this->rect = rect;
        this->roi = image.clone();
        //cv::resize(image, this->roi, cv::Size(30,30));
        this->center = cv::Point(rect.x + rect.width/2, rect.y + rect.height/2);
    }
    ballCandidate(){}

} ballCandidate;


std::vector<ballCandidate> findBallCandidates(bool isTopCamera, cv::Mat image, cv::Mat field);
