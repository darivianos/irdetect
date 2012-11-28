#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>


void importantvalue(Mat& TempImage, int ImpParameter){
	// Calculate Histogram of the Image
	int nRows = TempImage.rows;
	int nCols = TempImage.cols;
	uchar* p;
	int i,j;
	float HistInt[256] = {0.0}; // Initialize Histogram
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){

			// Now calculate the histogram (don't care about zero intensity)
			if (p[j]!= 0){
				HistInt[p[j]] += 1;
			}
		}
	}

	// Calculation of the importance function [0.5^n 1],
	// few pixels -> high importance (object or human)
	// great number of pixels -> low importance (background)
	float ImportValue;
	float MaxHist = 0.0;	// Maximum Value in the Histogram
	for(i=0;i<256;i++)
		if(MaxHist < HistInt[i])
			MaxHist = HistInt[i];

	// Calculate the importance factor then multiply with intensity and
	// keep only those pixels with remaining intensity more than 60% of the Max.

	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			float TempValue = HistInt[p[j]];

			// Importance Value (emphasize to small regions degrading the impact
			// of the background. Parameter n can be adjusted.
			ImportValue = pow(( 1 /(TempValue/ MaxHist + 1 ) ),ImpParameter);
			p[j] = floor(ImportValue*p[j]);
			if(p[j]<50)
				p[j] = 0;
		}
	}
	imshow("Important Value",TempImage);
}
