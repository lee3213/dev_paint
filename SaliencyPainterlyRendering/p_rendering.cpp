//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
#include "stdafx.h"

#include "p_rendering.h"
#include "util.h"
#include "render_.h"
#include "rgb2hsv.h"

void render_::proof_box(Point &s, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << s.y << endl;

}

void render_::proof_box(Point &s, int i_width, int i_height, char * p) {
	//Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << p << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << p << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << p << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << p << s.y << endl;

}





int   render_::P_Rendering(//cv::Mat srcImg,
//	unsigned char * srcData,
//	unsigned char * changed_canvas_data,//rstImage
Mat & _src_ROI_canvas,
	Mat & _before_ROI_canvas,
	cv::Mat & _changed_ROI_clone,
	cv::Mat & _ing_ROI_clone_q,
	//vector <Brush*> &_brush_set,
	Point _fetch_color_Point,
	Point _centered_SrtPoint,
	Point canvas_centered_SrtPoint,
	Point canvas_centered_EndPoint,
	int brush_area_w_size, int brush_area_h_size,
	//String tag,
	//int astroke_number,
	//int ing,
	int astroke_depth, int painting_count, 
	int color_BGR_B, int color_BGR_G, int color_BGR_R//BGR order
	//unsigned char * _ing_canvas_data

	)
	
{

	//cv::Mat bestBrush_src;
	cv::Mat bestBrush_resized;
	//cv::Mat bestBrush_gray_src;
	cv::Mat bestBrush_gray_resized;
	//cv::Mat bestBrush_embossed_src;
	cv::Mat bestBrush_embossed_resized;//R
	Mat alpha_channel;
	Mat hsv_v;
	unsigned char * src_ROI_canvas_data = _src_ROI_canvas.data;
	unsigned char * ing_ROI_clone_q_data = _ing_ROI_clone_q.data;
	unsigned char *before_ROI_canvas_data = _before_ROI_canvas.data;
	unsigned char * changed_ROI_clone_data = _changed_ROI_clone.data;
	
	int canvas_ROI_width = _before_ROI_canvas.size().width;

	int canvas_ROI_height = _before_ROI_canvas.size().height;
	int canvas_ROI_step1 = (int)_before_ROI_canvas.step1();
	int canvas_ROI_channels = (int)_before_ROI_canvas.channels();

//	int src_s_width = _src_ROI.size().width;
//	int src_s_height = _src_ROI.size().height;
//	int src_s_step1 = (int)_src_ROI.step1();
//	int src_s_channels = (int)_src_ROI.channels();

	cv::Mat testImg_src = _src_ROI_canvas.clone();
	Mat testImg_resized;
	resize(testImg_src, testImg_resized, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));
	if (testImg_resized.channels() == 3)
		cv::cvtColor(testImg_resized, testImg_resized, COLOR_RGB2GRAY);

	int brush_no = JudgementBrush(testImg_resized, /*added by cwlee*/astroke_depth,
		g_brush_thumbnail_size, g_brush_thumbnail_size, brush_set);

	

	bestBrush_gray_resized= brush_resized_set[astroke_depth].at(brush_no)->brush_gray;
	bestBrush_embossed_resized = brush_resized_set[astroke_depth].at(brush_no)->bump;
	unsigned char * bestBrush_data_gray_resized = (unsigned char *)bestBrush_gray_resized.data;
	unsigned char * bestBrush_embossed_resized_data = bestBrush_embossed_resized.data;


	static int tbrush_cnt = 0;
	bestBrush_resized.create(brush_area_h_size, brush_area_w_size, CV_8UC3);
	bestBrush_resized.setTo(255);
	unsigned char * bestBrush_data_resized = (unsigned char *)bestBrush_resized.data;
	int b_width = brush_area_w_size;
	int b_height = brush_area_h_size;
	int b_step1 = (int)bestBrush_resized.step1();
	int b_channels = bestBrush_resized.channels();

	 if (tbrush_cnt < 5) {
	
		debug_image("br/IP0_brush_gray_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_gray_resized);
		debug_image("br/IP0_brush_embossed_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_embossed_resized);
	 }

	

	int br_BGR_B, br_BGR_G, br_BGR_R;
	Point p;
	Scalar s_rgb,s_hsv;
	

	alpha_channel.create(brush_area_w_size, brush_area_h_size, CV_8UC1);
	alpha_channel.setTo(0);

	unsigned char * alpha_channel_data = alpha_channel.data;
	//debug_image("br/IP0_alpha_" + tag, tbrush_cnt, alpha_channel);
	

	hsv_v.create(brush_area_w_size, brush_area_h_size, CV_8UC1);
	hsv_v.setTo(0);
	unsigned char * hsv_v_data = hsv_v.data;

		int Alpha;// bestBrush_data[bIndex] /200.;
	//g_brush_style = BRUSH_ALPHA;


	for (int by = 0; by < brush_area_h_size; by++)
	{
		for (int bx = 0; bx < brush_area_w_size; bx++)
		{

			p.x = ( bx);
			p.y = ( by);
		/*	bool c_flag = false;

			if (p.x < 0) c_flag = true;
			if (p.y < 0) c_flag = true;
			if (p.x >(src_s_width - 1)) c_flag = true;
			if (p.y >(src_s_height - 1))  c_flag = true;
			//브러시에 색상 입히기
			*/

			int bIndex_3c = by *b_step1 + bx * b_channels;
			int gray_bIndex_1c = by*brush_area_w_size + bx;
		
			//for tranparent effect 

		//	if (bestBrush_data_gray[gray_bIndex] > g_brush_Ts) c_flag = true;

			if (bestBrush_data_gray_resized[gray_bIndex_1c] > g_brush_Ts) {
			
				alpha_channel_data[gray_bIndex_1c] = 50;
				continue;
			}
			
		//	int sIndex = p.y * src_s_step1 + p.x * src_s_channels;
		//	int s_canvas_Index = p.y * canvas_ROI_step1 + p.x * canvas_ROI_channels;
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

					p_poke(changed_ROI_clone_data, bIndex_3c, br_BGR_B, br_BGR_G, br_BGR_R);
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
					p_poke(changed_ROI_clone_data, bIndex_3c, br_BGR_B, br_BGR_G, br_BGR_R);
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


	
	int  int_result = JudgementImage(src_ROI_canvas_data, changed_ROI_clone_data, before_ROI_canvas_data,
		brush_area_w_size, brush_area_h_size, 
		//centered_SrtPoint, 
		//_fetch_color_Point,
		astroke_depth,
		b_width, b_height, b_channels, b_step1,
		canvas_ROI_width, canvas_ROI_height, canvas_ROI_channels,canvas_ROI_step1,
		m_tag);

	if (int_result == CHANGED_BETTER) {
		for (int by = 0; by < brush_area_h_size; by++)
		{
			for (int bx = 0; bx < brush_area_w_size; bx++)
			{

				p.x = ( bx);
				p.y = ( by);
				int gray_bIndex_1c = by*brush_area_w_size + bx;
				int bIndex_3c = by *b_step1 + bx * b_channels;
				//int sIndex = p.y * s_step1 + p.x * s_channels;

				
				if (alpha_channel_data[gray_bIndex_1c] > 200) {
					br_BGR_R = bestBrush_data_resized[bIndex_3c + _BGR_R];
					br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
					br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];
					p_poke(ing_ROI_clone_q_data, bIndex_3c, br_BGR_B, br_BGR_G, br_BGR_R);
//					p_poke(paint_map_data, bIndex_3c, astroke_depth, astroke_depth, astroke_depth);
				}

			}//for x
		}//for y
		
	} // if changed better

	//bestBrush_src.release();
	bestBrush_resized.release();
	//bestBrush_gray_src.release();
	bestBrush_gray_resized.release();
	//bestBrush_embossed_src.release();
	bestBrush_embossed_resized.release();//R
	alpha_channel.release();
	hsv_v.release();
	return int_result;
}

