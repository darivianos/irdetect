/*
 * image_io.h
 *
 *  Created on: 4 Oct 2011
 *      Author: slynen
 */

#ifndef IMAGE_IO_H_
#define IMAGE_IO_H_
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <irdetect/sort_comp.h>
#include <boost/bind.hpp>

using namespace std;

int getfilelists(std::vector<string>& initialPaths, std::vector<string>& imagepaths, bool sortlexical = false){
	std::string initialPath;
	DIR           *d;
	struct dirent *dir;
	for(size_t diridx = 0;diridx<initialPaths.size();++diridx){
		initialPath = initialPaths.at(diridx);
		d = opendir( initialPath.c_str() );
		if( d == NULL ) {
			throw logic_error( initialPath+" results in d == NULL" );
			return 1;
		}
		int i = 0;
		while( ( dir = readdir( d ) ) ) {
			if( strcmp( dir->d_name, "." ) == 0 ||
					strcmp( dir->d_name, ".." ) == 0 ) {
				continue;
			}
			if(dir==NULL) break;

			i++;
			imagepaths.push_back(initialPath+dir->d_name);
		}
	}
	if (sortlexical){
		sort(imagepaths.begin(), imagepaths.end()); //normal lexical sort
	}else{
		sort(imagepaths.begin(), imagepaths.end(), boost::bind(&numeric_string_compare, _2,_1)); //sorts strictly by the number in the file name
	}
	return 0;
}


#endif /* IMAGE_IO_H_ */
