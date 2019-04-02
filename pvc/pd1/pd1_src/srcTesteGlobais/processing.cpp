#include "processing.h"

void CallBackFunc(int event, int x, int y, int flags, void* img)
{
    Mat *imagem = ((Mat*)img); //derreferenciando e fazendo casting para (Mat*)


    if ( flags == EVENT_FLAG_LBUTTON )
    {
        cout << "Posicao do mouse (" << x << ", " << y << ")    ";

        imagem[MASK] = Scalar(1,1,1); //reseta a mascara toda vez que um botao for apertado

        //requisito 1
        pixelsIntensity(imagem, x, y);
        multiply(imagem[ORIGINAL], imagem[MASK], imagem[RED]);
        imshow("Imagem", imagem[RED]);
    }
}

int grayScale(Mat *img){


    /*****
     *
     * Analisa toda imagem e verifica se todos os pixels dela sao R = G = B
     *
     ******/


    Mat channels[3];
    Mat difference[2];

    split(img[ORIGINAL], channels);

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

void pixels2red(Vec3b *pixel, Mat *img, int gray){

    /*****
     *
     * Analisa toda imagem e verifica quais pixels estao dentro do intervalo de 5%
     *
     ******/

    //avalia apenas o primeiro canal
    if (gray){
        float upBound = 1.05 * (int)pixel->val[0];
        float botBound = 0.95 * (int)pixel->val[0];

        for (int i = 0; i < img[ORIGINAL].rows; i++ ){
            for (int j = 0; j < img[ORIGINAL].cols; j++){
                if (img[ORIGINAL].at<Vec3b>(i,j).val[0] <= upBound &&  img[ORIGINAL].at<Vec3b>(i,j).val[0] >= botBound ){
                    img[MASK].at<Vec3b>(i,j).val[0] = 0;
                    img[MASK].at<Vec3b>(i,j).val[1] = 0;
                    img[MASK].at<Vec3b>(i,j).val[2] = 255; // vermelho
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


        for (int i = 0; i < img[ORIGINAL].rows; i++ ){
            for (int j = 0; j < img[ORIGINAL].cols; j++){
                if (img[ORIGINAL].at<Vec3b>(i,j).val[0] <= upBound[0] &&  img[ORIGINAL].at<Vec3b>(i,j).val[0] >= botBound[0] &&
                    img[ORIGINAL].at<Vec3b>(i,j).val[1] <= upBound[1] &&  img[ORIGINAL].at<Vec3b>(i,j).val[1] >= botBound[1] &&
                    img[ORIGINAL].at<Vec3b>(i,j).val[2] <= upBound[2] &&  img[ORIGINAL].at<Vec3b>(i,j).val[2] >= botBound[2]){

                        img[MASK].at<Vec3b>(i,j).val[0] = 0;
                        img[MASK].at<Vec3b>(i,j).val[1] = 0;
                        img[MASK].at<Vec3b>(i,j).val[2] = 255; // vermelho
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

    Vec3b *pixel = img[ORIGINAL].ptr<Vec3b>(y,x);
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
