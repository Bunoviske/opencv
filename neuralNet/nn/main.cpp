#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>


using namespace std;
using namespace cv;

//tamanho da ROI em pixels (ROI quadrada)
#define sizes 20

Mat dataset = Mat();
Mat labels = Mat();

void getImages(String folder, float label[]){

    /*********
     *
     * Aquisicao das imagens de treino de uma pasta
     *
     *********/


    vector<String> filenames;


    cout << folder << endl;
    glob(folder, filenames);

    for(size_t i = 0; i < filenames.size(); ++i)
    {
        Mat src = imread(filenames[i]);

        if(!src.data )
            cerr << "Problem loading image!!!" << endl;
        else{

            resize(src, src, Size(sizes, sizes)); //ajustar para o tamanho usual
            cvtColor(src,src, CV_BGR2GRAY);
            Mat aux = src.reshape(0, 1); //reshape para construir Mat de uma dimensao
            dataset.push_back(aux);

            aux = Mat(1,2,CV_32FC1, label);
            labels.push_back(aux);

        }
    }

}

void create_sets(Mat *train, Mat *validation, Mat *test, Mat *data, vector<int>* random, int numTrain, int numVal){

    int cont_total = 0;
    Mat aux;

    for(int i = 0; i < numTrain ; i++){
        aux = data->row(random->at(cont_total));
        train->push_back(aux);
        cont_total++;
    }

    for(int i = 0;i <  numVal ; i++){
        aux = data->row(random->at(cont_total));
        validation->push_back(aux);
        cont_total++;
    }

//    for(int i = 0;i < numVal; i++){
//        aux = data->row(random->at(cont_total));
//        test->push_back(aux);
//        cont_total++;
//    }
}

Mat findFeatures(Mat *img){

    int lowThreshold = 25;
    int ratio = 3;

    Mat cols = img->colRange(0,img->cols);
    Mat aux(*img);
    cv::hconcat(aux, cols, aux);
    cols = img->colRange(0,1);
    cv::hconcat(aux, cols, aux);



    for (int i = 0; i < img->rows; i++){
        Mat teste(img->row(i));
        Mat detected_edges, binary;

        teste = teste.reshape(0,sizes);
        //normalize(teste,teste,0,255,CV_MINMAX);
        teste.convertTo(teste, CV_8UC1);

        cv::threshold(teste, binary, 50, 255, CV_THRESH_BINARY_INV);
        int black = cv::countNonZero(binary);

        cv::blur( teste, detected_edges, cv::Size(3,3));
        cv::Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio);


        //imshow("edges", detected_edges);
        //imshow("teste", teste);
        //imshow("bin", binary);
        //waitKey(0);

        detected_edges = detected_edges.reshape(0,1);
        detected_edges.convertTo(detected_edges, CV_32FC1);

        int k = 0;
        int j;
        for ( j = img->cols ; j < aux.cols; j++, k++){
            aux.at<float>(0,j) = detected_edges.at<float>(0,k);
        }
        aux.at<float>(0,j) = (float)black;

    }

    return aux;
}

int main(int argc, char** argv)
{
    float label1[] = {0, 1}; //label de quando eh bola

    //getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/1",label1);
    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/2", label1);
    //getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/3", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/4", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/5", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/6", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/7", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/8", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/9", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/10", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/11", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/12", label1);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/positivesClean/13", label1);


    float label2[] = {1,0}; //negativas
    getImages("/home/bruno/openCV/svmBall/convert2txt/500negatives/1",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/2",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/3",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/4",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/5",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/6",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/7",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/8",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/9",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/10",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/11",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/12",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/13",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/14",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/15",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/16",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/17",label2);
//    getImages("/home/bruno/openCV/svmBall/convert2txt/negativesClean/18",label2);


    cout << labels.size() << ' ' << dataset.size() << endl;

    //quantidade de exemplos para training, validation e test set
    int numValidationSet = 0.2 * labels.rows;
    int numTestSet = 0;//numValidationSet;
    int numTrainSet = labels.rows - (numValidationSet + numTestSet);

    //a Mat eh um vetor vertical
    Mat labelData;
    Mat labelVal;
    Mat labelTest;

    vector<int> random(labels.rows);
    for(int i = 0; i < labels.rows; i++){
        random[i] = i;
    }
    RNG rng(time(NULL));
    randShuffle(random, 1, &rng);


    create_sets(&labelData, &labelVal, &labelTest, &labels, &random, numTrainSet, numValidationSet);

    cout << labelData.rows << ' ' << labelData.cols << endl;
    cout << labelVal.rows << ' ' << labelVal.cols << endl;
    cout << labelTest.rows << ' ' << labelTest.cols << endl;

    Mat sampleData;
    Mat sampleVal;
    Mat sampleTest;


    //dataset = findFeatures(&dataset).clone();
    create_sets(&sampleData, &sampleVal, &sampleTest, &dataset, &random, numTrainSet, numValidationSet);

    cout << sampleData.rows << ' ' << sampleData.cols << endl;
    cout << sampleVal.rows << ' ' << sampleVal.cols << endl;
    cout << sampleTest.rows << ' ' << sampleTest.cols << endl;

    /*
         *
         *Conversao das matrizes para CV_32FC1 (tipo obrigatorio para funcao de treinamento - NN.train)
         *
         */

    labelData.convertTo(labelData, CV_32FC1);
    labelVal.convertTo(labelVal, CV_32FC1);
    labelTest.convertTo(labelTest, CV_32FC1);

    sampleData.convertTo(sampleData,CV_32FC1);
    sampleVal.convertTo(sampleVal,CV_32FC1);
    sampleTest.convertTo(sampleTest,CV_32FC1);

    CvANN_MLP nn;
    CvANN_MLP_TrainParams trainParams(TermCriteria(TermCriteria::MAX_ITER, 10000, 1e-6),
                                      CvANN_MLP_TrainParams::BACKPROP,
                                      0.1,0.1);
//    CvANN_MLP_TrainParams trainParams(TermCriteria(TermCriteria::MAX_ITER, 10000, 1e-6),
//                                      CvANN_MLP_TrainParams::RPROP,
//                                      0.1,FLT_EPSILON);

    int nclasses = 2;

//    Mat_<int> layers(4,1);
//    layers(0) = sampleData.cols;     // input
//    layers(1) = nclasses * 8;  // hidden
//    layers(2) = nclasses * 4;  // hidden
//    layers(3) = nclasses;      // output, 1 pin per class.

    Mat_<int> layers(3,1);
    layers(0) = sampleData.cols;     // input
    layers(1) = nclasses*16;  // hidden
    layers(2) = nclasses ;  // hidden

    //cout << layers << endl;


    nn.create(layers);

    cout << "Comeco do treino" << endl;

    Mat randomMat; //inicializar dps com valores aleatorios

    nn.train(sampleData,labelData,randomMat,Mat(),trainParams,0);

    cout << "Termino do treino" << endl;

    int valSize = sampleVal.rows;
    int acerto = 0;
    Mat responseMat;

    // Show the decision regions given by the SVM
    for (int i = 0; i < valSize; ++i){

        nn.predict(sampleVal.row(i), responseMat);
        Point maxIdx;
        minMaxLoc(responseMat,0,0,0,&maxIdx);

        //cout << labelVal.row(i) << ' ' << responseMat << endl;

        //cout << maxIdx.x << endl;

        //nao achou
        if (labelVal.at<float>(i,0) == 1 && maxIdx.x == 0){
            acerto++;
        }

        else if(labelVal.at<float>(i,1) == 1 && maxIdx.x == 1){
            //achou
            acerto++;

        }


    }
    cout << "Accuracy: " << (acerto*100)/valSize << "%" << endl;


    nn.save("annBall.xml");

    return 0;
}
