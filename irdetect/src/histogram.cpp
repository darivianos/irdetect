#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>


void histogram(Mat& Image,Mat& TempImage)
{
	int grayl = 8; //Lloyd Max gray level quantization (eight levels are enough to get a good approximation)
	int RepGray[grayl];
	int Thres[grayl+1];
	float Mean = meanvalue(Image);

	TempImage = Image.clone();
	int nRows = TempImage.rows;
	int nCols = TempImage.cols;

	// Replace Pixels with intensity lower than the mean
	// value with zero, since we make the assumption that
	// humans are warmer than the environment.
	int i,j;
	uchar* p;
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			if(p[j] < Mean){
				p[j] = 0;
			}

		}
	}

	// Contrast Enhancement and Dilation

	graylevels(TempImage,grayl,RepGray,Thres);
	contrast(TempImage,grayl,RepGray);

	Mat kern = (Mat_<char>(3,3) <<  1, 1,  1,
			1, 1,  1,
			1, 1,  1);

	// Apply the dilation operation
	erode( TempImage, TempImage, kern );
	dilate( TempImage, TempImage, kern );
	dilate( TempImage, TempImage, kern );
	imshow("Dilation",TempImage);

	// Apply the importance function to emphasize small regions of the image
	//int ImpParameter=1;
	//importantvalue(TempImage,ImpParameter);
	imshow("Important Value",TempImage);
}
