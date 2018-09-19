
#include "stdafx.h"
#include "debug_define.h"
#include "brush.h"
#include "debug_image.h"
#include "util.h"
#include <iostream>
#include "extern.h"
#include "rgb2hsv.h"
using namespace std;


int BrushInitialization(list<Brush> &_brush_set,int _brush_depth,int brush_size[])
{
	cv::Mat temp_brush;
	cv::Mat temp_index_brush;
	cv::Mat temp_thumbnail;
	cv::Mat temp_bump;

#ifdef DEBUG_BRUSH_IMAGE
	debug_image_single *debug_image_mask = nullptr;
	debug_image_single *debug_image_index = nullptr;
	debug_image_single *debug_image_thumbnail = nullptr;
	debug_image_single *debug_image_bump = nullptr;
	debug_image_single *debug_image_cropped = nullptr;
	
#endif
	int nth;
	static bool run_once = true;
	cv::Mat brush_cr, brush_th;
	string f_name;
	//Mat b160x160;
	//b160x160.create(160, 160, CV_8UC1);
	int bsize_w, bsize_h;
	Size sz;
	

	
	//	unsigned char * s;
	//unsigned char *d;

	
	nth = 0;
	Mat brush_gray;
	string fname;
	Brush __brush;
	int b_step1, b_channels;
	int s_step1, s_channels;


	for (; nth < g_BrushNumber; nth++)	{
		

		fname = cv::format("/render/brush/mask/%02d.jpg", nth + 1);
		temp_brush = imread(fname.c_str(), IMREAD_COLOR);

		unsigned char * temp_brush_data = temp_brush.data;
		bsize_w = temp_brush.size().width;
		bsize_h = temp_brush.size().height;
		s_step1=b_step1 = (int)temp_brush.step1();
		s_channels=b_channels = temp_brush.channels();
		if (temp_brush.size().width == 0 || temp_brush.size().height == 0)
		{
			cout << fname << endl;
			return -9998;
		}
		 cv::cvtColor(temp_brush, brush_gray, COLOR_RGB2GRAY);
		
		 unsigned char * bestBrush_data_gray_resized = brush_gray.data;
		//mat_print(temp_brush, "temp_brush");
	
	//	crop_s.x = b_rect.x;
		//crop_s.y = b_rect.y;
		//crop_e.x = crop_s.x + b_rect.width;
	//	crop_e.y = crop_s.y + b_rect.height;
#ifdef DO_CENTERED_BRUSH
		cv::Mat centered;
		centered.create(bsize_x, bsize_y, CV_8UC1);
		PaintBackGround(centered, 255, 255, 255);
		unsigned char * centered_data = centered.data;
#endif
	
		
	//	PaintBackGround(b160x160, 255, 255, 255);
		//unsigned char * b160x160_data = b160x160.data;
	
		sz=Size (g_INDEX_BRUSH_SIZE_WIDTH, g_INDEX_BRUSH_SIZE_HEIGHT);
		//	cout << s.height << "," << s.width << endl;
		cv::resize(temp_brush, temp_index_brush, sz);
	

		//fname = cv::format("/rst/br/i_%d.ppm", nth + 1);
	//	cv::imwrite(fname, temp_index_brush);

		fname = cv::format("/render/brush/mask-thumbnail/%02d.jpg", nth + 1);
		temp_thumbnail = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_thumbnail.size().width == 0 ||
			temp_thumbnail.size().height == 0) {
			cout << "thumbnail file error " << " : " << nth + 1 << endl;
			return -9998;
		}
		
		int t_size=g_brush_thumbnail_size= temp_thumbnail.size().width;
	

		fname = cv::format("/render/brush/bump/%02d.jpg", nth + 1);
		temp_bump = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_bump.size().width == 0 ||
			temp_bump.size().height == 0) {
			cout << "thumbnail file error " << " : " << nth + 1 << endl;
			return -9998;
		}
	
		__brush.brush_thumbnail=temp_thumbnail.clone();
		__brush.brush_gray = brush_gray.clone();
		__brush.brush = temp_brush.clone();
		__brush.brush_no = nth;
	
		__brush.bump=temp_bump.clone();
		__brush.index_brush=temp_index_brush.clone();
		_brush_set.push_back(__brush);
		if ( run_once){
			if (_brush_depth > 0)
			{
				Mat R_resized;
				Mat alpha_channel;
				Mat hsv_v;
				Mat R;
				int _brush_style = BRUSH_ALPHA;
				int s_width = bsize_w;
				int s_height = bsize_h;

				R.create(s_width, s_height, CV_8UC3);
				
				Mat bestBrush_resized;
				bestBrush_resized.create(s_width, s_height, CV_8UC3);
			
				unsigned char * bestBrush_data_resized = bestBrush_resized.data;
				unsigned char * changedData = R.data;
				unsigned char * bestBrush_embossed_resized_data = temp_bump.data;
				Point centered_SrtPoint = { 0,0 }, p;
			
				int color_BGR_B = 255;
				int color_BGR_G = 0;
				int color_BGR_R = 0;
				int paint_grid_w_size = bsize_w;
				int	paint_grid_h_size = bsize_h;

				Scalar s_rgb, s_hsv;

				int br_BGR_B = 255;
				int br_BGR_G = 0;
				int br_BGR_R = 0;
				unsigned char * alpha_channel_data;
				unsigned char * hsv_v_data;
				alpha_channel.create(paint_grid_w_size, paint_grid_h_size, CV_8UC1);
				hsv_v.create(paint_grid_w_size, paint_grid_h_size, CV_8UC1);
				debug_image("br2/ET_" , nth, temp_bump);
				for (color_BGR_B = 0; color_BGR_B < 255; color_BGR_B++) {
					R.setTo(255);
					alpha_channel.setTo(0);
					alpha_channel_data = alpha_channel.data;
				
					hsv_v.setTo(0);
					bestBrush_resized.setTo(255);
					hsv_v_data = hsv_v.data;

					int Alpha;
					int _TH = 200;
					for (int by = 0; by < paint_grid_h_size; by++)
					{
						for (int bx = 0; bx < paint_grid_w_size; bx++)
						{

							p.x = (centered_SrtPoint.x + bx);
							p.y = (centered_SrtPoint.y + by);
							bool c_flag = false;

							if (p.x < 0) c_flag = true;
							if (p.y < 0) c_flag = true;
							if (p.x > (s_width - 1)) c_flag = true;
							if (p.y > (s_height - 1))  c_flag = true;
							//브러시에 색상 입히기

							int bIndex_3c = by *b_step1 + bx * b_channels;
							int gray_bIndex_1c = by*paint_grid_w_size + bx;

							//for tranparent effect 

							//	if (bestBrush_data_gray[gray_bIndex] > g_brush_Ts) c_flag = true;

							if (bestBrush_data_gray_resized[gray_bIndex_1c] > g_brush_Ts) {
								c_flag = true;
								alpha_channel_data[gray_bIndex_1c] = 150;
								continue;
							}
							//	
							if (c_flag == true) {
								//	p_poke(bestBrush_data_re, bIndex, 255, 255, 255);
								alpha_channel_data[gray_bIndex_1c] = 100;
								continue;
							}
							int sIndex = p.y * s_step1 + p.x * s_channels;
							alpha_channel_data[gray_bIndex_1c] = 255;
							if (_brush_style == BRUSH_ALPHA) {
								s_rgb.val[_R] = color_BGR_R;//R
								s_rgb.val[_G] = color_BGR_G;//G
								s_rgb.val[_B] = color_BGR_B;//B
								RGB2HSV(s_rgb, s_hsv);

								hsv_v_data[gray_bIndex_1c] = (int)s_hsv.val[_V];

								//	Alpha = bestBrush_data_resized[bIndex];// T(x, y);
								//		Alpha = (int)s_hsv.val[2];
								Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
								if (Alpha < _TH) {

									s_hsv.val[2] = s_hsv.val[2] + g_ET*(bestBrush_embossed_resized_data[gray_bIndex_1c] - 128);
									HSV2RGB(s_hsv, s_rgb);
									bestBrush_data_resized[bIndex_3c + _BGR_R] = (int)s_rgb.val[_R];//R
									bestBrush_data_resized[bIndex_3c + _BGR_G] = (int)s_rgb.val[_G];//G
									bestBrush_data_resized[bIndex_3c + _BGR_B] = (int)s_rgb.val[_B];//B

								}
								else {
									alpha_channel_data[gray_bIndex_1c] = 100;
									/*
									bestBrush_data_resized[bIndex_3c + _BGR_B] = color_BGR_B;
									bestBrush_data_resized[bIndex_3c + _BGR_G] = color_BGR_G;
									bestBrush_data_resized[bIndex_3c + _BGR_R] = color_BGR_R;
									*/
								}
							}
							else if (_brush_style == BRUSH_COPY) {
								bestBrush_data_resized[bIndex_3c + _BGR_B] = color_BGR_B;
								bestBrush_data_resized[bIndex_3c + _BGR_G] = color_BGR_G;
								bestBrush_data_resized[bIndex_3c + _BGR_R] = color_BGR_R;
							}

							br_BGR_R = bestBrush_data_resized[bIndex_3c + _BGR_R];
							br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
							br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];
							//그리기
							if (alpha_channel_data[gray_bIndex_1c] == 255)
								p_poke(changedData, sIndex, br_BGR_B, br_BGR_G, br_BGR_R);

						}//for by
					}//for bx
					
					debug_image("br2/R_" + to_string(color_BGR_B), nth, R);
					debug_image("br2/bestBrush_" + to_string(color_BGR_B), nth, bestBrush_resized);
					
					/*
					for (int i = 0; i < _brush_depth; i++) {

						cv::resize(R, R_resized, Size(brush_size[i], brush_size[i]));
						debug_image("br2/R_resized_" + to_string(color_BGR_B) + to_string(brush_size[i]), nth, R_resized);
						R_resized.release();

					}
					*/
				}//for color_B
				debug_image("br2/alpha_channel_" + to_string(color_BGR_B), nth, alpha_channel);
				run_once = false;
			}//run once
			}//if > 0
		//mat_print(temp_thumbnail,"temp_thumbnail");
#ifdef DEBUG_BRUSH_IMAGE
		if (nth == 0)
		{
			std::string fpath;

			sz = temp_brush.size();
			debug_image_mask = new debug_image_single(sz, g_BrushNumber, temp_brush.type());

			sz = temp_index_brush.size();
			debug_image_index = new debug_image_single(sz,g_BrushNumber, temp_index_brush.type());

			sz = temp_thumbnail.size();
			debug_image_thumbnail = new debug_image_single(sz,g_BrushNumber, temp_thumbnail.type());

			sz = temp_bump.size();
			debug_image_bump = new debug_image_single(sz,g_BrushNumber, temp_bump.type());

			fpath = "image_mask_";
			debug_image_mask->set_file_format(fpath);

			fpath = "image_index_";
			debug_image_index->set_file_format(fpath);

			fpath = "image_thumbnail_";
			debug_image_thumbnail->set_file_format(fpath);

			fpath = "image_bump_";
			debug_image_bump->set_file_format(fpath);

			Mat *a;
			a = debug_image_index->get_mat();
			mat_print(a, "debug_image_index");
		}
		debug_image_mask->image_add(temp_brush);
		debug_image_index->image_add(temp_index_brush);
		debug_image_thumbnail->image_add(temp_thumbnail);
		debug_image_bump->image_add(temp_bump);
#endif
	//	delete brush_accumulation_map;
	}//for nth
	
	
#ifdef DEBUG_BRUSH_IMAGE
	if (debug_image_mask != nullptr)
		debug_image_mask->image_final();
	if (debug_image_index != nullptr)
		debug_image_index->image_final();
	if (debug_image_thumbnail != nullptr)
		debug_image_thumbnail->image_final();
	if (debug_image_bump != nullptr)
		debug_image_bump->image_final();
#endif
	/*
	for (nth=0; nth < g_BrushNumber; nth++) {
		debug_image("br/brush_gray_", nth, g_brush_set.at(nth).brush_gray);
		debug_image("br/brush_", nth, g_brush_set.at(nth).brush);
		debug_image("br/thubmnail_", nth, g_brush_set.at(nth).brush_thumbnail);
		debug_image("br/bump_", nth, g_brush_set.at(nth).bump);
	}
	*/
	return 0;
}