#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "extern.h"
using namespace cv;


#include <iostream>
using std::cout;
using std::endl;
inline cv::Rect safe_Rect(Point s, Point e, int i_width, int i_height);
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

inline void p_poke(unsigned char * p, int index, int r);

inline void p_peek(unsigned char * p, int index, int &p_0, int &p_1, int &p_2) {
	p_0 = p[index];
	p_1 = p[index + 1];
	p_2 = p[index + 2];
}
inline void p_peek_1c(unsigned char * p, int index, int &p_0) {
	p_0 = p[index];
}

//inline void rectangle_canvas(cv::Mat mat, cv::Rect  rect, Scalar s);
//
//bool depth_check(int depth);
/*
{
	static int called_cnt = 0;
	called_cnt++;
	if ((called_cnt % g_painting_try[depth])) return false;
	if (depth < 4) return true;
	else return false;
};
*/

struct MatchPathSeparator
{
	bool operator()(char ch) const
	{
		return ch == '/';
	}
};
std::string inline basename(std::string const& pathname)
{
	return std::string(std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator()).base(), pathname.end());
}

std::string inline removeExtension(std::string const& filename)
{
	std::string::const_reverse_iterator pivot = std::find(filename.rbegin(), filename.rend(), '.');
	return pivot == filename.rend() ? filename : std::string(filename.begin(), pivot.base() - 1);
}