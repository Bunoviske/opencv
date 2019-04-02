#include "processing.h"

//construtor
ImageProcessing::ImageProcessing(){

    for(int i = 0; i < 3;i++){
        upBound[i] = -1;
        botBound[i] = -1;
    }

    gray = 0;
}

int ImageProcessing::grayScale(){


    /*****
     *
     * Analisa toda imagem e verifica se todos os pixels dela sao R = G = B
     *
     ******/


    Mat channels[3];
    Mat difference[2];

    split(img, channels);

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



void ImageProcessing::pixels2red(){

    /*****
     *
     * Analisa toda imagem e verifica quais pixels estao dentro do intervalo de 5%, mudando estes para vermelho
     *
     ******/

    //avalia apenas o primeiro canal
    if (gray){

        for (int i = 0; i < img.rows; i++ ){
            for (int j = 0; j < img.cols; j++){
                if (img.at<Vec3b>(i,j).val[0] <= upBound[0] &&  img.at<Vec3b>(i,j).val[0] >= botBound[0] ){

                    red.at<Vec3b>(i,j).val[0] = 0;
                    red.at<Vec3b>(i,j).val[1] = 0;
                    red.at<Vec3b>(i,j).val[2] = 255; // vermelho
                }
            }
        }
    }
    //avalia os tres canais
    else{

        for (int i = 0; i < img.rows; i++ ){
            for (int j = 0; j < img.cols; j++){
                if (img.at<Vec3b>(i,j).val[0] <= upBound[0] &&  img.at<Vec3b>(i,j).val[0] >= botBound[0] &&
                    img.at<Vec3b>(i,j).val[1] <= upBound[1] &&  img.at<Vec3b>(i,j).val[1] >= botBound[1] &&
                    img.at<Vec3b>(i,j).val[2] <= upBound[2] &&  img.at<Vec3b>(i,j).val[2] >= botBound[2]){

                        red.at<Vec3b>(i,j).val[0] = 0;
                        red.at<Vec3b>(i,j).val[1] = 0;
                        red.at<Vec3b>(i,j).val[2] = 255; // vermelho
                }
            }
        }
    }
}

void ImageProcessing::setBoundaries(Vec3b *pixel){

    /*****
     *
     * Atribui os valores maximos e minimos do intervalo de 5%
     *
     ******/

    //avalia o primeiro canal
    if (gray){
        upBound[0] = 1.05 * (int)pixel->val[0];
        botBound[0] = 0.95 * (int)pixel->val[0];

    }
    //avalia os tres canais
    else{
        for (int k = 0; k < 3; k++){
            upBound[k] = 1.05 *  (int)pixel->val[k];
            botBound[k] = 0.95 *  (int)pixel->val[k];
        }
    }
}

void ImageProcessing::pixelsIntensity(int x, int y){

    /*****
     *
     * Verifica a intensidade dos pixels da imagem. Se for grayscale, mostra-se apenas a intensidade
     *
     ******/

    Vec3b *pixel = img.ptr<Vec3b>(y,x);
    gray = grayScale();

    //grayscale
    if (gray){
        cout << "Pixel Grayscale. Intensidade: " << (int)pixel->val[0] << endl;
    }
    //BGR
    else{

        cout << "Pixel BGR. B: " << (int)pixel->val[0];
        cout << " G: " << (int)pixel->val[1];
        cout << " R: " << (int)pixel->val[2] << endl;

    }

    //requisito dois
    setBoundaries(pixel);
}


//metodo de CallBack --> nao pertence à classe
void CallBackFunc(int event, int x, int y, int flags, void * objeto)
{
    ImageProcessing *Image = ((ImageProcessing*)objeto); //fazendo casting para (ImageProcessing*)

    if ( event == EVENT_FLAG_LBUTTON )
    {
        cout << "Posicao do mouse (" << x << ", " << y << ")    ";

        //requisito 1
        Image->pixelsIntensity(x, y);

        Image->red = Image->img.clone();

        //aplicacao da cor vermelha
        Image->pixels2red();

        //show the image
        imshow("Imagem", Image->red);
    }
}
