#include "processing.h"

//construtor
ImageProcessing::ImageProcessing(float focalLength, float cx, float cy){

    this->mouseFlag = 0;
    this->Xi = 0;
    this->Yi = 0;
    this->Xf = 0;
    this->Yf = 0;
    this->distancePixel = 0;
    this->f = focalLength;
    this->cx = cx;
    this->cy = cy;
    this->distanceReal = 0;
}

void ImageProcessing::drawGrid(IplImage *img){


    /*****
     *
     * Mostra o grid na imagem recebida como parametro (RAW ou UNDISTORTED)
     * Além disso, verifica se alguma reta foi traçada, e se sim, a desenha na imagem.
     *
     ******/

    // Desenho das linhas na vertical
    int offset = (img->height/3);
    for (int i = 0; i < 3; i++){
        cvLine(img, Point(0, offset), Point(img->width, offset), Scalar(255,255,255), 1, 8, 0);
        offset += (img->height/3);
    }

    // Desenho das linhas na horizontal
    offset = (img->width/3);
    for (int i = 0; i < 3; i++){
        cvLine(img, Point(offset, 0), Point(offset, img->height), Scalar(255,255,255), 1, 8, 0);
        offset += (img->width/3);
    }

    //Desenho dos circulos no meio de cada quadrado

    int x = (img->width/6);
    int y = (img->height/6);

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            cvCircle(img, Point(x,y),2,Scalar(255,255,255),1,8,0);
            y += (img->height/3);
        }
        y = (img->height/6);
        x += (img->width/3);

    }


    //Verifica se o usuario pediu para alguma linha ser traçada na imagem

    if (this->mouseFlag == 0){
        this->drawLine(img);
    }


}

void ImageProcessing::drawLine(IplImage *img){


    /*****
     *
     * Desenha uma linha entre os pontos de referencia
     *
     ******/

    cvLine(img, Point(this->Xi, this->Yi), Point(this->Xf, this->Yf), Scalar(255,0,0), 1, 8, 0);


}

void ImageProcessing::setDistance(){


    /*****
     *
     * Calcula distancia euclidiana em pixels
     *
     ******/

    this->distancePixel = sqrt(pow(this->Xf - this->Xi, 2) + pow(this->Yf - this->Yi, 2));

    cout << "Distancia Euclidiana em pixels: " << this->distancePixel << " pixels" << endl;


}

void ImageProcessing::setRealDistance(){

     /*****
     *
     * Calcula distancia euclidiana em cm
     *
     ******/

    this->distanceReal = sqrt(pow(this->XfReal - this->XiReal, 2) + pow(this->YfReal - this->YiReal, 2));

    cout << "Distancia em cm: " << this->distanceReal << " cm" << endl;

}

void ImageProcessing::setRealPos(){

    this->XiReal = ((this->Xi - this->cx)*this->z)/this->f;
    this->XfReal = ((this->Xf - this->cx)*this->z)/this->f;

    this->YiReal = ((this->Yi - this->cy)*this->z)/this->f;
    this->YfReal = ((this->Yf - this->cy)*this->z)/this->f;
}



void ImageProcessing::requisito1(){


    /*****
     *
     * Chama as funcoes que realizam o requisito 1 do projeto demonstrativo
     *
     ******/

    this->setDistance();

    //calcula a distancia real em cm
    this->setRealPos();
    this->setRealDistance();

}


//metodo de CallBack --> nao pertence à classe
void rawCallBackFunc(int event, int x, int y, int flags, void * objeto)
{
    ImageProcessing *Image = ((ImageProcessing*)objeto); //fazendo casting para (ImageProcessing*)

    if ( event == EVENT_FLAG_LBUTTON && Image->mouseFlag == 0) //primeiro clique
    {
        Image->mouseFlag = 1;

        cout << "Raw Image [Xi, Yi] = [" << x << ", " << y << "]   " << flush;

        Image->Xi = x;
        Image->Yi = y;

    }
    else if ( event == EVENT_FLAG_LBUTTON && Image->mouseFlag == 1) //segundo clique
    {
        Image->mouseFlag = 0;

        cout << "[Xf, Yf] = [" << x << ", " << y << "]   " << endl;

        Image->Xf = x;
        Image->Yf = y;

        Image->requisito1();

    }
}

//metodo de CallBack --> nao pertence à classe
void undistortCallBackFunc(int event, int x, int y, int flags, void * objeto)
{
    ImageProcessing *Image = ((ImageProcessing*)objeto); //fazendo casting para (ImageProcessing*)

    if ( event == EVENT_FLAG_LBUTTON && Image->mouseFlag == 0) //primeiro clique
    {
        Image->mouseFlag = 1;

        cout << "Undistort Image [Xi, Yi] = [" << x << ", " << y << "]   " << flush;

        Image->Xi = x;
        Image->Yi = y;

    }
    else if ( event == EVENT_FLAG_LBUTTON && Image->mouseFlag == 1) //segundo clique
    {
        Image->mouseFlag = 0;

        cout << "[Xf, Yf] = [" << x << ", " << y << "]   " << endl;

        Image->Xf = x;
        Image->Yf = y;

        Image->requisito1();

    }
}

