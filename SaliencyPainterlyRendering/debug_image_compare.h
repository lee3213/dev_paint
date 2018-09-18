#pragma once
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;
class debug_image_single {
public:
	cv::Mat Mat_aRow_d;
	//std::string fname_format;
	string fname;
	Point c_image_size;
	Point a_image_size;
	int c_howmany;
	int c_called_cnt;
	int r = 255, g = 255, b = 255;
	debug_image_single(Size im_size, int howmany, int mat_type);
	debug_image_single::~debug_image_single()
	{
		Mat_aRow_d.release();
	}
	void set_file_format(std::string p);
	void image_add(cv::Mat & leftimage);
	void debug_image_single::image_add(cv::Mat & leftimage, int bsize);
	Mat * get_mat();
	void write_image_file(int calledcnt);
	void image_reset(int r);

	void image_final();
	void image_final_split(int a_size);
	void draw_rectangle(Point s, Point e);
	void next_roi();
};