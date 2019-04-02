#include "processing.h"

int main(int argc, char** argv)
{

    if (argc > 2){
        int i = 0;
        char name[30];

        //o primeiro nome deve ser da imagem da esquerda e deve conter 'L' para indicar isso
        while (i < strlen(argv[1])){
            if (argv[1][i] != 'L'){
                name[i] = argv[1][i];
                i++;
            }
            else{
                name[i] = '\0';
                i = strlen(argv[1]);
            }
        }
        strcat(name, "Depthimage.jpg");

        cv::Mat esq = cv::imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
        cv::Mat dir = cv::imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);

        cv::resize(esq, esq, cv::Size(320,320));
        cv::resize(dir, dir, cv::Size(320,320));

        ImageProcessing  * stereo = new ImageProcessing(esq, dir, name);

        int stop = 0;
        int W;

        while (stop == 0){

            cout << "Digite o tamanho da janela W (valor impar) ou -1 para terminar: " << endl;
            cin >> W;

            if(W == -1){
                stop = 1;
            }
            else if (W % 2 != 0){
                stereo->run(W);
            }
            else{
                cout << "Digite um valor impar!" << endl;
            }

        }

    }

    else
    {
        cout << "Erro de execucao do programa. Digite pelo terminal:" << endl;
        cout << "./main imgL.jpg imgR.jpg" << endl;
    }

    return 0;

}
