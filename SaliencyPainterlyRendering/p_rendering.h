#pragma once

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
//#include "RenderingImage.h"
#include"image_util.h"
#include "debug_image.h"
#include "Judgement.h"
int   P_Rendering(//cv::Mat srcImg,
	unsigned char * srcData,
	unsigned char * changedData,
	Mat &beforeImg,
	cv::Mat &testImg_resized,
	list<Brush> &_brush_set,
	Point fetch_color_Point, Point centered_SrtPoint, int paint_grid_w_size, int paint_grid_h_size,
	String tag,
	int astroke_number, int ing, int astroke_depth, int painting_count, 
	int color_BGR_B, int color_BGR_G, int color_BGR_R,
	unsigned char * ing_canvas_data
	//string f_path_a,
	);
