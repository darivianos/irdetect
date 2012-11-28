#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>

float meanvalue(Mat& Result){
	// find mean value of the given image
	  int nRows = Result.rows;
	  int nCols = Result.cols;

	  int i,j;
	  float sum=0.0;

	  uchar* p;
	  for(i=0;i<nRows;++i)
	  {
		  p = Result.ptr<uchar>(i);
		  for(j=0;j<nCols;++j)
			  sum += p[j];
	  }

	  return (float)sum/(nRows*nCols);
}
