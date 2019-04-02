#include "processing.h"

//global
ImageProcessing keypoints = ImageProcessing();

fstream harris;
fstream surfi;

//construtor
ImageProcessing::ImageProcessing(){
    offset = 25;
    flag = 0;
    turn = 1;
    contGlobal = 1;

    FILE *fp;
    //inicializa os arquivos em branco

    fp = fopen("harris.txt", "w");
    fclose(fp);
    fp = fopen("surf.txt", "w");
    fclose(fp);

}

void ImageProcessing::runVideo(cv::Mat* img){
    this->show = img->clone();
    //this->setInterface();
    cv::imshow("Video", this->show);

    if (keypoints.flag){
        keypoints.process(img);
    }
}

void ImageProcessing::process(cv::Mat* compImage){

    harris.open("harris.txt", ios_base::out | ios_base::app);
    surfi.open("surf.txt", ios_base::out | ios_base::app);
    harris << endl << "Frame " << contGlobal << endl << endl;
    surfi << endl << "Frame " << contGlobal << endl << endl;
    contGlobal++;

    //Detectores
    vector<cv::KeyPoint> key1Harris, key2Harris, key1Surf, key2Surf;
    cv::GoodFeaturesToTrackDetector gftt(400,0.01,10);
    cv::SurfFeatureDetector surf(25);

    gftt.detect(this->imgTemplate,key1Harris);
    gftt.detect(*compImage,key2Harris);

    surf.detect(this->imgTemplate,key1Surf);
    surf.detect(*compImage,key2Surf);

    //Descritores
    cv::SurfDescriptorExtractor surfDesc;
    cv::Mat SurfdescriptorsTemplate, SurfdescriptorsComp, HarrisdescriptorsComp, HarrisdescriptorsTemplate;

    surfDesc.compute(this->imgTemplate,key1Harris,HarrisdescriptorsTemplate);
    surfDesc.compute(*compImage,key2Harris,HarrisdescriptorsComp);

    surfDesc.compute(this->imgTemplate,key1Surf,SurfdescriptorsTemplate);
    surfDesc.compute(*compImage,key2Surf,SurfdescriptorsComp);

    //Matches
    cv::BFMatcher matcher;
    std::vector<cv::DMatch> matches1,matches2;

    //Harris matching
    //matcher.match(HarrisdescriptorsTemplate,HarrisdescriptorsComp, matches1);
    //Surf matching
    //matcher.match(SurfdescriptorsTemplate,SurfdescriptorsComp, matches2);



    for (int i = 1; i <= 1000; i += i == 1 ? 9:10 ){

        if ( i < HarrisdescriptorsComp.rows){
            cv::Mat aux = HarrisdescriptorsComp.rowRange(0,i);

            clock_t timer = clock();
            matcher.match(HarrisdescriptorsTemplate,aux, matches1);
            timer = clock() - timer;

            writeFile(((float)timer)/CLOCKS_PER_SEC*1000,i,0,matches1);
        }

        if (i < SurfdescriptorsComp.rows){
            cv::Mat aux = SurfdescriptorsComp.rowRange(0,i);

            clock_t timer = clock();
            matcher.match(SurfdescriptorsTemplate,aux, matches2);
            timer = clock() - timer;

            writeFile(((float)timer)/CLOCKS_PER_SEC*1000,i,1,matches2);

        }

    }

    harris.close();
    surfi.close();



    this->drawMatches(compImage,key1Harris,key2Harris,matches1,"harrisMatches");
    this->drawMatches(compImage,key1Surf,key2Surf,matches2,"surfMatches");


}

void ImageProcessing::writeFile(float time,int keysSize, bool flag,std::vector<cv::DMatch>& matches){

    float thres = 1;

    //harris
    if (flag == 0){
        int cont = 0;
        for (int i = 0; i < matches.size(); i++){
            if (matches[i].distance < thres){
                cont++;
            }
        }
        harris << "Tempo para processar " << keysSize << " keypoints: " << time
               << " ms. Foram correspondidos " << cont << " keypoints "<< endl;
    }
    //surf
    else{
        int cont = 0;
        for (int i = 0; i < matches.size(); i++){
            if (matches[i].distance < thres){
                cont++;
            }
        }
        surfi << "Tempo para processar " << keysSize << " keypoints: " << time
              << " ms. Foram correspondidos " << cont << " keypoints "<< endl;
    }


}

void ImageProcessing::drawMatches(cv::Mat *compImage,vector<cv::KeyPoint>& key1, vector<cv::KeyPoint>& key2,
                                  std::vector<cv::DMatch>& matches, string name){

    if (matches.size() >= 24) {
        int value = 24;
        matches.erase(matches.begin()+value, matches.end());
    }

    if (key1.size() == 0 || key2.size() == 0){
        cout << "Escolha um template maior - nenhum keypoint detectado" << endl;
    }
    else{
        cv::Mat imageMatches;
        cv::drawMatches(
                    this->imgTemplate,key1, // 1st image and its keypoints
                    *compImage,key2, // 2nd image and its keypoints
                    matches, //the matches
                    imageMatches,    // the image produced
                    cv::Scalar(255,255,255)); // color of the lines

        cv::imshow(name, imageMatches);
    }
}



void ImageProcessing::setImage(cv::Mat img){
    this->imgTemplate = img.clone();
}



void ImageProcessing::CallBackFunc(int event, int x, int y, int flags, void * img)
{
    cv::Mat Image = *((cv::Mat*)img); //fazendo casting para (Mat)

    if ( event == cv::EVENT_FLAG_LBUTTON){

        //os pontos clicados determinam a roi do objeto

        if (keypoints.turn == 1){
            keypoints.turn = 2;
            keypoints.point1 = cv::Point(x,y);
        }
        else{
            keypoints.turn = 1;
            keypoints.point2 = cv::Point(x,y);

            cv::Rect ROI = cv::Rect(keypoints.point1, keypoints.point2);

            keypoints.setImage(Image(ROI));
            keypoints.flag = 1;

        }
        //        if (keypoints.isInsideRect(x,y) == 1){

        //            keypoints.setImage(Image);
        //            keypoints.flag = 1;
        //            cout << "Template salvo." << endl;
        //        }
        //        else if (keypoints.isInsideRect(x,y) == 2){

        //            if (keypoints.flag){
        //                cout << "Iniciando analise." << endl;

        //                keypoints.process(Image);
        //            }
        //            else{
        //                cout << "Insira um template antes de fazer a analise." << endl;
        //            }
        //        }


    }
}


int ImageProcessing::isInsideRect(int x, int y){
    if (x >= show.cols - 4*offset && y >= show.rows - offset)
        return 1;
    else if (x < 4*offset && y >= show.rows - offset)
        return 0;
    else
        return 0;
}

void ImageProcessing::setInterface(){
    cv::Point p(show.cols - 3*offset, show.rows - offset/2);
    cv::Rect rec(show.cols - 4*offset, show.rows - offset, 4*offset, offset);
    cv::rectangle(this->show,rec,cv::Scalar(255,255,255),-1);
    cv::putText(this->show,"Capture",p,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0));


    p = cv::Point(offset/3, show.rows - offset/2);
    rec = cv::Rect(0, show.rows - offset, 4*offset, offset);
    cv::rectangle(this->show,rec,cv::Scalar(255,255,255),-1);
    cv::putText(this->show,"Analyze",p,cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0));
}
