
#include "stdafx.h"
#include "debug_define.h"
#include "brush.h"
#include "debug_image.h"
#include "util.h"
#include <iostream>
#include "extern.h"
#include "rgb2hsv.h"
using namespace std;


int BrushInitialization(vector <Brush*> &_brush_set)
{
	cv::Mat temp_mask;
	//cv::Mat temp_index_brush;
	cv::Mat temp_thumbnail;
	cv::Mat temp_bump;


	int nth;
	static bool run_once = false;
	cv::Mat brush_outlined, brush_th;
	string f_name;
	
	int bsize_w, bsize_h;
	Size sz;
	
	nth = 0;
	Mat mask_8UC1;

	string fname;
	Brush *__brush;
	int b_step1, b_channels;
	int s_step1, s_channels;

	_brush_set.reserve(g_BrushNumber);
	for (; nth < g_BrushNumber; nth++)	{
		string f_path = "/render/brush/mask/";

		string fn = cv::format("%02d.jpg",nth + 1);
		fname = f_path + fn;
		//	fname = cv::format("/render/brush/mask/%02d.jpg", nth + 1);
		temp_mask = imread(fname.c_str(), IMREAD_COLOR);

		
		if (temp_mask.size().width == 0 || temp_mask.size().height == 0)
		{
			cout << fname << endl;
			continue;
		}
		
		unsigned char * mask_8UC1_mask_data = temp_mask.data;
		bsize_w = temp_mask.size().width;
		bsize_h = temp_mask.size().height;
		s_step1 = b_step1 = (int)temp_mask.step1();
		s_channels = b_channels = temp_mask.channels();
	
		 cv::cvtColor(temp_mask, mask_8UC1, COLOR_RGB2GRAY);
		 unsigned char * bestBrush_data_8UC1_resized = mask_8UC1.data;
	
	
	//	PaintBackGround(b160x160, 255, 255, 255);
		//unsigned char * b160x160_data = b160x160.data;
	
		//sz=Size (g_INDEX_BRUSH_SIZE_WIDTH, g_INDEX_BRUSH_SIZE_HEIGHT);
		//	cout << s.height << "," << s.width << endl;
		//cv::resize(mask_8UC1, temp_index_brush, sz);
	

		//fname = cv::format("/rst/br/i_%d.ppm", nth + 1);
	//	cv::imwrite(fname, temp_index_brush);

		fname = cv::format("/render/brush/mask-thumbnail/%02d.jpg", nth + 1);
		temp_thumbnail = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_thumbnail.size().width == 0 ||
			temp_thumbnail.size().height == 0) {
			cout << "thumbnail file error " << " : " << nth + 1 << endl;
			continue;
		}
		
	//	int t_size=g_brush_thumbnail_size= temp_thumbnail.size().width;
	

		fname = cv::format("/render/brush/bump/%02d.jpg", nth + 1);
		temp_bump = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_bump.size().width == 0 ||
			temp_bump.size().height == 0) {
			cout << "thumbnail file error " << " : " << nth + 1 << endl;
			return -9998;
		}
	
		__brush = new Brush;
		__brush->brush_thumbnail = temp_thumbnail.clone();
		__brush->brush_8UC1 = mask_8UC1.clone();
		__brush->brush_mask = temp_mask.clone();
		__brush->brush_no = nth;

		__brush->bump = temp_bump.clone();
		
		_brush_set.push_back(__brush);
#ifdef BRUSH_BR2
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
				int paint_area_w_size = bsize_w;
				int	paint_area_h_size = bsize_h;

				Scalar s_rgb, s_hsv;

				int br_BGR_B = 255;
				int br_BGR_G = 0;
				int br_BGR_R = 0;
				unsigned char * alpha_channel_data;
				unsigned char * hsv_v_data;
				alpha_channel.create(paint_area_w_size, paint_area_h_size, CV_8UC1);
				hsv_v.create(paint_area_w_size, paint_area_h_size, CV_8UC1);
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
					for (int by = 0; by < paint_area_h_size; by++)
					{
						for (int bx = 0; bx < paint_area_w_size; bx++)
						{

							p.x = (centered_SrtPoint.x + bx);
							p.y = (centered_SrtPoint.y + by);
							bool c_flag = false;

							if (p.x < 0) c_flag = true;
							if (p.y < 0) c_flag = true;
							if (p.x > (s_width - 1)) c_flag = true;
							if (p.y > (s_height - 1))  c_flag = true;
							//�귯�ÿ� ���� ������

							int bIndex_3c = by *b_step1 + bx * b_channels;
							int gray_bIndex_1c = by*paint_area_w_size + bx;

							//for tranparent effect 

							//	if (bestBrush_data_8UC1[gray_bIndex] > g_brush_Ts) c_flag = true;

							if (bestBrush_data_8UC1_resized[gray_bIndex_1c] > g_brush_Ts) {
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
								Alpha = bestBrush_data_8UC1_resized[gray_bIndex_1c];
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
							//�׸���
							if (alpha_channel_data[gray_bIndex_1c] == 255)
								p_poke(changedData, sIndex, br_BGR_B, br_BGR_G, br_BGR_R);

						}//for by
					}//for bx
					
					debug_image("br2/R_" + to_string(color_BGR_B), nth, R);
					debug_image("br2/bestBrush_" + to_string(color_BGR_B), nth, bestBrush_resized);
					
					/*
					for (int i = 0; i < _brush_depth; i++) {

						cv::resize(R, R_resized, Size(render_brush_size[i], render_brush_size[i]));
						debug_image("br2/R_resized_" + to_string(color_BGR_B) + to_string(render_brush_size[i]), nth, R_resized);
						R_resized.release();

					}
					*/
				}//for color_B
				debug_image("br2/alpha_channel_" + to_string(color_BGR_B), nth, alpha_channel);
				}//if > 0
			}//run once
		
		//mat_print(temp_thumbnail,"temp_thumbnail");
#endif
#ifdef DEBUG_BRUSH_IMAGE
		if (nth == 0)
		{
			std::string fpath;

			sz = mask_8UC1.size();
			debug_image_mask = new debug_image_single(sz, g_BrushNumber, mask_8UC1.type());

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
		debug_image_mask->image_add(mask_8UC1);
		debug_image_index->image_add(temp_index_brush);
		debug_image_thumbnail->image_add(temp_thumbnail);
		debug_image_bump->image_add(temp_bump);
#endif
	//	delete brush_accumulation_map;
	}//for nth
	
	run_once = false;
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
		debug_image("br/brush_8UC1_", nth, g_brush_set.at(nth).brush_8UC1);
		debug_image("br/brush_", nth, g_brush_set.at(nth).brush);
		debug_image("br/thubmnail_", nth, g_brush_set.at(nth).brush_thumbnail);
		debug_image("br/bump_", nth, g_brush_set.at(nth).bump);
	}
	*/
	return 0;
}