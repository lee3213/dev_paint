#pragma once

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"

;
using namespace cv;
cv::Mat ColorReversal(cv::Mat &srcImg);

void PaintBackGround(cv::Mat &rstImg, int r, int g, int b);
void  PaintBackGround(unsigned char * rstData, int width, int height, int r, int g, int b);
cv::Rect rounding_box(cv::Mat a_mat, int bsize_x, int bsize_y, int V_th);
void overlap(Mat &a, Mat &b,int channel);
void overlap_white(Mat &a, Mat &b);
void overlap_blue(Mat &a, Mat &b);
void overlap_white(unsigned char * p_a, unsigned char * p_b, int rows, int cols);