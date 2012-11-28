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

void blob( Mat& Image){


	int nRows = Image.rows;
	int nCols = Image.cols;
	int i,j;

	uchar* ImCur;
	uchar* ImPrev;
	uchar* OutCur;
	uchar* OutPrev;

	Mat OutputImage = Mat::zeros(Image.rows,Image.cols,Image.depth());

	// Find the blobs of the specific intensity in the image


	std::map<int, Pixels> regions;


	for(i=0;i<nRows;i++)
	{
		ImCur = Image.ptr<uchar>(i);
		OutCur = OutputImage.ptr<uchar>(i);

		for(j=0;j<nCols;j++){

			if ((i == 0) || (i == nRows-1) || (j == 0) || (j == nCols-1)){
				*(ImCur+j) = 0;
				*(OutCur+j) = 0;
			}

			if (*(ImCur+j) != 0){
				// check if it is has adjacent
				ImPrev = Image.ptr<uchar>(i-1);
				OutPrev = OutputImage.ptr<uchar>(i-1);

				int label = -1;

				bool leftAndMeSame = ((int)*(ImCur+j-1)==(int)*(ImCur+j));
				bool AboveAndMeSame = ((int)*(ImPrev+j)==(int)*(ImCur+j));

				if(AboveAndMeSame && leftAndMeSame){

					label = (int)*(OutPrev+j);	//set the pixel to above region
					*(OutCur+j) = *(OutPrev+j);
					regions[label].push_back(make_pair(i,j));

					int labelleft = (int)*(OutCur+j-1);
					int labelabove = (int)*(OutPrev+j);

					if (regions.count(labelleft) == 0){
							continue;
					}

					if(labelleft != labelabove){
						Pixels& RegionOfLeft = regions[labelleft];
						Pixels& RegionOfAbove = regions[labelabove];

						Pixels::iterator it;
						for(it = RegionOfLeft.begin() ; it != RegionOfLeft.end() ; it++){ //put all pixels from left region to region of Left pixel
							RegionOfAbove.push_back(*it); //add pair with coordinates of pixel to region Left
							uchar* temp = OutputImage.ptr<uchar>((int)(*it).first); //set label of the pixel to Left region
							temp += (*it).second;
							*temp = labelabove;
						}
						RegionOfLeft.erase(RegionOfLeft.begin(),RegionOfLeft.end());
						regions.erase(labelleft); //remove region of left pixel from list of regions
					}
				}else{
					if(AboveAndMeSame){ //pixel above belongs to same blob
						label = (int)*(OutPrev+j);
						*(OutCur+j) = *(OutPrev+j);
						regions[label].push_back(make_pair(i,j));
					}
					if (leftAndMeSame){ //pixel left belongs to same blob
						label = (int)*(OutCur+j-1);
						*(OutCur+j) = *(OutCur+j-1);
						regions[label].push_back(make_pair(i,j));
					}

				}


				if(label==-1){

					int nextlabel = 1;
					while (regions.count(nextlabel) > 0)
						nextlabel++;

					*(OutCur+j) =  (uchar)nextlabel; //set pixel to belong to new blob
					regions[nextlabel].push_back(make_pair(i,j));//add pixel to group
				}
			}
		}
	}

//	uchar* q;
//	for(i=0;i<nRows;i++){
//		q = OutputImage.ptr<uchar>(i);
//		for(j=0;j<nCols;j++){
//			cout << (int) *(q+j);
//		}
//		cout << endl;
//	}

	std::map<int, Pixels>::iterator iter;
	vector<int> ToBeErased;

	for(iter = regions.begin(); iter != regions.end(); iter++){
		//cout << "Region: " << (*iter).first << endl << "Size: " << (int)(*iter).second.size() << endl;

		Pixels& RegionPixels = (*iter).second; //second element of the map is the vector with the coordinates

		int maxrow = 0;
		int maxcol = 0;
		int minrow = 100000;
		int mincol = 100000;

		Pixels::iterator it;
		for(it = RegionPixels.begin(); it != RegionPixels.end() ; it++){
			int temprow = (int)(*it).first;
			int tempcol = (int)(*it).second;
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

		if( (int)RegionPixels.size() > 70 || (int)RegionPixels.size() < 5){
			for(it = RegionPixels.begin(); it != RegionPixels.end() ; it++){
				uchar* temp = Image.ptr<uchar>((int)(*it).first); //set intensity of all pixels of that region to zero.
				temp += (*it).second;
				*temp = 0;
			}
			RegionPixels.erase(RegionPixels.begin(),RegionPixels.end());
			ToBeErased.push_back((*iter).first);
		}
	}
	vector<int>::iterator lk;
	for(lk = ToBeErased.begin();lk!=ToBeErased.end();lk++){
		regions.erase(*lk);
	}
	for(iter = regions.begin(); iter != regions.end(); iter++)
			cout << "Region: " << (*iter).first << endl << "Size: " << (int)(*iter).second.size() << endl;
}
