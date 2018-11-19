#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "util.h"
using namespace cv;
#define FILE_JPEG 0
#define FILE_PPM 1
void debug_image_abs(string path, string fnameMat, Mat &a, int ftype=0);
void debug_image_abs(string str, Mat &a, int ftype=0 );
void debug_image_abs(char *str, Mat &a, int ftype=0 );
void debug_image_method(string str, Mat &a, int ftype=0 );
void debug_image_method(char *str, Mat &a, int ftype=0 );
void debug_image(string str, Mat &a, int ftype=0 );
void debug_image(char *p, Mat &a, int ftype=0 );
void debug_image(char * p, int d, vector<Mat> a, int ftype =0);
void debug_image(char * p, int d, Mat &a, int ftype =0);
void debug_image(string p, int d, Mat &a, int ftype =0);
