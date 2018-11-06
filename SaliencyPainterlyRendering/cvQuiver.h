#pragma once
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;
void cvQuiver(Mat &Image, int x, int y, int u, int v, CvScalar Color,
	int Size, int Thickness);