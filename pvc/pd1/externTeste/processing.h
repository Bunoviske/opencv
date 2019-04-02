#ifndef PROCESSING
#define PROCESSING

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*************
 * Globais
 *************/

extern Mat mascara; //mascara global
extern Mat red; // matriz auxiliar que mostrará os pixels vermelhos


/*************
 * Metodos de processamento da imagem
 *************/

int grayScale(Mat *img);
void pixels2red(Vec3b *pixel, Mat *imagem, int gray);
void pixelsIntensity(Mat *img, int x, int y);



//mascara::Mat(1, 1, CV_8UC3); //mascara global
//red(1, 1, CV_8UC3); // matriz auxiliar que mostrará os pixels vermelhos


#endif
