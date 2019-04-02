#include "processing.h"

int main(int argc, char** argv)
{

    cv::VideoCapture cap;
    cv::Mat img;
    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
    cap.open(-1);
    cv::setMouseCallback("Video", ImageProcessing::CallBackFunc, &img);
    cout << "Dê dois cliques com mouse para capturar um objeto de modelo" << endl;

    while (cap.read(img)){

        cv::cvtColor(img,img, CV_BGR2GRAY);

        //cv::resize(img,img,cv::Size(img.cols/2,img.rows/2));

        keypoints.runVideo(&img);

        int c = cv::waitKey(33);

        if( (char)c == 27 ) //esc
        {
            break;
        }

    }



    return 0;

}
