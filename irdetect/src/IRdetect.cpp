#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <irdetect/irdetect.h>
#include <irdetect/image_io.h>
#include <iomanip>
#include <ios>

using namespace std;
using namespace cv;


int main( int argc, char* argv[])
{

	std::vector<std::string> paths;
	paths.push_back(std::string(argv[1]));

	std::vector<std::string> images;
	getfilelists(paths, images);

	Mat I,K,OutputImage;

	for(size_t i = 0;i<images.size();++i){


		std::string filename = images.at(i);

		//Gray image
		I = imread( filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

		if(!I.rows){
			std::cout<<"read error for image "<<filename<<std::endl;
			continue;
		}

		I.convertTo(K,CV_8U);
		/// Create window and set position in the screen
		int ResizeFactor = 2;
		namedWindow( "Input",0);
		namedWindow( "Important Value", 0 );
		namedWindow( "OutputImage", 0);
		namedWindow( "Dilation", 0 );
		moveWindow("Input",0,0);
		moveWindow("Important Value",ResizeFactor*I.cols+200,0);
		moveWindow("OutputImage",0,ResizeFactor*I.rows +200);
		moveWindow("Dilation",ResizeFactor*I.cols+200,ResizeFactor*I.rows +200);
		resizeWindow("Input",ResizeFactor*I.cols,ResizeFactor*I.rows);
		resizeWindow("Important Value",ResizeFactor*I.cols,ResizeFactor*I.rows);
		resizeWindow("OutputImage",ResizeFactor*I.cols,ResizeFactor*I.rows);
		resizeWindow("Dilation",ResizeFactor*I.cols,ResizeFactor*I.rows);




		imshow("Input",K);
		//Step 1 : Diffusion to get rid of noise and edges.
		diffusion(K,K);

		// Step 2: Extract and show the regions of interest
		histogram(K,OutputImage);

		// Blobs of the image
		blob(OutputImage);
		imshow("OutputImage",OutputImage);

		cvWaitKey(0);
	}


	return 0;
}
