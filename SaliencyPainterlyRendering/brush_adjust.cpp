
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
	Size effective_centered_sz(0,0);
	int nth;
	cv::Mat brush_accumulation_map(200,200,CV_8UC1);
	cv::Mat brush_centered_accumulation_map(200, 200, CV_8UC1);
	cv::Mat brush_outlined, brush_th;
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
	cv::Mat *centered[MAX_BRUSH];
	unsigned char * centered_data[MAX_BRUSH];
	cv::Mat *centered_outlined[MAX_BRUSH];
	unsigned char * centered_outlined_data[MAX_BRUSH];

	nth = 0;
	Brush * __brush;
	Mat centered_ppm;
	Rect b_effective_centered_rect;
	Point centered_s(200, 200);
	Point centered_e(0, 0);
	for (; nth < g_BrushNumber; nth++) {
		string fname;

		fname = cv::format("/render/brush/mask/%02d.jpg", nth + 1);
		Mat mask_8UC1 = imread(fname.c_str(), IMREAD_GRAYSCALE);

		unsigned char * mask_8UC1_data = mask_8UC1.data;
		bsize_width = mask_8UC1.size().width;
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

		Rect b_effective_rect;
		b_effective_rect = rounding_box(mask_8UC1, bsize_width, bsize_height, g_alpha_TH);

		//	crop_s.x = b_effective_rect.x;
			//crop_s.y = b_effective_rect.y;
			//crop_e.x = crop_s.x + b_effective_rect.width;
		//	crop_e.y = crop_s.y + b_effective_rect.height;


		centered[nth] = new Mat(bsize_width, bsize_height, CV_8UC1);
		centered[nth]->setTo(255);
		centered_data[nth] = centered[nth]->data;
		centered_outlined[nth] = new Mat(bsize_width, bsize_height, CV_8UC1);
		centered_outlined[nth]->setTo(255);
		centered_outlined_data[nth] = centered_outlined[nth]->data;
		//	cv::Mat brush_roi = mask_8UC1(b_effective_rect).clone();

	//	unsigned char  *brush_roi_data = brush_roi.data;



	//	cout << "Brush : " << setw(4)<<nth + 1 << " " << setw(4) << crop_s.x << ", " << setw(4) << crop_s.y << ", " << setw(4) << crop_e.x << ", " << setw(4) << crop_e.y << endl;

		brush_outlined = mask_8UC1.clone();
		cv::rectangle(brush_outlined, b_effective_rect, RGB(0, 0, 0));
		cv::line(brush_outlined, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_outlined, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/br_outlined_", nth + 1, brush_outlined);
		//f_name = format("/rst/br/br_%d_0crop.ppm", nth + 1);
		//cv::imwrite(f_name, brush_outlined);// 1CH

		
		b_effective_centered_rect.width = b_effective_rect.width;
		b_effective_centered_rect.height = b_effective_rect.height;
		b_effective_centered_rect.x = (bsize_width - b_effective_rect.width) / 2;
		b_effective_centered_rect.y = (bsize_height - b_effective_rect.height) / 2;

		if (centered_s.x > b_effective_centered_rect.x)
			centered_s.x = b_effective_centered_rect.x;
		if (centered_s.y > b_effective_centered_rect.y)
			centered_s.y = b_effective_centered_rect.y;

		if (centered_e.x <( b_effective_centered_rect.x + b_effective_rect.width))
			centered_e.x = (b_effective_centered_rect.x + b_effective_rect.width);
		if (centered_e.y < (b_effective_centered_rect.y + b_effective_rect.height))
			centered_e.y = (b_effective_centered_rect.y + b_effective_rect.height);

		if (effective_centered_sz.width < b_effective_centered_rect.width)
			effective_centered_sz.width = (b_effective_centered_rect.width);
		if (effective_centered_sz.height < (b_effective_centered_rect.height))
			effective_centered_sz.height = (b_effective_centered_rect.height);
		cout <<setw(5)<<nth
			<< setw(5) << b_effective_centered_rect.x << ", " 
			<< setw(5) << b_effective_centered_rect.y << ", "
			<< setw(5) << b_effective_centered_rect.width << ", " 
			<< setw(5) << b_effective_centered_rect.height <<
			endl;
		//	PaintBackGround(b160x160, 255, 255, 255);
			//unsigned char * b160x160_data = b160x160.data;
		//mask_8uc1 : read from filed, centered_data : effecive_centered,
	//	mat_copyTo(mask_8UC1_data, centered_data[nth], b_effective_rect, b_effective_centered_rect, bsize_width);
		Mat mask_8UC1_ROI = mask_8UC1(b_effective_rect);
		Mat centered_8UC1_ROI = (*centered[nth])(b_effective_centered_rect);
		mask_8UC1_ROI.copyTo(centered_8UC1_ROI);
		debug_image("br/br_centered", nth + 1, centered[nth]);
#define CENTERED_EFFECTED true
#ifdef CENTERED_EFFECTED
		Mat centered_ROI = mask_8UC1(b_effective_rect);
		centered_ROI.copyTo(centered_ppm);
		debug_image("br/centered_ROI_"+to_string(centered_ROI.size().width) +"_"+ to_string(centered_ROI.size().height), nth+1, centered_ROI);
		//fromat fmt = "%02d";
	//	cout << setw(5)<<(nth)<<setw(5)<<(centered_ROI.size().width)<<setw(5)<<(centered_ROI.size().height) << endl;
		f_name = format("/render/stencil/stencil_%d.pgm", nth + 1);
		vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PXM_BINARY);
		compression_params.push_back(1);
		cv::imwrite(f_name, centered_ppm, compression_params);// 1CH
#endif
									  //Make accumulated brush map 
		unsigned char * s;
		s = mask_8UC1.data;
		int s_i;
		unsigned char * c_s;
		c_s = centered[nth]->data;

		for (int x = 0; x < bsize_width; x++)
			for (int y = 0; y < bsize_height; y++)
			{
				s_i = x + y * bsize_width * 1;

				for (int n = 0; n < 1; n++) {
					if (s[s_i] != 255) {

						accumulation_data[s_i] = s[s_i];
						accumulation_centered_data[s_i] = c_s[s_i];
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
		centered[nth]->copyTo(*centered_outlined[nth]);
		cv::rectangle(*centered_outlined[nth], b_effective_centered_rect, RGB(0, 0, 0));
		cv::line(*centered_outlined[nth], Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(*centered_outlined[nth], Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/centered_outlined", nth + 1, *centered_outlined[nth]);
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

	}//for nth to get effective_centered brush

		Rect r = Rect(Point(0, 0), Point(bsize_width - 1, bsize_height - 1));
		debug_image("br/0_acc_map_",nth,brush_accumulation_map);

		//	cv::imwrite("/rst/br/0_brush_accumulation_map.ppm", brush_accumulation_map);

		Rect acc_rr = rounding_box(brush_accumulation_map, bsize_width, bsize_height,g_alpha_TH);
			cout << "accumulation "<< acc_rr.x << ", " << acc_rr.y << ", " << acc_rr.width << ", " << acc_rr.height << " : "<<bsize_width-(acc_rr.x+ acc_rr.width)
				<<", "<< bsize_height-(acc_rr.y+ acc_rr.height)

				<<endl;

		crop_s.x = acc_rr.x;
		crop_s.y = acc_rr.y;
		crop_e.x = crop_s.x + acc_rr.width;
		crop_e.y = crop_s.y + acc_rr.height;
		r = Rect(crop_s, crop_e);
	/*	original outline
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_e.x, crop_s.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_s.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_e.x, crop_s.y), Point(crop_e.x, crop_e.y), 0);
		line(brush_accumulation_map, Point(crop_s.x, crop_e.y), Point(crop_e.x, crop_e.y), 0);
		*/
		//cv::rectangle(brush_accumulation_map, acc_rr, RGB(0, 0, 0));

		Rect centered_rr = rounding_box(brush_centered_accumulation_map, bsize_width, bsize_height, g_alpha_TH);

	//	cv::rectangle(brush_accumulation_map, r, RGB(0, 0, 0));
		
		cv::line(brush_accumulation_map, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_accumulation_map, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));

		

		debug_image("br/0_acc_map_outlined",nth, brush_accumulation_map);
		//cv::imwrite("/rst/br/0_accumulation_map_crop.ppm", brush_accumulation_map);

		debug_image("br/0_brush_centered_acc_map",nth, brush_centered_accumulation_map);

		cout << "centered_s " <<
			centered_s.x << ", " <<
			centered_s.y << endl;

		cout << "centered_e " <<
			centered_e.x << ", " <<
			centered_e.y << endl;
			 cout <<"centered x: "<< centered_rr.x << ",y: " << centered_rr.y << ",w: " << centered_rr.width << ",h: " << centered_rr.height <<" :sizew "<<bsize_width-(centered_rr.x+ centered_rr.width)
				<<",sizeh "<< bsize_height-(centered_rr.y+centered_rr.height)
				 << endl;
			 
			 cout << "centered sz w: " << effective_centered_sz.width << " ,h:  " << effective_centered_sz.height << endl;
	//	cv::rectangle(brush_centered_accumulation_map, r, RGB(0, 0, 0));
			 Point centered_crop_s, centered_crop_e;
			 centered_crop_s.x = centered_rr.x;
			 centered_crop_s.y = centered_rr.y;
			 centered_crop_e.x = centered_crop_s.x + centered_rr.width;
			 centered_crop_e.y = centered_crop_s.y + centered_rr.height;
		Rect centered_crop_rr(centered_s, centered_e);

		cout << "centered crop x: " << centered_crop_rr.x << ",y: " << centered_crop_rr.y << ",w: " << centered_crop_rr.width << ",h: " << centered_crop_rr.height << endl;

		cv::rectangle(brush_centered_accumulation_map, centered_crop_rr, RGB(0, 0, 0));
	
		cv::line(brush_centered_accumulation_map, Point(bsize_height / 2, 0), Point(bsize_height / 2, bsize_width), RGB(0, 0, 0));
		cv::line(brush_centered_accumulation_map, Point(0, bsize_width / 2), Point(bsize_height, bsize_width / 2), RGB(0, 0, 0));
		debug_image("br/0_brush_centered_acc_map_outlined",nth, brush_centered_accumulation_map);
		int w = 140;
		centered_crop_rr.x = (200 - w)/2;
		centered_crop_rr.y = (200 - w)/2;
		centered_crop_rr.width = w;
		centered_crop_rr.height = w;
		cv::rectangle(brush_centered_accumulation_map, centered_crop_rr, RGB(0, 0, 0));
		debug_image("br/0_brush_centered_acc_map_160_outlined", nth, brush_centered_accumulation_map);
#ifndef CENTERED_EFFECTED
		nth = 0;
		for (; nth < g_BrushNumber; nth++) {
			Mat centered_ROI =(*centered[nth])(centered_crop_rr);
			centered_ROI.copyTo(centered_ppm);

			f_name = format("/render/stencil/stencil_%d.pgm", nth + 1);
			vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PXM_BINARY);
			compression_params.push_back(1);
			cv::imwrite(f_name, centered_ppm, compression_params);// 1CH
			debug_image("br/c_", nth, centered_ppm);
		}
#endif
	return 0;
}
#endif