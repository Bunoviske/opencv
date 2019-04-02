#include "camera_calibration.h"
#include "string.h"



int main(int argc, char** argv)
{

    if (argc > 1){
        if (!strcmp(argv[1], "CALIB")){
            mainCalib(0);
        }
        else if(!strcmp(argv[1], "LOAD")){
            mainCalib(1);
        }
        else{
            cout << "Erro de execucao do programa. Digite um dos argumentos pelo terminal:" << endl;
            cout << "1- ./main CALIB" << endl;
            cout << "2- ./main LOAD" << endl;

        }
    }

    // Se nao tiver argumentos de entrada, é aberto streaming de video
    else
    {
        cout << "Erro de execucao do programa. Digite um dos argumentos pelo terminal:" << endl;
        cout << "1- ./main CALIB" << endl;
        cout << "2- ./main LOAD" << endl;
    }

    return 0;

}
