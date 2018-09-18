#pragma once
#include "stdafx.h"


#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;
struct _RGB_set {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct _HSV_set {
	signed int h;
	unsigned char s;
	unsigned char v;
} ;

#define _R 0
#define _G 1
#define _B 2
#define _H 0
#define _S 1
#define _V 2
void RGB2HSV(struct _RGB_set RGB, struct _HSV_set *HSV);
void HSV2RGB(struct _HSV_set HSV, struct _RGB_set *RGB);
void RGB2HSV(cv::Scalar RGB, Scalar &HSV);
void HSV2RGB(Scalar HSV, Scalar &RGB);