#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
#include <irdetect/irdetect.h>


void diffusion(Mat& src,Mat& Result)
{
  /// Gaussian Blurring of my Image
  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
  /// Generate grad_x and grad_y
  Mat grad_x, grad_y, grad;

  Sobel( src, grad_x, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_x, grad_x );
  Sobel( src, grad_y, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_y, grad_y );
  addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad );

  // Calculate Coefficients for filtering C = 1/(M+1) where M = grad;
  Mat M,C;
  add(grad,1.0,M);
  divide(1.0,M,C);

  // Multiply each pixel in I by its corresponding pixel in C
  Mat I;
  multiply(src,C,I);

  // Set Output = F(I)/F(C) where F(X) is equivalent to X when each pixel is
  // replaced by itself plus the sum of its neighbours.

  Mat final, kern = (Mat_<char>(3,3) <<  1, 1,  1,
                                  	  	 1, 1,  1,
                                  	  	 1, 1,  1);
  filter2D(I, I, I.depth(), kern );
  filter2D(C, C, C.depth(), kern );
  divide(I,C,final);
}

