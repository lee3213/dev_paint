#pragma once
#pragma once

#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
using namespace std;
using namespace cv;

class Brush_pgm {
public:
	cv::Mat brush_8UC1;
	Size brush_8UC1_Size;
	//cv::Mat brush_mask;

	cv::Mat brush_thumbnail;
	//cv::Mat bump;
	int brush_no;
};


int brush_pgm();
//, int _called_once, int render_brush_size[]);