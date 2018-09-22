//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
#include "stdafx.h"

#include "p_rendering.h"
#include "util.h"
#include "rgb2hsv.h"
#include "render_.h"
void proof_box(Point &s, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0)
		cout << "error 1:" << s.x << endl;
	if (s.x >= i_width)
		cout << "error 2:" << s.x << endl;



	if (s.y < 0) cout << "error 3:" << s.y << endl;

	if (s.y >= i_height)
		cout << "error 4:" << s.y << endl;

}

void proof_box(Point &s, int i_width, int i_height, char * p) {
	//Rect rr;
	if (s.x < 0)
		cout << "error 1:" << p << s.x << endl;
	if (s.x >= i_width)
		cout << "error 2:" << p << s.x << endl;



	if (s.y < 0) cout << "error 3:" << p << s.y << endl;

	if (s.y >= i_height)
		cout << "error 4:" << p << s.y << endl;

}


Mat brush_at_brush(list<Brush> & _brush_set, int no) {

	list<Brush>::iterator it = _brush_set.begin();
	for (; it != _brush_set.end(); it++) {
		if ((*it).brush_no == no)
			return (*it).brush.clone();
	}
	return _brush_set.front().brush.clone();
}

Mat brush_at_brush_embose(list<Brush> & _brush_set, int no) {

	list<Brush>::iterator it = _brush_set.begin();
	for (; it != _brush_set.end(); it++) {
		if ((*it).brush_no == no)
			return (*it).bump.clone();
	}
	return _brush_set.front().bump.clone();
}
Mat brush_at_brush_gray(list<Brush> & _brush_set, int no) {

	list<Brush>::iterator it = _brush_set.begin();
	for (; it != _brush_set.end(); it++) {
		if ((*it).brush_no == no)
			return (*it).brush_gray.clone();
	}
	cerr << "at G " << no << endl;
	return _brush_set.front().brush_gray.clone();
}



int   render_::P_Rendering(//cv::Mat srcImg,
//	unsigned char * srcData,
	unsigned char * changedData,//rstImage
	Mat & beforeImg,//rstImage.clone()

	cv::Mat &_testImg_resized,
	//list<Brush> &_brush_set,
	Point _fetch_color_Point,
	Point centered_SrtPoint,
	int paint_grid_w_size, int paint_grid_h_size,
	//String tag,
	int astroke_number, int ing, int astroke_depth, int painting_count, 
	int color_BGR_B, int color_BGR_G, int color_BGR_R,//BGR order
	unsigned char * ing_canvas_data
	
	)
	
{
	unsigned char *beforeData = beforeImg.data;
	int s_width = beforeImg.size().width;
	int s_height = beforeImg.size().height;
	int s_step1 = (int)beforeImg.step1();
	int s_channels = (int)beforeImg.channels();

	int brush_no = JudgementBrush(_testImg_resized, /*added by cwlee*/astroke_depth,
		g_brush_thumbnail_size, g_brush_thumbnail_size, brush_set);
	//, stroke_no, paint_grid_w_size, paint_grid_h_size,
	//centered_SrtPoint,centered_EndPoint,first_try);//get one of similiar best 5
	cv::Mat bestBrush_src;
	cv::Mat bestBrush_resized;
	cv::Mat bestBrush_gray_src;
	cv::Mat bestBrush_gray_resized;
	cv::Mat bestBrush_embossed_src;
	cv::Mat bestBrush_embossed_resized;//R
	Mat alpha_channel;
	Mat hsv_v;

	//bestBrush_src = brush_at_brush(_brush_set, brush_no);
	bestBrush_gray_src= brush_at_brush_gray(brush_set, brush_no);
	bestBrush_embossed_src = brush_at_brush_embose(brush_set, brush_no);

	//unsigned char * bestBrush_src_data = (unsigned char *)bestBrush_src.data;
	unsigned char * bestBrush_embose_src_data = (unsigned char *)bestBrush_embossed_src.data;

	int brush_size_width = bestBrush_src.cols;
	int brush_size_height = bestBrush_src.rows;

//	cv::resize(bestBrush_src, bestBrush_resized, cv::Size(paint_grid_w_size, paint_grid_h_size));
	cv::resize(bestBrush_gray_src, bestBrush_gray_resized, cv::Size(paint_grid_w_size, paint_grid_h_size));
	cv::resize(bestBrush_embossed_src, bestBrush_embossed_resized, cv::Size(paint_grid_w_size, paint_grid_h_size));

	static int tbrush_cnt = 0;
	bestBrush_resized.create(paint_grid_h_size, paint_grid_w_size, CV_8UC3);
	bestBrush_resized.setTo(255);

	unsigned char * bestBrush_data_resized = (unsigned char *)bestBrush_resized.data;
	unsigned char * bestBrush_data_gray_resized = (unsigned char *)bestBrush_gray_resized.data;
	unsigned char * bestBrush_embossed_resized_data = bestBrush_embossed_resized.data;

	 if (tbrush_cnt < 5) {
		 
	//	debug_image("br/T_bestBrush_src_" + tag, tbrush_cnt, bestBrush_src);
		debug_image("br/T_bestBrush_gray_src_" + m_tag+to_string(brush_no), tbrush_cnt, bestBrush_gray_src);
		debug_image("br/T_bestBrush_embossed_src_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_embossed_src);

	//	debug_image("br/IP0_bestBrush_resized_" + tag + to_string(brush_no), tbrush_cnt, bestBrush_resized);
		debug_image("br/IP0_brush_gray_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_gray_resized);
		debug_image("br/IP0_brush_embossed_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_embossed_resized);
	//	if (tbrush_cnt == 0) {
	//		mat_print(bestBrush_resized, "bestBrush_resized");
	//		mat_print(bestBrush_gray_resized, "bestBrush_gray_resized");
	//		mat_print(bestBrush_embossed_resized, "bestBrush_embossed_Resized");
	//	}
		

	 }

	int b_step1 = (int)bestBrush_resized.step1();
	int b_channels = bestBrush_resized.channels();


	int br_BGR_B, br_BGR_G, br_BGR_R;
	Point p;
	Scalar s_rgb,s_hsv;
	
	
	alpha_channel.create(paint_grid_w_size, paint_grid_h_size, CV_8UC1);
	alpha_channel.setTo(0);

	unsigned char * alpha_channel_data = alpha_channel.data;
	//debug_image("br/IP0_alpha_" + tag, tbrush_cnt, alpha_channel);
	

	hsv_v.create(paint_grid_w_size, paint_grid_h_size, CV_8UC1);
	hsv_v.setTo(0);
	unsigned char * hsv_v_data = hsv_v.data;

	
		int Alpha;// bestBrush_data[bIndex] /200.;
	
	

	//g_brush_style = BRUSH_ALPHA;

	for (int by = 0; by < paint_grid_h_size; by++)
	{
		for (int bx = 0; bx < paint_grid_w_size; bx++)
		{

			p.x = (centered_SrtPoint.x + bx);
			p.y = (centered_SrtPoint.y + by);
			bool c_flag = false;

			if (p.x < 0) c_flag = true;
			if (p.y < 0) c_flag = true;
			if (p.x >(s_width - 1)) c_flag = true;
			if (p.y >(s_height - 1))  c_flag = true;
			//브러시에 색상 입히기

			int bIndex_3c = by *b_step1 + bx * b_channels;
			int gray_bIndex_1c = by*paint_grid_w_size + bx;
		
			//for tranparent effect 

		//	if (bestBrush_data_gray[gray_bIndex] > g_brush_Ts) c_flag = true;

			if (bestBrush_data_gray_resized[gray_bIndex_1c] > g_brush_Ts) {
				c_flag = true;
				alpha_channel_data[gray_bIndex_1c] = 50;
				continue;
			}
			//	
			if (c_flag == true) {
			//	p_poke(bestBrush_data_re, bIndex, 255, 255, 255);
				alpha_channel_data[gray_bIndex_1c] = 99;
				continue;
			}

			int sIndex = p.y * s_step1 + p.x * s_channels;

			if (g_brush_style == BRUSH_ALPHA) {
				s_rgb.val[_R] = color_BGR_R;//R
				s_rgb.val[_G] = color_BGR_G;//G
				s_rgb.val[_B] = color_BGR_B;//B
				RGB2HSV(s_rgb, s_hsv);

				hsv_v_data[gray_bIndex_1c] = (int)s_hsv.val[_V];

				//	Alpha = bestBrush_data_resized[bIndex];// T(x, y);
		//		Alpha = (int)s_hsv.val[2];
				Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
				if (Alpha < g_TH) {
				
					s_hsv.val[2] = s_hsv.val[2] + g_ET*(bestBrush_embossed_resized_data[gray_bIndex_1c] - 128);
					 HSV2RGB(s_hsv, s_rgb);
						bestBrush_data_resized[bIndex_3c + _BGR_R]=(int)s_rgb.val[_R];//R
						bestBrush_data_resized[bIndex_3c + _BGR_G] = (int)s_rgb.val[_G];//G
						bestBrush_data_resized[bIndex_3c + _BGR_B] = (int)s_rgb.val[_B];//B
						alpha_channel_data[gray_bIndex_1c] = 255;
				}
				else {
					alpha_channel_data[gray_bIndex_1c ] = 150;//set alpha to transparent
				}
				if (alpha_channel_data[gray_bIndex_1c]> 200) {
					br_BGR_R = bestBrush_data_resized[bIndex_3c + _BGR_R];
					br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
					br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];

					p_poke(changedData, sIndex, br_BGR_B, br_BGR_G, br_BGR_R);
				}
			}

			else if (g_brush_style == BRUSH_COPY) {
				bestBrush_data_resized[bIndex_3c + _BGR_B] = color_BGR_B;
				bestBrush_data_resized[bIndex_3c + _BGR_G] = color_BGR_G;
				bestBrush_data_resized[bIndex_3c + _BGR_R] = color_BGR_R;
				br_BGR_R = bestBrush_data_resized[bIndex_3c + _BGR_R];
				br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
				br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];
				alpha_channel_data[gray_bIndex_1c] = 255;
					p_poke(changedData, sIndex, br_BGR_B, br_BGR_G, br_BGR_R);
			}

		

	
			//그리기
			
		}
	}


	if (tbrush_cnt < 5) {
		debug_image("br/IP1_bestBrush_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_resized);
		debug_image("br/IP1_alpha_" + m_tag + to_string(brush_no), tbrush_cnt,alpha_channel);
		if ( g_brush_style == BRUSH_ALPHA)
			debug_image("br/IP1_hsv_v_" + m_tag + to_string(brush_no), tbrush_cnt, hsv_v);
		tbrush_cnt++;
	}
	//판단알고리즘 적용



	int  int_result = JudgementImage(m_srcData, changedData, beforeData,
		paint_grid_w_size, paint_grid_h_size, centered_SrtPoint,
		astroke_depth, s_width, s_height, s_channels,_fetch_color_Point,m_tag);

	if (int_result == CHANGED_BETTER) {
		for (int by = 0; by < paint_grid_h_size; by++)
		{
			for (int bx = 0; bx < paint_grid_w_size; bx++)
			{

				p.x = (centered_SrtPoint.x + bx);
				p.y = (centered_SrtPoint.y + by);
				int gray_bIndex_1c = by*paint_grid_w_size + bx;
				int bIndex_3c = by *b_step1 + bx * b_channels;
				//int sIndex = p.y * s_step1 + p.x * s_channels;

				
				if (alpha_channel_data[gray_bIndex_1c] > 200) {
					br_BGR_R = bestBrush_data_resized[bIndex_3c + _BGR_R];
					br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
					br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];
					p_poke_canvas(ing_canvas_data, p.x, p.y, br_BGR_B, br_BGR_G, br_BGR_R);
				}

			}//for x
		}//for y
		
	} // if changed better

	bestBrush_src.release();
	bestBrush_resized.release();
	bestBrush_gray_src.release();
	bestBrush_gray_resized.release();
	bestBrush_embossed_src.release();
	bestBrush_embossed_resized.release();//R
	alpha_channel.release();
	hsv_v.release();
	return int_result;
}

