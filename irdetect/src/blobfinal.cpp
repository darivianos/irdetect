/* Linear Contrast Enhancement */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <new>
#include <boost/shared_ptr.hpp>
using namespace std;
using namespace cv;


typedef std::vector<std::pair<int,int> > Pixels;

void blobfinal( Mat& Image){


	int nRows = Image.rows;
	int nCols = Image.cols;
	int i,j;


	// Initialize Output Image with zeros
	Mat OutputImage(nRows,nCols,CV_8U,Scalar(0));


	// Find the blobs of the specific intensity in the image
	std::map<int, Pixels> regions;

	//iterator for accessing image elements
	Mat_<uchar>::iterator p = Image.begin<uchar>();
	Mat_<uchar>::iterator q = OutputImage.begin<uchar>();

	for(i=0;i<nRows;i++)
	{
		for(j=0;j<nCols;j++){
			Mat_<uchar>::iterator ImMe = (p + i*nCols + j);
			Mat_<uchar>::iterator OutMe = (q + i*nCols + j);

			if ((i == 0) || (i == nRows-1) || (j == 0) || (j == nCols-1)){
				 *ImMe = 0;
			}

			if (*ImMe != 0){
				// check if it is has adjacent

				Mat_<uchar>::iterator ImAbove = (p + (i-1)*nCols + j);
				Mat_<uchar>::iterator ImLeft = (p + i*nCols + j - 1);
				Mat_<uchar>::iterator OutAbove = (q + (i-1)*nCols + j);
				Mat_<uchar>::iterator OutLeft = (q + i*nCols + j - 1);

				int label = -1;

				bool leftAndMeSame = (*ImLeft == *ImMe) ;
				bool AboveAndMeSame = (*ImAbove == *ImMe);

				if(AboveAndMeSame && leftAndMeSame){

					label = (int)*OutAbove;	//set the pixel to above region
					*OutMe = *OutAbove;
					regions[label].push_back(make_pair(i,j));


					if(*OutAbove != *OutLeft){
						Pixels& RegionOfLeft = regions[(int)*OutLeft];
						Pixels& RegionOfAbove = regions[(int)*OutAbove];

						for(Pixels::iterator it = RegionOfAbove.begin() ; it != RegionOfAbove.end() ; it++){ //put all pixels from left region to region of above pixel
							RegionOfLeft.push_back(*it); //add pair with coordinates of pixel to region above
						 //set label of the pixel to above region
							Mat_<uchar>::iterator temp = q + (*it).first * nCols + (*it).second;
							*temp = *OutLeft;
						}

						regions.erase((int)*OutAbove); //remove region of left pixel from list of regions
					}
				}else{
					if(AboveAndMeSame){ //pixel above belongs to same blob
						label = (int)*OutAbove;	//set the pixel to above region
						*OutMe = *OutAbove;
						regions[label].push_back(make_pair(i,j));
					}
					if (leftAndMeSame){ //pixel left belongs to same blob
						label = (int)*OutLeft;	//set the pixel to Left region
						*OutMe = *OutLeft;
						regions[label].push_back(make_pair(i,j));
					}

				}
				if(label == 0)
					cout << "Something is wrong, check code again" << endl;

				if(label==-1){

					int nextlabel = 1;
					while (regions.count(nextlabel) > 0)
						nextlabel++;

					*OutMe =  nextlabel; //set pixel to belong to new blob
					regions[nextlabel].push_back(make_pair(i,j));//add pixel to group
				}
			}
		}
	}

	uchar* l;
	for(i=0;i<nRows;i++){
		l = OutputImage.ptr<uchar>(i);
		for(j=0;j<nCols;j++){
			cout << (int) *(l+j);
		}
		cout << endl;
	}

	std::map<int, Pixels>::iterator iter;
	for(iter = regions.begin(); iter != regions.end(); iter++){

		Pixels& RegionPixels = (*iter).second; //second element of the map is the vector with the coordinates

		int maxrow = 0;
		int maxcol = 0;
		int minrow = 100000;
		int mincol = 100000;

		Pixels::iterator it;
		for(it = RegionPixels.begin(); it != RegionPixels.end() ; it++){
			int temprow = (*it).first;
			int tempcol = (*it).second;
			if(temprow > maxrow)
				maxrow = temprow;
			if(temprow < minrow)
				minrow = temprow;
			if(tempcol > maxcol)
				maxcol = tempcol;
			if(tempcol < mincol)
				mincol = tempcol;
		}
		int coldist = maxcol - mincol;
		int rowdist = maxrow - minrow;

		if(coldist<2 || coldist >8 || rowdist<2 || rowdist >8 || (int)RegionPixels.size() > 70 || (int)RegionPixels.size() < 5)
			for(it = RegionPixels.begin(); it != RegionPixels.end() ; it++){
				Mat_<uchar>::iterator temp = q + (*it).first * nCols + (*it).second;
				*temp = 0;
			}
		regions.erase(iter);
	}
}
