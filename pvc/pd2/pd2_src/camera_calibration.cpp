#include "camera_calibration.h"



void mediaSTD(CvMat *media, CvMat *desvio, CvMat **matrix){

    double mean, STD;
    CvMat* matAux = cvCreateMat(1,5,CV_32FC1);

    for (int i = 0; i < matrix[0]->rows; i++){
        for (int j = 0; j < matrix[0]->cols; j++){
            for (int k = 0; k < 5; k++){
                CV_MAT_ELEM(*matAux, float,0,k) = CV_MAT_ELEM(*matrix[k], float,i,j);
            }
            cvMean_StdDev(matAux, &mean, &STD);
            CV_MAT_ELEM(*media, float,i,j) = (float)mean;
            CV_MAT_ELEM(*desvio, float,i,j) = (float)STD;

        }
    }
    Mat show(desvio);
    cout << show << endl;

}



void mainCalib(int load)
{


    CvCapture* capture;
    capture = cvCreateCameraCapture( 0 );
    if(!capture)
    {
        printf("\nCouldn't open the camera\n");
        return;
    }
    IplImage *image = cvQueryFrame( capture );
    cvNamedWindow( "Raw Video");


    if (load == 0){
        int n_boards = 0;		//Number of snapshots of the chessboard
        int frame_step;			//Frames to be skipped
        int board_w;			//Enclosed corners horizontally on the chessboard
        int board_h;			//Enclosed corners vertically on the chessboard

        printf("Enter the numbers of snapshots = ");
        scanf("%d",&n_boards);

        printf("\rEnter the numbers of frames to skip = ");
        scanf("%d",&frame_step);

        board_w  = 8;
        board_h  = 6;

        int board_total  = board_w * board_h;						//Total enclosed corners on the board
        CvSize board_sz = cvSize( board_w, board_h );

        cvNamedWindow( "Snapshot" );

        CvMat *intrinsicAux[5], *distortionAux[5];
        CvMat* mediaIntrinsic = cvCreateMat(3,3,CV_32FC1);
        CvMat* desvioIntrinsic = cvCreateMat(3,3,CV_32FC1);
        CvMat* mediaDistort = cvCreateMat(4,1,CV_32FC1);
        CvMat* desvioDistort = cvCreateMat(4,1,CV_32FC1);

        for(int ii = 0; ii < 5; ii++){

            cout << "Calibracao numero " << ii+1 << endl;

            //Allocate storage for the parameters according to total number of corners and number of snapshots
            CvMat* image_points      = cvCreateMat(n_boards*board_total,2,CV_32FC1); //armazena a posicao dos corners (x,y)
            CvMat* object_points     = cvCreateMat(n_boards*board_total,3,CV_32FC1);
            CvMat* point_counts      = cvCreateMat(n_boards,1,CV_32SC1);
            CvMat* intrinsic_matrix  = cvCreateMat(3,3,CV_32FC1);
            CvMat* distortion_coeffs = cvCreateMat(4,1,CV_32FC1);

            //Note:
            //Intrinsic Matrix - 3x3			   Lens Distorstion Matrix - 4x1
            //	[fx 0 cx]							[k1 k2 p1 p2   k3(optional)]
            //	[0 fy cy]
            //	[0  0  1]


            CvPoint2D32f* corners = new CvPoint2D32f[ board_total ];
            int corner_count;
            int successes = 0;
            int step, frame = 0;


            IplImage *gray_image = cvCreateImage(cvGetSize(image),8,1);			//subpixel


            //Loop while successful captures equals total snapshots
            //Successful captures implies when all the enclosed corners are detected from a snapshot

            while(successes < n_boards)
            {
                if((frame++ % frame_step) == 0)									//Skip frames
                {
                    //Find chessboard corners:
                    int found = cvFindChessboardCorners(image, board_sz, corners, &corner_count,CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS );

                    cvCvtColor(image, gray_image, CV_BGR2GRAY);										//Get Subpixel accuracy on those corners
                    cvFindCornerSubPix(gray_image, corners, corner_count, cvSize(11,11),cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

                    cvDrawChessboardCorners(image, board_sz, corners, corner_count, found);			//Draw it

                    // If we got a good board, add it to our data
                    if( corner_count == board_total )
                    {
                        cvShowImage( "Snapshot", image );		//show in color if we did collect the image
                        step = successes*board_total;
                        for( int i=step, j=0; j<board_total; ++i,++j ) {
                            CV_MAT_ELEM(*image_points, float,i,0) = corners[j].x;
                            CV_MAT_ELEM(*image_points, float,i,1) = corners[j].y;
                            CV_MAT_ELEM(*object_points,float,i,0) = (float) j/board_w;
                            CV_MAT_ELEM(*object_points,float,i,1) = (float) (j%board_w);
                            CV_MAT_ELEM(*object_points,float,i,2) = 0.0f;
                        }
                        CV_MAT_ELEM(*point_counts, int,successes,0) = board_total;
                        successes++;
                        printf("\r%d successful Snapshots out of %d collected.",successes,n_boards);
                    }
                    else
                        cvShowImage( "Snapshot", gray_image );										//Show Gray if we didn't collect the image
                }


                //Handle pause/unpause and ESC
                int c = cvWaitKey(15);
                if(c == 'p')
                {
                    c = 0;
                    while(c != 'p' && c != 27)
                    {
                        c = cvWaitKey(250);
                    }
                }
                if(c == 27)
                    return;

                image = cvQueryFrame( capture );								//Get next image
                cvShowImage("Raw Video", image);
            }

            //End WHILE loop with enough successful captures

            cvDestroyWindow("Snapshot");

            printf("\n\n *** Calibrating the camera now...\n");

            //Allocate matrices according to successful number of captures
            CvMat* object_points2  = cvCreateMat(successes*board_total,3,CV_32FC1);
            CvMat* image_points2   = cvCreateMat(successes*board_total,2,CV_32FC1);
            CvMat* point_counts2   = cvCreateMat(successes,1,CV_32SC1);

            //Tranfer the points to matrices
            for(int i = 0; i<successes*board_total; ++i)
            {
                CV_MAT_ELEM( *image_points2, float, i, 0)  =	CV_MAT_ELEM( *image_points, float, i, 0);
                CV_MAT_ELEM( *image_points2, float,i,1)    =	CV_MAT_ELEM( *image_points, float, i, 1);
                CV_MAT_ELEM(*object_points2, float, i, 0)  =  CV_MAT_ELEM( *object_points, float, i, 0) ;
                CV_MAT_ELEM( *object_points2, float, i, 1) =  CV_MAT_ELEM( *object_points, float, i, 1) ;
                CV_MAT_ELEM( *object_points2, float, i, 2) =  CV_MAT_ELEM( *object_points, float, i, 2) ;
            }

            for(int i=0; i<successes; ++i)
            {
                CV_MAT_ELEM( *point_counts2, int, i, 0) =  CV_MAT_ELEM( *point_counts, int, i, 0);			//These are all the same number
            }
            cvReleaseMat(&object_points);
            cvReleaseMat(&image_points);
            cvReleaseMat(&point_counts);

            // Initialize the intrinsic matrix with both the two focal lengths in a ratio of 1.0

            CV_MAT_ELEM( *intrinsic_matrix, float, 0, 0 ) = 1.0f;
            CV_MAT_ELEM( *intrinsic_matrix, float, 1, 1 ) = 1.0f;

            //Calibrate the camera
            //_____________________________________________________________________________________

            cvCalibrateCamera2(object_points2, image_points2, point_counts2,  cvGetSize( image ), intrinsic_matrix, distortion_coeffs, NULL, NULL, 0 );
            //CV_CALIB_FIX_ASPECT_RATIO

            //_____________________________________________________________________________________

            intrinsicAux[ii] = cvCloneMat(intrinsic_matrix);
            distortionAux[ii] = cvCloneMat(distortion_coeffs);

            char intrisicChar[50] = "Intrinsics";
            char distChar[50] = "Distortion";
            char aux[3];
            itoa(ii+1, aux, 10);

            strcat(intrisicChar,aux);
            strcat(intrisicChar,".xml");

            strcat(distChar,aux);
            strcat(distChar,".xml");

            cvSave(intrisicChar,intrinsic_matrix);
            cvSave(distChar,distortion_coeffs);


        }
        printf(" *** Calibration Done!\n\n");


        //Calculo da media e do desvio padrao para a matriz intrisica
        cout << "Matriz com o desvio padrao da matriz intrisica" << endl;
        mediaSTD(mediaIntrinsic, desvioIntrinsic, intrinsicAux);
        cvSave("IntrinsicsFinal.xml",mediaIntrinsic);


        //Calculo da media e do desvio padrao para a matriz de distorcao
        cout << "Matriz com o desvio padrao da matriz de distorcao" << endl;
        mediaSTD(mediaDistort, desvioDistort, distortionAux);
        cvSave("DistortionFinal.xml",mediaDistort);


        //Save values to file
        printf("Storing Intrinsics.xml and Distortions.xml files...\n");
        printf("Files saved.\n\n");

    }
    printf("Starting corrected display....\n");

    //Sample: load the matrices from the file
    CvMat *intrinsic = (CvMat*)cvLoad("IntrinsicsFinal.xml");
    CvMat *distortion = (CvMat*)cvLoad("DistortionFinal.xml");

    // Build the undistort map used for all subsequent frames.

    IplImage* mapx = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    IplImage* mapy = cvCreateImage( cvGetSize(image), IPL_DEPTH_32F, 1 );
    cvInitUndistortMap(intrinsic,distortion,mapx,mapy);

    // Run the camera to the screen, showing the raw and the undistorted image.


    //Como fx = fy (pixel quadrado), faz-se a media desses dois valores
    float f = (CV_MAT_ELEM( *intrinsic, float, 0, 0 ) + CV_MAT_ELEM( *intrinsic, float, 1, 1 ))/2;


    ImageProcessing * raw = new ImageProcessing(f,CV_MAT_ELEM( *intrinsic, float, 0, 2 ),
                                                  CV_MAT_ELEM( *intrinsic, float, 1, 2 ));
    ImageProcessing * undistort = new ImageProcessing(f,CV_MAT_ELEM( *intrinsic, float, 0, 2 ),
                                                        CV_MAT_ELEM( *intrinsic, float, 1, 2 ));



    float aux;

    // valor de Z para que seja possivel calcular o tamanho do objeto em cm (x = fx*(X/Z) + cx)

    cout << "Informe o valor de Z em cm: " << endl;
    cin >> aux;
    raw->z = aux;
    undistort->z = aux;

    cvNamedWindow( "Undistort" );

    setMouseCallback("Raw Video", rawCallBackFunc, raw);

    setMouseCallback("Undistort", undistortCallBackFunc, undistort);



    while(image)
    {


        IplImage *t = cvCloneImage(image);

        raw->drawGrid(image);
        cvShowImage( "Raw Video", image );			// Show raw image

        cvRemap( t, image, mapx, mapy );			// Undistort image
        cvReleaseImage(&t);

        undistort->drawGrid(image);
        cvShowImage("Undistort", image);			// Show corrected image

        //Handle pause/unpause and ESC
        int c = cvWaitKey(15);
        if(c == 'p')
        {
            cout << "Informe um novo valor de Z em cm: " << endl;
            cin >> aux;
            raw->z = aux;
            undistort->z = aux;

            cout << "Aperte 'p' com a tela de video selecionada" << endl;
            c = 0;
            while(c != 'p' && c != 27)
            {
                c = cvWaitKey(250);
            }
        }
        if(c == 27)
            break;

        image = cvQueryFrame( capture );
    }

}



