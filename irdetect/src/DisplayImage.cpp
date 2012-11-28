#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	if( argc != 2 )
	{
		cout << "Usage: display_image ImageToLoadAndDisplay"<< endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

	if (! image.data)
	{
		cout << "Could not open and find the image" << std::endl;
		return -1;
	}

	namedWindow("Display Window",CV_WINDOW_AUTOSIZE);
	imshow("Display Window", image);

	// Lloyd Max quantization

	Mat TempImage = image.clone();

	// Step 1: Calculate and normalize Histogram
	// Calculate Histogram of the Image
	int nRows = TempImage.rows;
	int nCols = TempImage.cols;

	uchar* p;
	int i,j,k;
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

	//step 2: Choose initial representatives
	int NumGrayLevels = 8;
	int* RepGray = new int[NumGrayLevels];

	//Representatives and thresholds conditions
	for(i=0;i<NumGrayLevels;i++)
		*(RepGray + i) = (i+1)*255/(NumGrayLevels+1);

	int* Thres = new int[NumGrayLevels + 1];
	*(Thres) = 0;					//initialize first and last threshold
	*(Thres + NumGrayLevels) = 255;

	int flag;
	do{
		cout << "Representatives" << endl;
		for(i=0;i<NumGrayLevels;i++)
			cout << "\t" << *(RepGray + i);
		cout << endl;

		for(i=1;i<NumGrayLevels;i++)
			*(Thres + i) = *(RepGray + (i-1)) + floor((*(RepGray + (i-1) + 1) - *(RepGray + (i-1)))/2.0);

		cout << "Thresholds" << endl;
		for(i=0;i<NumGrayLevels+1;i++)
			cout << "\t"<< *(Thres + i);
		cout << endl;

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
	namedWindow("Output",CV_WINDOW_AUTOSIZE);
	imshow("Output", TempImage);


	waitKey(0);
	return 0;

}
