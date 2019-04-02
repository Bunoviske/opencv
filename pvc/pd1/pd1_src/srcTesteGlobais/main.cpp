#include "processing.h"




int delay(int argc, int fps){

    if (argc > 1){
        //se for negativo, eh uma foto. Senao, sera dado delay correspondente ao fps do video
        return waitKey(1000/fps);
    }
    else{
        //como eh streaming, usa-se 30fps
        return waitKey(33);
    }
}


int main(int argc, char** argv)
{
    VideoCapture cap;
    Mat img[3];
    namedWindow("Imagem", WINDOW_AUTOSIZE);

    // Le imagem/video do terminal
    if (argc > 1){
        cap.open(argv[1]);
    }

    // Se nao tiver argumentos de entrada, é aberto streaming de video
    else
    {
        cap.open(-1);
    }

    int fps = -1;
    if (cap.get(CV_CAP_PROP_FRAME_COUNT) > 1){
            fps = cap.get(CV_CAP_PROP_FPS);
    }

    int aux = 1;

    while(cap.read(img[ORIGINAL]))
    {
        //entra nessa condicao apenas um vez, para criar a mascara com tamanho baseado na imagem de entrada
        if (aux){
            aux = 0;
			img[MASK] = img[ORIGINAL].clone();
            img[MASK] = Scalar(1,1,1);
            img[RED] = img[ORIGINAL].clone();
        }

        setMouseCallback("Imagem", CallBackFunc, img);


        //aplicacao da mascara
        multiply(img[ORIGINAL], img[MASK], img[RED]);

        //show the image
        imshow("Imagem", img[RED]);

        int c = delay(argc, fps);

        //se desejar fechar video/streaming, apertar esc
        if( (char)c == 27 ) //esc
        {
            break;
        }
    }

    return 0;
}

