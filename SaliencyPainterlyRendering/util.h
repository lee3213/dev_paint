#pragma once
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "extern.h"
using namespace cv;


#include <iostream>
using std::cout;
using std::endl;
cv::Rect safe_Rect(Point s, Point e, int i_width, int i_height);
void mat_copyTo(unsigned char *s, unsigned char * d, Rect s_rect, Rect d_rect, int width);
// debug_image(Mat &a, char * p);
//void debug_image(Mat &a, char * p, int d);

std::string type2str(int type);
void mat_print(cv::Mat &amat, cv::String matname);
void mat_print(cv::Mat *amat, cv::String matname);
void mat_print(cv::Mat &amat, cv::String matname , int d);
void mat_print(cv::Mat *amat, cv::String matname, int d);
void Point_2p_debug(cv::Point p1, cv::String p1_tag, cv::Point p2, cv::String p2_tag);
void Point_1p_debug(cv::Point p1, cv::String p1_tag);
inline void p_poke(unsigned char * p, int index, int r, int g, int b);
inline void p_peek(unsigned char * p, int index, int &r, int &g, int &b);
inline void p_poke_canvas(unsigned char * p, int index, int ,int r, int g, int b);
inline void p_peek_canvas(unsigned char * p, int index, int y,int &r, int &g, int &b);
inline void p_poke(unsigned char * p, int index, int r);
inline void p_peek(unsigned char * p, int index, int &r);

inline void rectangle_canvas(cv::Mat mat, cv::Rect  rect, Scalar s);
//
//bool depth_check(int depth);
/*
{
	static int called_cnt = 0;
	called_cnt++;
	if ((called_cnt % g_painting_count[depth])) return false;
	if (depth < 4) return true;
	else return false;
};
*/
