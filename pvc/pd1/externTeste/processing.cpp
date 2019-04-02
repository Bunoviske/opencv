#include "processing.h"

//mascara(0,0, CV_8UC3);//(1, 1, CV_8UC3); //mascara global
//red(0,0, CV_8UC3); // matriz auxiliar que mostrará os pixels vermelhos


int grayScale(Mat *img){


    /*****
     *
     * Analisa toda imagem e verifica se todos os pixels dela sao R = G = B
     *
     ******/


    Mat channels[3];
    Mat difference[2];

    split(*img, channels);

    absdiff( channels[0], channels[1], difference[0]);

    if (countNonZero(difference[0]) == 0 ){

        absdiff( channels[0], channels[2], difference[1]);

        if (countNonZero(difference[1]) == 0){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }

}

void pixels2red(Vec3b *pixel, Mat *imagem, int gray){

    /*****
     *
     * Analisa toda imagem e verifica quais pixels estao dentro do intervalo de 5%
     *
     ******/

    //avalia apenas o primeiro canal
    if (gray){
        float upBound = 1.05 * (int)pixel->val[0];
        float botBound = 0.95 * (int)pixel->val[0];

        for (int i = 0; i < imagem->rows; i++ ){
            for (int j = 0; j < imagem->cols; j++){
                if (imagem->at<Vec3b>(i,j).val[0] < upBound &&  imagem->at<Vec3b>(i,j).val[0] > botBound ){
                    mascara.at<Vec3b>(i,j).val[0] = 0;
                    mascara.at<Vec3b>(i,j).val[1] = 0;
                    mascara.at<Vec3b>(i,j).val[2] = 255; // vermelho
                }
            }
        }
    }
    //avalia os tres canais
    else{
        float upBound[3];
        float botBound[3];

        for (int k = 0; k < 3; k++){
            upBound[k] = 1.05 *  (int)pixel->val[k];
            botBound[k] = 0.95 *  (int)pixel->val[k];
        }


        for (int i = 0; i < imagem->rows; i++ ){
            for (int j = 0; j < imagem->cols; j++){
                if (imagem->at<Vec3b>(i,j).val[0] < upBound[0] &&  imagem->at<Vec3b>(i,j).val[0] > botBound[0] &&
                    imagem->at<Vec3b>(i,j).val[1] < upBound[1] &&  imagem->at<Vec3b>(i,j).val[1] > botBound[1] &&
                    imagem->at<Vec3b>(i,j).val[2] < upBound[2] &&  imagem->at<Vec3b>(i,j).val[2] > botBound[2]){

                        mascara.at<Vec3b>(i,j).val[0] = 0;
                        mascara.at<Vec3b>(i,j).val[1] = 0;
                        mascara.at<Vec3b>(i,j).val[2] = 255; // vermelho
                }
            }
        }
    }
}


void pixelsIntensity(Mat *img, int x, int y){

    /*****
     *
     * Verifica a intensidade dos pixels da imagem. Se for grayscale, mostra-se apenas a intensidade
     *
     ******/

    Vec3b *pixel = img->ptr<Vec3b>(y,x);
    int flag = grayScale(img);

    //grayscale
    if (flag){
        cout << "Pixel Grayscale. Intensidade: " << (int)pixel->val[0] << endl;
    }
    //BGR
    else{

        cout << "Pixel BGR. B: " << (int)pixel->val[0];
        cout << " G: " << (int)pixel->val[1];
        cout << " R: " << (int)pixel->val[2] << endl;

    }

    //requisito dois
    pixels2red(pixel, img, flag);
}
