#pragma once
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"

#include "RenderingImage.h"
#define BEFORE_BETTER 0
#define CHANGED_BETTER 1
typedef struct ColorDistanceData
{
	double distance;
	int nth;
}DisData;

using namespace std;
using namespace cv;

//int JudgementImage(unsigned char *srcData, unsigned char * changedData, unsigned char * beforeData,
	//int bsize_x,int bsize_y, 
//	Point bSrtPoint,  int depth, int i_w, int i_h, int i_c);

//int JudgementImage(cv::Mat rstImg, cv::Mat tempImg, int bsize, unsigned char* srcData, unsigned char* rstData, unsigned char* tempData, Point bSrtPoint,string,int depth);
int JudgementImage(unsigned char * srcData, unsigned char * changedData_p, unsigned char * beforeData_p,
	int paint_grid_w_size, int paint_grid_h_size, Point centered_SrtPoint,
	int astroke_depth, int i_w, int i_h, int i_c,Point _fetch_Point,string tag);
int JudgementBrush(cv::Mat &srcImg, int depth, int width, int height,list<Brush>_brush_set);
//, /* list<Brush> &brush, int nBrushNumber,*/int st_no,
	//int paint_grid_w_size, int paint_grid_h_size,Point C_S,Point C_E,int first_try);