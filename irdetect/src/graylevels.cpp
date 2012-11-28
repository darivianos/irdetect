/* Linear Contrast Enhancement */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>


void graylevels(Mat& Image,int NumGrayLevels,int* RepGray,int* Thres)
{
	// Lloyd Max quantization

	Mat TempImage = Image;

	// Step 1: Calculate and normalize Histogram
	// Calculate Histogram of the Image
	int nRows = TempImage.rows;
	int nCols = TempImage.cols;

	uchar* p;
	int i,j,k;
	int MaxIntensity = 0;
	int MinIntensity = 255;
	float HistInt[256] = {0.0}; // Initialize Histogram
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			// Maximum Intensity in the image
			if (p[j]>MaxIntensity){
				MaxIntensity = p[j];
			}
			// Minimum Intensity in the image
			if ((p[j]!=0)&&(p[j]<MinIntensity))
				MinIntensity = p[j];
			// Now calculate the histogram (don't care about zero intensity)
			if (p[j]!= 0){
				HistInt[p[j]] += 1;
			}
		}
	}

	//Representatives and thresholds conditions
	for(i=0;i<NumGrayLevels;i++)
		*(RepGray + i) = MinIntensity + (i+1)*(MaxIntensity-MinIntensity)/(NumGrayLevels+1);

	*(Thres) = MinIntensity;					//initialize first and last threshold
	*(Thres + NumGrayLevels) = MaxIntensity;

	int flag;
	do{

		for(i=1;i<NumGrayLevels;i++)
			*(Thres + i) = *(RepGray + (i-1)) + floor((*(RepGray + (i-1) + 1) - *(RepGray + (i-1)))/2.0);


		// Calculate New Representatives using MSE minimization
		flag = 0;
		for(i=0;i<NumGrayLevels;i++){
			float tempNum = 0;
			float tempDen = 0;
			int temp;
			for(j = *(Thres+i)+1; j<= *(Thres+i+1) ; j++){
				tempNum += j * (HistInt[j]);
				tempDen += HistInt[j];
			};
			if(tempDen != 0){
				temp = round(tempNum/tempDen);
				if(*(RepGray + i) - temp != 0){
					flag = 1;
					*(RepGray + i) = temp;
				}
			}
		}

	}while(flag == 1);

	cout << "Representatives:" << endl;
	for(i=0;i<NumGrayLevels;i++)
		cout << "\t" << *(RepGray + i);
	cout << endl;

	cout << "Thresholds:" << endl;
	for(i=0;i<NumGrayLevels+1;i++)
		cout << "\t"<< *(Thres + i);
	cout << endl;


	// renew image
	for(i=0;i<nRows;++i)
	{
		p = TempImage.ptr<uchar>(i);
		for(j=0;j<nCols;++j){
			for(k=0;k<NumGrayLevels;k++)
				if(p[j]<=*(Thres +k +1)){
					p[j] = *(RepGray+k);
					break;
				}
		}
	}
}
