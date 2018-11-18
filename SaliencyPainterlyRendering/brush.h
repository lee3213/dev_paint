#pragma once

#include "stdafx.h"

#include "opencv2\\opencv.hpp"
#include "opencv2\\core.hpp"
using namespace std;
using namespace cv;

class Brush {
public:
	cv::Mat brush_gray;
	cv::Mat brush;
	cv::Mat brush_hsv;
	cv::Mat index_brush;
	cv::Mat brush_thumbnail;
	cv::Mat bump;
	int brush_no;
	int m_depth;

};


#define BRUSH_ALPHA 1
#define BRUSH_COPY 2
int BrushInitialization(vector <Brush*> &brush);
//, int _called_once, int brush_size[]);