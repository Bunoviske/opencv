#ifndef PROCESSING
#define PROCESSING

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*************
 * DEFINES
 *************/

#define ORIGINAL 0
#define MASK 1
#define RED 2


/*************
 * Metodos de processamento da imagem
 *************/

void CallBackFunc(int event, int x, int y, int flags, void* img);
int grayScale(Mat *img);
void pixels2red(Vec3b *pixel, Mat *imagem, int gray);
void pixelsIntensity(Mat *img, int x, int y);


#endif
