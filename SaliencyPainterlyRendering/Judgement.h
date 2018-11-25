#pragma once
#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "brush.h"


typedef struct ColorDistanceData
{
	double distance;
	int nth;
}DisData;

using namespace std;
using namespace cv;

//int JudgementImage(unsigned char *srcData, unsigned char * changedData, unsigned char * beforeData,
	//int bsize_width,int bsize_height, 
//	Point bSrtPoint,  int depth, int i_w, int i_h, int i_c);

//int JudgementImage(cv::Mat rstImg, cv::Mat tempImg, int bsize, unsigned char* srcData, unsigned char* rstData, unsigned char* tempData, Point bSrtPoint,string,int depth);
int JudgementImage(unsigned char * srcData, unsigned char * changedData_p, unsigned char * beforeData_p,
	int paint_area_w_size, int paint_area_h_size,
	Point centered_SrtPoint,// Point _fetch_Point,
	int astroke_depth,
	int b_w, int b_h, int b_c, int b_step1,
	int c_w,int c_h,int c_c, int canvas_ROI_step1,
	string tag,
	int image_channel1);

int JudgementBrush(cv::Mat &srcImg, int depth, int width, int height,vector <Brush*>_brush_set,int src_step1,string tag);
//, /* vector <Brush*> &brush, int nBrushNumber,*/int Strk_no,
	//int paint_area_w_size, int paint_area_h_size,Point C_S,Point C_E,int fitry);
