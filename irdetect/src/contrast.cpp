/* Linear Contrast Enhancement */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>


void contrast(Mat& Image,int grayl,int* RepGray)
{

	uchar* p;
	int i,j;
	int nRows = Image.rows;
	int nCols = Image.cols;
	// Linear Stretching to enhance contrast in the image
	int MaxIntensity = *(RepGray+grayl-1);
	int MinIntensity = *(RepGray);
	cout << "Max Intensity : " << MaxIntensity << endl  ;
	cout << "Min Intensity : " << MinIntensity << endl;

	for(i=0;i<nRows;++i)
	{
		p = Image.ptr<uchar>(i);
		for(j=0;j<nCols;++j){

			if (p[j]!= 0){
				p[j] = ((p[j]-MinIntensity)*255)/(MaxIntensity - MinIntensity);
			}
		}
	}
}
