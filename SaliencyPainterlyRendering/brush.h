#pragma once

#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"

typedef struct BrushData {
	cv::Mat brush_gray;
	cv::Mat brush;
	cv::Mat index_brush;
	cv::Mat brush_thumbnail;
	cv::Mat bump;
	int brush_no;
}Brush;


#define BRUSH_ALPHA 1
#define BRUSH_COPY 2
int BrushInitialization(list<Brush> &brush,int _called_once,int brush_size[]);