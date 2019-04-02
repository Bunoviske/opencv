#include "edgeModule.h"
#include "canny.h"
#include "sobel.h"
#include "laplaciano.h"


int main(int argc, char** argv)
{

    if (argc > 2){
        int i = 0;
        char name[60];

        //analise para descobrir o nome da imagem
        while (i < strlen(argv[2])){
            if (argv[2][i] != 'g'){
                name[i] = argv[2][i];
                i++;
            }
            else{
                name[i] = '\0';
                i = strlen(argv[2]);
            }
        }

        cv::VideoCapture cap;
        cv::Mat img, gt;

        cap.open(argv[1]);
        cap.read(img);
        cap.release();
        cap.open(argv[2]);
        cap.read(gt);
        cap.release();

        cv::cvtColor(img, img, CV_BGR2GRAY);
        cv::cvtColor(gt, gt, CV_BGR2GRAY);

        int stop = 0;
        int in;

        while (stop == 0){

            cout << "Escolha uma das opcoes (digite o numero):" << endl <<
                    " 1. Canny \n 2. Sobel \n 3. Laplaciano \n 4. Terminar o programa" << endl;
            cin >> in;

            if(in == 4){
                stop = 1;
            }
            else if (in == 1){
                char auxname[60];
                strcpy(auxname, name);
                strcat(auxname, "canny.jpg");
                canny  * lineDetector = new canny(img, gt, auxname);
                lineDetector->run();
                delete lineDetector;

            }
            else if (in == 2){
                char auxname[60];
                strcpy(auxname, name);
                strcat(auxname, "sobel.jpg");
                sobel  * lineDetector = new sobel(img, gt, auxname);
                lineDetector->run();
                delete lineDetector;

            }
            else if (in == 3){
                char auxname[60];
                strcpy(auxname, name);
                strcat(auxname, "laplaciano.jpg");
                laplaciano  * lineDetector = new laplaciano(img, gt, auxname);
                lineDetector->run();
                delete lineDetector;
            }
            else{
                cout << "Digite 1, 2, 3 ou 4" << endl;
            }

        }

    }

    else
    {
        cout << "Erro de execucao do programa. Digite pelo terminal:" << endl;
        cout << "./main 46.gif 46gt.gif" << endl;
    }

    return 0;

}
