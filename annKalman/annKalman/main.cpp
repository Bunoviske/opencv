#include "processing.hpp"


int main(int argc, char** argv)
{
    VideoCapture cap;
    ImageProcessing * Image;
    if (argc > 2){
        Image = new ImageProcessing(argv[2]);
        cap.open(argv[1]);
    }
    else{
        cout << "Insira arquivo xml" << endl;
        return -1;
    }
    namedWindow("Imagem", WINDOW_AUTOSIZE);

//VER KALMAN

    while(cap.read(Image->img))
    {
        resize(Image->img, Image->img, Size(320,240)); //tamanho do video no NAO

        imshow("Imagem", Image->img);

        Image->copy = Image->img.clone();
        Image->predict(Image->copy);

        int c = waitKey(0);
        Image->candidatos.clear();

        //se desejar fechar video/streaming, apertar esc
        if( (char)c == 27 ) //esc
        {
            break;
        }
    }

    return 0;
}
