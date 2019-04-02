#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void CallBackFunc(int event, int x, int y, int flags, void * objeto);

class ImageProcessing{
public:

    /*
     * Variaveis
     */

    Mat red; // matriz auxiliar que mostrará os pixels vermelhos
    Mat img; //img original

    int gray; //flag

    //valores de referencia para aplicar a red em cada frame
    float upBound[3];
    float botBound[3];

    /*
     * Métodos
     */

    ImageProcessing(); //construtor
    void pixels2red();
    void pixelsIntensity(int x, int y);

private:

    /*
     * Métodos
     */

    void setBoundaries(Vec3b *pixel);
    int grayScale();

};
