#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

struct sample{
	Mat patch; // image to be trained
	int label; // can be either 1 or -1
	double weight;
};

struct pixel{
	int row;
	int col;
};

//weak classifier constructor
int counter; //global variable that gives a unique id to every classifier
int rows = 40;	//rows of the image patch to be classified
int cols = 40;	//cols of the image patch to be classified

class classifier{
private:
	int id; //identifier of the classifier
	pixel FirstEdge;
	pixel SecondEdge;

public:
	classifier(){
		id = counter;
		// randomly choose the pixels to set the classifier
		srand(time(NULL)); //initialize random seed
		FirstEdge.col = rand() % cols;
		FirstEdge.row = rand() % rows;
		SecondEdge.col = rand() % cols;
		SecondEdge.row = rand() % rows;

	};

	int get_id(){
		return id;
	};

	pixel get_FirstEdge(){
		return FirstEdge;
	}

	pixel get_SecondEdge(){
		return SecondEdge;
	}
};

typedef vector<int> Vec;

int ClassifierObject(classifier CurrentClassifier,sample CurrentSample, int Thres){

	Mat Patch = CurrentSample.patch;
	pixel FirstEdge = CurrentClassifier.get_FirstEdge();
	pixel SecondEdge = CurrentClassifier.get_SecondEdge();

	int intensity1 = (int)Patch.at<uchar>(FirstEdge.row,FirstEdge.col);
	int intensity2 = (int)Patch.at<uchar>(SecondEdge.row,SecondEdge.col);
	int classif;

	if (abs(intensity1 - intensity2) > Thres)
		classif = 1;
	else
		classif = -1;

	if(classif == CurrentSample.label)
		return 0;	//correctly classified sample by the classifier
	else
		return 1;	//wrong classified sample by the classifier

}

int main(){

	vector<sample> data;
	vector<sample>::iterator it_data;
	int NumData = (int) data.size();

	//initialize weights of samples and global counter of classifiers
	counter = -1;
	for(it_data = data.begin(); it_data != data.end(); ++it_data){
		it_data->weight = 1.0 / ((double)NumData);
	}

	// create a pool of randomly initialized classifiers
	NumClassifiers = 300;
	map<int,Vec> pool; // how every classifier with identifier (int) performed
						// on the training data (store to vector vec)

	map<int,classifier> pool_classifiers;
	for(int i = 0; i < NumClassifiers; ++i){
		int Thres = 10; //Threshold for comparison between pixels
		counter++;
		classifier CurrentClassifier; //construct a new temp classifier with id = counter
		Vec temp_vec; //vector to store how data are classified according to classifier temp

		//go through all the data and see how the classifier performs on them
		for(it_data = data.begin(); it_data != data.end(); ++it_data){
			//function that returns 0 if the sample was correclty classified
			//otherwise returns 1 (easy to check the error)
			sample CurrentSample = *it_data;

			int result = ClassifierObject(CurrentClassifier,CurrentSample,Thres);
			temp_vec.push_back(result);
		}
		//passing to the classifier the vector of the data
		//this vector has 1 to misclassified data by the specific classifier
		pool[i] = temp_vec;
		pool_classifiers[i] = CurrentClassifier;

	}

	//train the data

	//final vector that contains the classifiers where float will be the
	//weight of the chosen classifier

	vector<pair<classifier,float> > FinalClassifiers;
	int B = 20; // Number of weak learners to be trained in a sequence
	for(int i = 0; i < B ; ++i){
		//fit classifier by minimizing weighted error..
		float minerror = 0.0;
		float minregul = 0.0;//regularization term of the error
		int id_minclas = 0;
		int c = 0;
		Vec::iterator it_vector;
		for(it_vector = pool[0].begin(); it_vector != pool[0].end(); ++it_vector){
			sample tempdata = *(data.begin() + c); //get the current sample
			minerror += tempdata.weight * (*it_vector);
			minregul += tempdata.weight;
			c++;
		}
		minerror = minerror/minregul;

		//go through the rest classifiers and find the one with the minimum error
		map<int,Vec>::iterator iter_map;
		for(iter_map = pool.begin() + 1; iter_map != pool.end(); ++iter_map){
			Vec temp_vec = (*iter_map).second;
			c = 0;
			float error = 0;
			float regular = 0; //regularization term of the error
			for(it_vector = temp_vec.begin(); it_vector != temp_vec.end(); ++it_vector){
				sample tempdata = *(data.begin() + c); //get the current sample
				error += tempdata.weight * (*it_vector);
				regular += tempdata.weight;
				c++;
			}
			error = error / regular;

			if(error < minerror){
				minerror = error;
				id_minclas = (*iter_map).first;
			}

		}

		//According to the error of the classifier assign new weights to
		//the data, and get the classifiers weight.
		float ClassifierWeight = log((1-minerror)/minerror);
		FinalClassifiers.push_back(make_pair(pool_classifiers[id_minclas],ClassifierWeight));

		c = 0;
		for(it_data = data.begin(); it_data != data.end(); ++it_data){
			int Ident = *(pool[id_minclas].begin() + c); //go through the vector
			it_data->weight = (it_data->weight) * exp(ClassifierWeight * Ident);
		}

		//erase the best classifier from the pool and the corresponding vector of it
		//so as to make sure that it will not be used again... Diversity
		pool_classifiers.erase(id_minclas);
		pool.erase(id_minclas);
	}

	// the training phase would return
	return 1;
}
