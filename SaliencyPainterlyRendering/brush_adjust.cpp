
#include "stdafx.h"
#include "debug_define.h"
#include "brush.h"
#include "debug_image.h"
#include "util.h"
#include <iostream>
#include "extern.h"
#include "image_util.h"
using namespace std;

//#define  DO_CENTERED_BRUSH true
#ifdef DO_CENTERED_BRUSH
int Brush_adjust(vector <Brush*> &brush_set)
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

	Point crop_s, crop_e;




	Size sz;




	int nth;
	cv::Mat brush_accumulation_map;
	cv::Mat brush_centered_accumulation_map;
	cv::Mat brush_cr, brush_th;
	string f_name;
	//Mat b160x160;
	//b160x160.create(160, 160, CV_8UC1);
	int bsize_x, bsize_y;



//	brush_accumulation_map.create(m_brush_size_x, m_brush_size_y, CV_8UC1);
	//brush_centered_accumulation_map.create(m_brush_size_x, m_brush_size_y, CV_8UC1);
	//	unsigned char * s;
	//unsigned char *d;
	unsigned char * accumulation_data = brush_accumulation_map.data;
	unsigned char * accumulation_centered_data = brush_centered_accumulation_map.data;
	brush_accumulation_map.setTo(255);
	brush_centered_accumulation_map.setTo(255);
	vector <Brush*>::iterator it = brush_set.begin();
	nth = 0;
	for (; nth < g_BrushNumber; nth++, it++) {
		string fname;

		fname = cv::format("./brush/mask/%02d.ppm", nth + 1);
		temp_brush = imread(fname.c_str(), IMREAD_GRAYSCALE);

		unsigned char * temp_brush_data = temp_brush.data;
		bsize_x = temp_brush.size().width;
		bsize_y = temp_brush.size().height;
		if (temp_brush.size().width == 0 || temp_brush.size().height == 0)
		{
			cout << fname << endl;
			exit(-9998);
		}
		(*it)->brush_mask.push_back(temp_brush);

		(*it)->brush_no = nth;
		//mat_print(temp_brush, "temp_brush");

		Rect b_rect;
		b_rect = rounding_box(temp_brush, bsize_x, bsize_y, 235);

		//	crop_s.x = b_rect.x;
			//crop_s.y = b_rect.y;
			//crop_e.x = crop_s.x + b_rect.width;
		//	crop_e.y = crop_s.y + b_rect.height;

		cv::Mat centered;
		centered.create(bsize_x, bsize_y, CV_8UC1);
		PaintBackGround(centered, 255, 255, 255);
		unsigned char * centered_data = centered.data;

		//	cv::Mat brush_roi = temp_brush(b_rect).clone();

	//	unsigned char  *brush_roi_data = brush_roi.data;



	//	cout << "Brush : " << setw(4)<<nth + 1 << " " << setw(4) << crop_s.x << ", " << setw(4) << crop_s.y << ", " << setw(4) << crop_e.x << ", " << setw(4) << crop_e.y << endl;

		brush_cr = temp_brush.clone();
		cv::rectangle(brush_cr, b_rect, RGB(0, 0, 0));
		cv::line(brush_cr, Point(bsize_y / 2, 0), Point(bsize_y / 2, bsize_x), RGB(0, 0, 0));
		cv::line(brush_cr, Point(0, bsize_x / 2), Point(bsize_y, bsize_x / 2), RGB(0, 0, 0));
		debug_image("br/br_adj_", nth + 1, brush_cr);
		//f_name = format("/rst/br/br_%d_0crop.ppm", nth + 1);
		//cv::imwrite(f_name, brush_cr);// 1CH

		Rect b_rect2;
		b_rect2.width = b_rect.width;
		b_rect2.height = b_rect.height;
		b_rect2.x = (bsize_x - b_rect.width) / 2;
		b_rect2.y = (bsize_y - b_rect.height) / 2;


		//	PaintBackGround(b160x160, 255, 255, 255);
			//unsigned char * b160x160_data = b160x160.data;

		mat_copyTo(temp_brush_data, centered_data, b_rect, b_rect2, bsize_x);
		debug_image("br/br_centered", nth + 1, centered);
		//f_name = format("/rst/br/br_%d_1centered.ppm", nth + 1);
		//cv::imwrite(f_name, centered);// 1CH

									  //Make accumulated brush map 
		unsigned char * s;
		s = temp_brush.data;
		int s_i;

		for (int x = 0; x < bsize_x; x++)
			for (int y = 0; y < bsize_y; y++)
			{
				s_i = x + y * bsize_x * 1;

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
		cv::line(centered, Point(bsize_y / 2, 0), Point(bsize_y / 2, bsize_x), RGB(0, 0, 0));
		cv::line(centered, Point(0, bsize_x / 2), Point(bsize_y, bsize_x / 2), RGB(0, 0, 0));
		debug_image("br/centered_1", nth + 1, centered);
		//cv::imwrite(f_name, centered);// 1CH

		//debug_image("br/centered_2",nth+1,centered)
		//f_name = format("/rst/br/br_%d_2centered.ppm", nth + 1);
		//cv::imwrite(f_name, centered);// 1CH
	

		cv::Size siz = Size(g_INDEX_BRUSH_SIZE_WIDTH, g_INDEX_BRUSH_SIZE_HEIGHT);
		//	cout << s.height << "," << s.width << endl;
		cv::resize(temp_brush, temp_index_brush, siz);
		(*it)->index_brush.push_back(temp_index_brush);

		fname = cv::format("./brush/bump/%02d.ppm", nth + 1);
		temp_bump = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_bump.size().width == 0 ||
			temp_bump.size().height == 0) {
			cout << "bump file error " << " : " << nth + 1 << endl;
		}
		(*it)->bump.push_back(temp_bump);




		//fname = cv::format("/rst/br/i_%d.ppm", nth + 1);
	//	cv::imwrite(fname, temp_index_brush);

		fname = cv::format("./brush/mask-thumbnail/%02d.ppm", nth + 1);
		temp_thumbnail = imread(fname.c_str(), IMREAD_GRAYSCALE);
		if (temp_thumbnail.size().width == 0 ||
			temp_thumbnail.size().height == 0) {
			cout << "thumbnail file error " << " : " << nth + 1 << endl;
		}
		(*it)->brush_thumbnail.push_back(temp_thumbnail);
		int t_size =  temp_thumbnail.size().width;
		b_rect = rounding_box(temp_thumbnail, t_size, t_size, 235);// bsize_x, bsize_y, 235);

		crop_s.x = b_rect.x;
		crop_s.y = b_rect.y;
		crop_e.x = crop_s.x + b_rect.width;
		crop_e.y = crop_s.y + b_rect.height;

		//	cout << "Brush : " << setw(4)<<nth + 1 << " " << setw(4) << crop_s.x << ", " << setw(4) << crop_s.y << ", " << setw(4) << crop_e.x << ", " << setw(4) << crop_e.y << endl;

		brush_th = temp_thumbnail.clone();

		cv::rectangle(brush_th, b_rect, RGB(0, 0, 0));
		cv::line(brush_th, Point(t_size / 2, 0), Point(t_size / 2, t_size), RGB(0, 0, 0));
		cv::line(brush_th, Point(0, t_size / 2), Point(t_size, t_size / 2), RGB(0, 0, 0));

		debug_image("br/th_crop", nth + 1, brush_th);



		//mat_print(temp_thumbnail,"temp_thumbnail");

		if (nth == 0)
		{
			std::string fpath;

			sz = temp_brush.size();
			debug_image_mask = new debug_image_single(sz, g_BrushNumber, temp_brush.type());

			sz = temp_index_brush.size();
			debug_image_index = new debug_image_single(sz, g_BrushNumber, temp_index_brush.type());

			sz = temp_thumbnail.size();
			debug_image_thumbnail = new debug_image_single(sz, g_BrushNumber, temp_thumbnail.type());

			sz = temp_bump.size();
			debug_image_bump = new debug_image_single(sz, g_BrushNumber, temp_bump.type());

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

		//	delete brush_accumulation_map;

	}
	Rect r = Rect(Point(0, 0), Point(bsize_x - 1, bsize_y - 1));
	debug_image("br/acc", brush_accumulation_map);

	//	cv::imwrite("/rst/br/0_brush_accumulation_map.ppm", brush_accumulation_map);

	Rect acc_rr = rounding_box(brush_accumulation_map, bsize_x, bsize_y, 235);
	//	cout << "accumulation "<< rr.x << ", " << rr.y << ", " << rr.width << ", " << rr.height << " : "<<bsize_x-(rr.x+rr.width)
	//		<<", "<< bsize_y-(rr.y+rr.height)

	//		<<endl;

	crop_s.x = acc_rr.x;
	crop_s.y = acc_rr.y;
	crop_e.x = crop_s.x + acc_rr.width;
	crop_e.y = crop_s.y + acc_rr.height;

	cv::rectangle(brush_accumulation_map, r, RGB(0, 0, 0));
	r = Rect(crop_s, crop_e);
	cv::line(brush_accumulation_map, Point(bsize_y / 2, 0), Point(bsize_y / 2, bsize_x), RGB(0, 0, 0));
	cv::line(brush_accumulation_map, Point(0, bsize_x / 2), Point(bsize_y, bsize_x / 2), RGB(0, 0, 0));
	cv::rectangle(brush_accumulation_map, acc_rr, RGB(0, 0, 0));
	line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_e.x, crop_s.y), 0);
	line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_s.x, crop_e.y), 0);
	line(brush_accumulation_map, Point(crop_e.x, crop_s.y), Point(crop_e.x, crop_e.y), 0);
	line(brush_accumulation_map, Point(crop_s.x, crop_e.y), Point(crop_e.x, crop_e.y), 0);

	debug_image("br/0_accumulation_map_crop", brush_accumulation_map);
	//cv::imwrite("/rst/br/0_accumulation_map_crop.ppm", brush_accumulation_map);

	debug_image("br/0_brush_centered_accumulation_map", brush_centered_accumulation_map);


	Rect rr = rounding_box(brush_centered_accumulation_map, bsize_x, bsize_y, 235);

	//	 cout <<"centered "<< rr.x << ", " << rr.y << ", " << rr.width << ", " << rr.height <<" : "<<bsize_x-(rr.x+rr.width)
	////		<<", "<< bsize_y-(rr.y+rr.height)
	//		 << endl;
	cv::rectangle(brush_centered_accumulation_map, r, RGB(0, 0, 0));
	r = Rect(crop_s, crop_e);
	cv::rectangle(brush_centered_accumulation_map, rr, RGB(0, 0, 0));
	line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_e.x, crop_s.y), 0);
	line(brush_accumulation_map, Point(crop_s.x, crop_s.y), Point(crop_s.x, crop_e.y), 0);
	line(brush_accumulation_map, Point(crop_e.x, crop_s.y), Point(crop_e.x, crop_e.y), 0);
	line(brush_accumulation_map, Point(crop_s.x, crop_e.y), Point(crop_e.x, crop_e.y), 0);
	cv::line(brush_centered_accumulation_map, Point(bsize_y / 2, 0), Point(bsize_y / 2, bsize_x), RGB(0, 0, 0));
	cv::line(brush_centered_accumulation_map, Point(0, bsize_x / 2), Point(bsize_y, bsize_x / 2), RGB(0, 0, 0));
	debug_image("br/0_brush_centered_accumulation_map_crop", brush_centered_accumulation_map);



	if (debug_image_mask != nullptr)
		debug_image_mask->image_final();
	if (debug_image_index != nullptr)
		debug_image_index->image_final();
	if (debug_image_thumbnail != nullptr)
		debug_image_thumbnail->image_final();
	if (debug_image_bump != nullptr)
		debug_image_bump->image_final();

	return 0;
	}
#endif