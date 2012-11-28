/*
 * testfalsecolor.cpp
 *
 *  Created on: Oct 22, 2012
 *      Author: slynen
 */
#include <opencv2/opencv.hpp>
#include <irdetect/falsecolor.h>
#include <assert.h>

int main(int argv, char** argc){

	cv::Mat im = cv::imread("src/test/frame04091.png",0);

	cv::Mat im_fc; //the false color image

	convertFalseColor(im, im_fc, palette::False_color_palette2);

	cv::imshow("false color", im_fc);

	cv::waitKey();

}
