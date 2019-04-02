#include "backSub.h"

int main(int argc, char** argv)
{

    cv::VideoCapture capAux;
    int fps;

    CvCapture *cap;
    IplImage *aux;

    if (argc > 1){
        capAux.open(argv[1]);
        //cap = cvCaptureFromAVI(argv[1]);
        cap = cvCaptureFromFile(argv[1]);
        fps = capAux.get(CV_CAP_PROP_FPS);

    }
    else{
        cout << "Insira um video" << endl;
        return -1;
    }


    aux = cvQueryFrame(cap);

    IplImage *prev = cvCreateImage(cvGetSize(aux),8,1);
    IplImage *curr = cvCreateImage(cvGetSize(aux),8,1);

    cvCvtColor(aux,prev, CV_BGR2GRAY);
    aux = cvQueryFrame(cap);

    while (aux){
        cvCvtColor(aux,curr, CV_BGR2GRAY);


        cv::Mat img = cv::cvarrToMat(curr);

        double t = (double)cvGetTickCount();
        opticalFlow.run(prev, curr);
        t = (double)cvGetTickCount() - t;
        printf( "Tempo backSub = %g ms        ", t/((double)cvGetTickFrequency()*1000.) );

        t = (double)cvGetTickCount();
        backgroundSub.run(&img);
        t = (double)cvGetTickCount() - t;
        printf( "Tempo Horn-Shunck = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );

        cv::imshow("Video", img);

        int c = cv::waitKey(1000/fps);

        if( (char)c == 27 ) //esc
        {
            break;
        }


        prev = cvCloneImage(curr);
        aux = cvQueryFrame(cap);

    }


    return 0;

}
