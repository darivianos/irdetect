//IRdetect.cpp

using namespace cv;

namespace cv{
class Mat; //forward decl
}

#ifndef DIFFUSION_H
#define DIFFUSION_H
void diffusion(Mat& ,Mat&);
#endif

#ifndef HISTOGRAM_H
#define HISTOGRAM_H
void histogram(Mat& ,Mat&);
#endif

#ifndef MEANVALUE_H
#define MEANVALUE_H
float meanvalue(Mat&);
#endif

#ifndef CONTRAST_H
#define CONTRAST_H
void contrast(Mat&,int,int*);
#endif

#ifndef IMPORTANTVALUE_H
#define IMPORTANTVALUE_H
void importantvalue(Mat&, int);
#endif

#ifndef GRAYLEVELS_H
#define GRAYLEVELS_H
void graylevels(Mat&,int,int*,int*);
#endif

#ifndef BLOB_H
#define BLOB_H
void blob(Mat&);
#endif

#ifndef BLOBFINAL_H
#define BLOBFINAL_H
void blobfinal(Mat&);
#endif
