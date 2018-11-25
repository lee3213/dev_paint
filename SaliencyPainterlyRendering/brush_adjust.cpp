
#include "stdafx.h"
#include "debug_define.h"
#include "brush.h"
#include "debug_image.h"
#include "util.h"
#include <iostream>
#include "extern.h"
#include "image_util.h"
#include "dir.h"
using namespace std;
using namespace cv;
#define  DO_CENTERED_BRUSH true
#ifdef DO_CENTERED_BRUSH
int Brush_adjust(vector <Brush*> &_brush_set)
{
	//cv::Mat mask_8UC1;
	// ;/ cv::Mat temp_index_brush;
	//cv::Mat temp_thumbnail;
	//cv::Mat temp_bump;

	string render_stencil_path("\\render\\stencil");
	check_and_create(render_stencil_path, true);

	Point crop_s, crop_e;

	Size sz;

	int nth;
	cv::Mat brush_accumulation_map(200,200,CV_8UC1);
	cv::Mat brush_centered_accumulation_map(200, 200, CV_8UC1);
	cv::Mat brush_cr, brush_th;
	string f_name;
	//Mat b160x160;
	//b160x160.create(160, 160, CV_8UC1);
	int bsize_width, bsize_height;



	//	brush_accumulation_map.create(m_render_brush_size_x, m_render_brush_size_y, CV_8UC1);
		//brush_centered_accumulation_map.create(m_render_brush_size_x, m_render_brush_size_y, CV_8UC1);
		//	unsigned char * s;
		//unsigned char *d;
	unsigned char * accumulation_data = brush_accumulation_map.data;
	unsigned char * accumulation_centered_data = brush_centered_accumulation_map.data;
	brush_accumulation_map.setTo(255);
	brush_centered_accumulation_map.setTo(255);
	_brush_set.reserve(g_BrushNumber);
	
	nth = 0;
	Brush * __brush;
	Mat centered_ppm;
	for (; nth < g_BrushNumber; nth++) {
		string fname;
	
		fname = cv::format("/render/brush/mask/%02d.jpg", nth + 1);
		Mat mask_8UC1 = imread(fname.c_str(), IMREAD_GRAYSCALE);

		unsigned char * mask_8UC1_data = mask_8UC1.data;
		bsize_width= mask_8UC1.size().width;
		bsize_height = mask_8UC1.size().height;
		if (bsize_width == 0 || bsize_height == 0)
		{
			cout << fname << endl;
			exit(-9998);
		}
		

		__brush = new Brush;
		//__brush->brush_thumbnail = temp_thumbnail.clone();
		__brush->brush_8UC1 = mask_8UC1.clone();
	//	__brush->brush_mask = temp_mask.clone();
		__brush->brush_no = nth;
		__brush->brush_8UC1_Size.width = bsize_width;
		__brush->brush_8UC1_Size.height = bsize_height;
	//	__brush->bump = temp_bump.clone();

		_brush_set.push_back(__brush);

		Rect b_rect;
		b_rect = rounding_box(mask_8UC1, bsize_width, bsize_height, g_alpha_TH);

		//	crop_s.x = b_rect.x;
			//crop_s.y = b_rect.y;
			//crop_e.x = crop_s.x + b_rect.width;
		//	crop_e.y = crop_s.y + b_rect.height;

		cv::Mat centered;
		centered.create(bsize_width, bsize_height, CV_8UC1);
		PaintBackGround(centered, 255, 255, 255);
		unsigned char * centered_data = centered.data;

		//	cv::Mat brush_roi = mask_8UC1(b_rect).clone();

	//	unsigned char  *brush_roi_data = brush_roi.data;



	//	cout << "Brush : " << setw(4)<<nth + 1 << " " << setw(4) << crop_s.x << ", " << setw(4) << crop_s.y << ", " << setw(4) << crop_e.x << ", " << setw(4) << crop_e.y << endl;

		brush_cr = mask_8UC1.clone();
		cv::rectangle(brush_cr, b_rect, RGB(0, 0, 0));
		cv::line(brush_cr, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_cr, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/br_src_", nth + 1, brush_cr);
		//f_name = format("/rst/br/br_%d_0crop.ppm", nth + 1);
		//cv::imwrite(f_name, brush_cr);// 1CH

		Rect b_rect2;
		b_rect2.width = b_rect.width;
		b_rect2.height = b_rect.height;
		b_rect2.x = (bsize_width - b_rect.width) / 2;
		b_rect2.y = (bsize_height - b_rect.height) / 2;


		//	PaintBackGround(b160x160, 255, 255, 255);
			//unsigned char * b160x160_data = b160x160.data;

		mat_copyTo(mask_8UC1_data, centered_data, b_rect, b_rect2, bsize_width);
		debug_image("br/br_centered", nth + 1, centered);
		Mat centered_ROI = mask_8UC1(b_rect);
		centered_ROI.copyTo(centered_ppm);
		debug_image("br/centered_ROI_", nth, centered_ROI);
		f_name = format("/render/stencil/stencil_%d.pgm", nth + 1);
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PXM_BINARY);
		compression_params.push_back(1);
		cv::imwrite(f_name, centered_ppm,compression_params);// 1CH

									  //Make accumulated brush map 
		unsigned char * s;
		s = mask_8UC1.data;
		int s_i;

		for (int x = 0; x < bsize_width; x++)
			for (int y = 0; y < bsize_height; y++)
			{
				s_i = x + y * bsize_width * 1;

				for (int n = 0; n < 1; n++) {
					if (s[s_i] != 255) {

						accumulation_data[s_i] = s[s_i];
						accumulation_centered_data[s_i] = s[s_i];
						/*	if (centered_data[s_i] != 255 && x>20  && y>20 && x<180 && y < 180 ) {
								b160 = (x - 20) + (y - 20) * 160;
								b160x160_data[b160] = centered_data[s_i];
								accumulation_centered_data[s_i] = centered_data[s_i];
							}
							*/
					}
					//s_i++;
				}

			}


		//	fname = cv::format("/rst/br/b_%02d.ppm", nth + 1);
		//	cv::imwrite(fname, b160x160);	

		cv::rectangle(centered, b_rect2, RGB(0, 0, 0));
		cv::line(centered, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(centered, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/centered_1", nth + 1, centered);
		//cv::imwrite(f_name, centered);// 1CH

		//debug_image("br/centered_2",nth+1,centered)
		//f_name = format("/rst/br/br_%d_2centered.ppm", nth + 1);
		//cv::imwrite(f_name, centered);// 1CH



		/*
				fname = cv::format("/stencil/bump/%02d.ppm", nth + 1);
				temp_bump = imread(fname.c_str(), IMREAD_GRAYSCALE);
				if (temp_bump.size().width == 0 ||
					temp_bump.size().height == 0) {
					cout << "bump file error " << " : " << nth + 1 << endl;
				}
				*/
		

		Rect r = Rect(Point(0, 0), Point(bsize_width - 1, bsize_height - 1));
		debug_image("br/acc_map_",nth,brush_accumulation_map);

		//	cv::imwrite("/rst/br/0_brush_accumulation_map.ppm", brush_accumulation_map);

		Rect acc_rr = rounding_box(brush_accumulation_map, bsize_width, bsize_height, 235);
		//	cout << "accumulation "<< rr.x << ", " << rr.y << ", " << rr.width << ", " << rr.height << " : "<<bsize_width-(rr.x+rr.width)
		//		<<", "<< bsize_height-(rr.y+rr.height)

		//		<<endl;

		crop_s.x = acc_rr.x;
		crop_s.y = acc_rr.y;
		crop_e.x = crop_s.x + acc_rr.width;
		crop_e.y = crop_s.y + acc_rr.height;

		cv::rectangle(brush_accumulation_map, r, RGB(0, 0, 0));
		r = Rect(crop_s, crop_e);
		cv::line(brush_accumulation_map, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_accumulation_map, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		cv::rectangle(brush_accumulation_map, acc_rr, RGB(0, 0, 0));
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_e.x, crop_s.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_s.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_e.x, crop_s.y), Point(crop_e.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_e.y), Point(crop_e.x, crop_e.y), 0);

		debug_image("br/0_accumulation_map_crop",nth, brush_accumulation_map);
		//cv::imwrite("/rst/br/0_accumulation_map_crop.ppm", brush_accumulation_map);

		debug_image("br/0_brush_centered_accumulation_map",nth, brush_centered_accumulation_map);


		Rect rr = rounding_box(brush_centered_accumulation_map, bsize_width, bsize_height, 235);

		//	 cout <<"centered "<< rr.x << ", " << rr.y << ", " << rr.width << ", " << rr.height <<" : "<<bsize_width-(rr.x+rr.width)
		////		<<", "<< bsize_height-(rr.y+rr.height)
		//		 << endl;
		cv::rectangle(brush_centered_accumulation_map, r, RGB(0, 0, 0));
		r = Rect(crop_s, crop_e);
		cv::rectangle(brush_centered_accumulation_map, rr, RGB(0, 0, 0));
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_e.x, crop_s.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_s.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_e.x, crop_s.y), Point(crop_e.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_e.y), Point(crop_e.x, crop_e.y), 0);
		cv::line(brush_centered_accumulation_map, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_centered_accumulation_map, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/0_brush_centered_accumulation_map_crop",nth, brush_centered_accumulation_map);



		
	}
	return 0;
}
#endif