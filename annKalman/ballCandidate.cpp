#include "ballCandidate.hpp"

float distance(int Xi, int Xf, int Yi, int Yf){
    return sqrt(pow(Xf - Xi, 2) + pow(Yf - Yi, 2));
}

std::vector<ballCandidate> findBallCandidates(bool isTopCamera, cv::Mat image, cv::Mat field)
{
    const int MIN_BALL_SIZE = isTopCamera? 14:25;
    const int MAX_BALL_SIZE = isTopCamera? 30:65;

    std::vector<ballCandidate> candidates;
    cv::Mat grad_x, grad_y, grad, highContrastAreas, img;
    cv::GaussianBlur(image,img,cv::Size(5,5),1);
    cv::cvtColor(img, grad, CV_BGR2GRAY);
    cv::Mat gray = grad.clone();

    //cv::imshow("antes", grad);
    //cv::equalizeHist(grad,grad);
    //cv::imshow("dps", grad);
    cv::Scharr(grad,grad_x,CV_16S,1,0,3);
    cv::Scharr(grad,grad_y,CV_16S,0,1,3);
    cv::convertScaleAbs(grad_x,grad_x);
    cv::convertScaleAbs(grad_y,grad_y);
    cv::addWeighted(grad_x,0.5,grad_y,0.5,0,grad);
    //cv::imshow("grad", grad);
    //cv::waitKey(0);
    cv::threshold(grad,highContrastAreas,254,255,CV_THRESH_TOZERO);

    highContrastAreas.copyTo(grad);
    cv::dilate(highContrastAreas,highContrastAreas,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));//(3,3) para longe

    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point> > contrastContours;
    cv::findContours(highContrastAreas, contrastContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    //cv::imshow("thres", highContrastAreas);


    std::vector<cv::Point2f> center(contrastContours.size());
    std::vector<float> radius(contrastContours.size());


    for (unsigned int i = 0; i < contrastContours.size(); i++)
    {
        //cv::drawContours(image, contrastContours, i, cv::Scalar(0,0,255));
        cv::minEnclosingCircle(contrastContours[i], center[i], radius[i]);
        //cv::circle(image,center[i],radius[i],cv::Scalar(0,255,0));

        //        if (cv::contourArea(contrastContours[i]) > 5000.){
        //             cv::circle(image,center[i],radius[i],cv::Scalar(0,255,0));
        //        }

        //////////////////// Detecta rects!

        cv::RotatedRect rect = cv::minAreaRect(contrastContours[i]);

        // rotated rectangle
        cv::Point2f rect_points[4]; rect.points( rect_points );

        float lx = abs(rect_points[0].x -  rect_points[(1)%4].x);
        float ly = abs(rect_points[1].y -  rect_points[(2)%4].y);
        if (lx*ly > 5000){ //calculo da area!

            cv::Rect rect1 = rect.boundingRect();

            if(rect1.x < 0) rect1.x = 0;
            if(rect1.y < 0) rect1.y = 0;
            if(rect1.x + rect1.width > image.cols){
                rect1.width = image.cols - rect1.x;
            }
            if(rect1.y + rect1.height > image.rows){
                rect1.height = image.rows - rect1.y;
            }
            cv::Mat aux = gray.clone(); //caso tenha dois retangulos sobrepostos
            cv::Mat newSearch = aux(rect1);


            cv::threshold(newSearch,newSearch,50,255,CV_THRESH_BINARY_INV);
            cv::imshow("thres", newSearch);

            cv::dilate(newSearch,newSearch,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));


            std::vector<std::vector<cv::Point> > blackContours;
            std::vector<cv::Vec4i> hierarchy1;
            cv::findContours(newSearch, blackContours, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point2f> centros;
            std::vector<float> raios;
            cv::Point2f centroAux;
            float raioAux;

            for(int ii = 0; ii < blackContours.size();ii++){
                cv::minEnclosingCircle(blackContours[ii], centroAux, raioAux);
                if (raioAux > 1 && raioAux < 50){ //se o raio for mt pequeno ou mt grande, descartar o centro dele
                    centroAux.x = centroAux.x + rect1.x;
                    centroAux.y = centroAux.y + rect1.y;
                    centros.push_back(centroAux);
                    raios.push_back(raioAux);
                    cv::circle(image,centroAux, raioAux,cv::Scalar(0,0,255));
                }
            }
            if (centros.size() > 0){
                bool continua = 0;
                while (!continua){
                    cv::Point2f finalCentro;
                    finalCentro = centros[0];
                    centros.erase(centros.begin());
                    for (int ii = 0; ii < centros.size();ii++){
                        if(distance(centros[ii].x, finalCentro.x, centros[ii].y, finalCentro.y) < 50){
                            finalCentro += centros[ii];
                            finalCentro.x /= 2;
                            finalCentro.y /= 2;
                            centros.erase(centros.begin()+ii);
                            ii--;
                        }
                    }
                    cv::circle(image,finalCentro,30,cv::Scalar(0,255,0));
                    if(centros.size() == 0){
                        continua = 1;
                    }
                }
            }

            //cv::waitKey(0);

            //            cv::rectangle(image,rect1,cv::Scalar(0,255,0));
            //            for( int j = 0; j < 4; j++ )
            //                cv::line( image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );
        }

        //        if(lx > ly){
        //            if(lx/ly > 5. && ly > 0.){
        //                for( int j = 0; j < 4; j++ )
        //                    cv::line( image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );
        //            }
        //        }
        //        else{
        //            if(ly/lx > 5. && lx > 0.){
        //                for( int j = 0; j < 4; j++ )
        //                    cv::line( image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );
        //            }
        //        }


        ////////////////////


        if (radius[i] > MIN_BALL_SIZE && radius[i] < MAX_BALL_SIZE )
            //TODO: FIND RELATION BETWEEN BALL RADIUS AND BALL CENTER.Y e descartar candidatos fora do campo
        {
            int pX = center[i].x - 0.7071*radius[i] > 0? center[i].x - 0.7071*radius[i] : 0,
                    pY = center[i].y - 0.7071*radius[i] > 0? center[i].y - 0.7071*radius[i] : 0,
                    width = center[i].x + 0.7071*radius[i] > img.cols?  img.cols - pX : 1.4142*radius[i],
                    height = center[i].y + 0.7071*radius[i] > img.rows? img.rows - pY : 1.4142*radius[i];
            //if (field.at<uchar>(pX,pY) == 255){
            cv::Rect r = cv::Rect(pX, pY, width, height);
            candidates.push_back(ballCandidate(r,image(r)));
            cv::rectangle(image, r, cv::Scalar(255,0,0), 1, 8, 0);
            //}
        }

    }

    cv::imshow("rects", image);


    return candidates;
}

