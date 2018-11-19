//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
#include "stdafx.h"


#include "util.h"
#include "render_.h"
#include "rgb2hsv.h"
#include "time_stamp.h"
#include "Judgement.h"
#define DEBUG_BRUSH_CNT 20

	int render_::P_Rendering(Mat & _src_ROI_canvas_clone, Mat & _before_ROI_canvas_clone, 
		cv::Mat & _changed_ROI_clone, 
		cv::Mat & _ing_ROI_clone, 
		Point _fetch_color_Point, Point centered_SrtPoint, Point canvas_centered_SrtPoint,
		Point canvas_centered_EndPoint, int _brush_area_w_size, int _brush_area_h_size, 
		int astroke_depth, int painting_try, int color_BGR_B, int color_BGR_G, int color_BGR_R,
		int _depth,int _try_,
		Mat _changed_canvas_ROI
	)


{
		static int tbrush_cnt = 0;
		static int saved_depth = -1;
		if (saved_depth < astroke_depth)
		{
			saved_depth = astroke_depth;
			tbrush_cnt = 0;
		}
		
	cv::Mat bestBrush_gray_resized;
	//cv::Mat bestBrush_embossed_src;
	cv::Mat bestBrush_embossed_resized;//R
	//cv::Mat paint_by_brush(brush_area_h_size,brush_area_w_size,CV_8UC3);
	//int br_BGR_B, br_BGR_G, br_BGR_R;

	int Alpha;
#ifdef DEBUG_ALPHA
	Scalar s_rgb, s_hsv;
	Mat alpha_channel;
	Mat hsv_v;
#endif
	unsigned char * ing_ROI_clone_data = _ing_ROI_clone.data;

	unsigned char * src_ROI_canvas_clone_data = _src_ROI_canvas_clone.data;
	
	unsigned char *before_ROI_canvas_clone_data = _before_ROI_canvas_clone.data;
	unsigned char * changed_ROI_clone_data = _changed_ROI_clone.data;
	unsigned char * changed_ROI_data = _changed_canvas_ROI.data;

	int canvas_ROI_clone_width = _before_ROI_canvas_clone.size().width;
	int canvas_ROI_clone_height = _before_ROI_canvas_clone.size().height;
	int canvas_ROI_clone_step1 = (int)_before_ROI_canvas_clone.step1();
	int canvas_ROI_clone_channels = (int)_before_ROI_canvas_clone.channels();

	int canvas_ROI_width = _changed_canvas_ROI.size().width;
	int canvas_ROI_height = _changed_canvas_ROI.size().height;
	int canvas_ROI_step1 = (int)_changed_canvas_ROI.step1();
	int canvas_ROI_channels = (int)_changed_canvas_ROI.channels();
	
	Mat src_ROI_clone_resized_8UC1;
	cv::cvtColor(_src_ROI_canvas_clone,src_ROI_clone_resized_8UC1, COLOR_RGB2GRAY);
	resize(src_ROI_clone_resized_8UC1, src_ROI_clone_resized_8UC1, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));

	if (tbrush_cnt < DEBUG_BRUSH_CNT) {
		debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "0src_ROI_ ", _src_ROI_canvas_clone);
		debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "1src_ROI_ ", src_ROI_clone_resized_8UC1);
		debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "2bef_ROI_ ", _before_ROI_canvas_clone);
		debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "3chg_ROI_", _changed_ROI_clone);
	}
	string tag="/br/p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_);
	int brush_no = JudgementBrush(src_ROI_clone_resized_8UC1, /*added by cwlee*/astroke_depth,
		g_brush_thumbnail_size, g_brush_thumbnail_size, g_brush_set,tag);

	bestBrush_gray_resized= brush_resized_set[astroke_depth].at(brush_no)->brush_gray;
	bestBrush_embossed_resized = brush_resized_set[astroke_depth].at(brush_no)->bump;
	unsigned char * bestBrush_data_gray_resized = (unsigned char *)bestBrush_gray_resized.data;
	unsigned char * bestBrush_embossed_resized_data = bestBrush_embossed_resized.data;
	
	int b_width = _brush_area_w_size;
	int b_height = _brush_area_h_size;
	int b_step1 = (int)bestBrush_gray_resized.step1();
	int b_channels = bestBrush_gray_resized.channels();

//#ifdef DEBUG_BRUSH


//#endif
//paint_by_brush_ptr[bIndex_3c + _BGR_B] = color_BGR_B;
//	paint_by_brush_ptr[bIndex_3c + _BGR_G] = color_BGR_G;
//paint_by_brush_ptr[bIndex_3c + _BGR_R] = color_BGR_R;
//br_BGR_R = paint_by_brush[bIndex_3c + _BGR_R];
//br_BGR_G = paint_by_brush[bIndex_3c + _BGR_G];
//br_BGR_B = paint_by_brush[bIndex_3c + _BGR_B];
	for (int by = 0; by < _brush_area_h_size; by++)
	{
		for (int bx = 0; bx < _brush_area_w_size; bx++)
		{
			int canvas_ROI_clone_Index_3c = by *canvas_ROI_clone_step1 + bx * canvas_ROI_clone_channels;
			int gray_bIndex_1c = by*_brush_area_w_size + bx;
				Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
				if (Alpha < g_alpha_TH) {
					p_poke(changed_ROI_clone_data, canvas_ROI_clone_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
				}
				
		}
				
	}


//#ifdef DEBUG_ALPHA
	if (tbrush_cnt < DEBUG_BRUSH_CNT) {

			debug_image("br/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "_brN_" + to_string(brush_no), bestBrush_gray_resized);
			debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth) + "_t_" + to_string(_try_) + "5chg_ROI_", _changed_ROI_clone);
			tbrush_cnt++;
	}
//#endif
	
	int  int_result = JudgementImage(src_ROI_canvas_clone_data, changed_ROI_clone_data, before_ROI_canvas_clone_data,
		_brush_area_w_size, _brush_area_h_size, 
		centered_SrtPoint, 
		//_fetch_color_Point,
		astroke_depth,
		b_width, b_height, b_channels, b_step1,
		canvas_ROI_clone_width, canvas_ROI_clone_height, canvas_ROI_clone_channels,canvas_ROI_clone_step1,
		m_tag);

	int p_step = (int)paint_map_8UC1[0].step1();
	if (int_result == CHANGED_BETTER) {

		for (int by = 0; by < _brush_area_h_size; by++)
		{
			for (int bx = 0; bx < _brush_area_w_size; bx++)
			{
				int canvas_ROI_clone_Index_3c = by *canvas_ROI_clone_step1 + bx * canvas_ROI_clone_channels;
				int canvas_Index_1c = (canvas_centered_SrtPoint.y+by) *p_step + (canvas_centered_SrtPoint.x+bx) ;
				int canvas_ROI_Index_3c = by *canvas_ROI_step1 + bx * canvas_ROI_channels;
				int gray_bIndex_1c = by*_brush_area_w_size + bx;
				int bIndex_3c = by *b_step1 + bx * b_channels;

					Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
					if (Alpha < g_alpha_TH) {
						
						p_poke(ing_ROI_clone_data, canvas_ROI_clone_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						p_poke(changed_ROI_data, canvas_ROI_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						p_poke_add_n(paint_map_8UC1_data[_depth], canvas_Index_1c,1);
					}
			}//for x
		}//for y
		if (tbrush_cnt < DEBUG_BRUSH_CNT) {

			debug_image("p" + to_string(_depth) + "/" + m_tag_ + to_string(_depth)+"t_" + to_string(_try_) + "6ing_ROI_", _changed_ROI_clone);
		}
	} // if changed better


#ifdef DEBUG_ALPHA
	alpha_channel.release();
	hsv_v.release();
#endif
	return int_result;
}

