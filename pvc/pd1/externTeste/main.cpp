#include "processing.h"


void CallBackFunc(int event, int x, int y, int flags, void* img)
{
    Mat imagem = *((Mat*)img); //derreferenciando e fazendo casting para (Mat*)


    if ( flags == EVENT_FLAG_LBUTTON )
    {
        cout << "Posicao do mouse (" << x << ", " << y << ")    ";

        mascara = Scalar(1,1,1); //reseta a mascara toda vez que um botao for apertado

        //requisito 1
        pixelsIntensity(&imagem, x, y);
        multiply(imagem, mascara, red);
        imshow("Imagem", red);
    }
}

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
    Mat img;
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

    while(cap.read(img))
    {
        //entra nessa condicao apenas um vez, para criar a mascara com tamanho baseado na imagem de entrada
        if (aux){
            aux = 0;
            //resize(mascara, mascara, Size(img.cols, img.rows));
            mascara = img.clone();
            mascara = Scalar(1,1,1);
            red = img.clone();
        }

        setMouseCallback("Imagem", CallBackFunc, &img);


        //aplicacao da mascara
        multiply(img, mascara, red);

        //show the image
        imshow("Imagem", red);

        int c = delay(argc, fps);

        //se desejar fechar video/streaming, apertar esc
        if( (char)c == 27 ) //esc
        {
            break;
        }
    }

    return 0;
}

