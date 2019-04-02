#include "processing.h"

int main(int argc, char** argv)
{

    if (argc > 1){

        cv::Mat img = cv::imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

//        uchar labels[6] = {1, 2, 3, 4,4,1};
//        img = cv::Mat(1, 6, CV_8UC1, labels);

        if(! img.data )
        {
            cout <<  "Erro ao abrir imagem" << endl ;
            return -1;
        }

        texture.run(&img);

    }

    else
    {
        cout << "Erro de execucao do programa. Digite pelo terminal:" << endl;
        cout << "./main img.jpg" << endl;
    }

    return 0;

}
