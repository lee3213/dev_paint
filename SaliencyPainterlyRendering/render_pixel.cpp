//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
#include "stdafx.h"


#include "util.h"
#include "render_.h"
#include "rgb2hsv.h"
#include "time_stamp.h"
#include "Judgement.h"

#include "define_local.h"
/*
int result = P_Rendering(
	src_canvas_ROI,
	before_canvas_ROI_clone,
	changed_canvas_ROI_clone,
	ing_ROI_canvas,
	changed_canvas_ROI,
	centered_ROI_canvas_Rect,
	fetch_color_Point,
	centered_SrtPoint,
	centered_SrtPoint_canvas,
	centered_EndPoint_canvas,
	brush_area_w_size, brush_area_h_size,

	astroke_depth, render_try,
	color_BGR_B, color_BGR_G, color_BGR_R,

	x_src_step1,
	x_canvas_step1,
	x_src_ptr,
	accu_canvas_data[astroke_depth],
	ing_canvas_data[astroke_depth]
);
*/


int render_::P_Rendering(Mat & _src_canvas_ROI,
	Mat & _before_ROI_canvas_clone,
	cv::Mat & _changed_canvas_ROI_clone,
	cv::Mat & _ing_canvas_ROI,
	Mat &_changed_canvas_ROI,
	Mat & _accu_canvas,
	Rect centered_ROI_canvas_Rect,

	Point _fetch_color_Point, Point centered_SrtPoint,
	Point centered_SrtPoint_canvas,
	Point centered_EndPoint_canvas,
	int _adjusted_w_size, int _adjusted_h_size,
	int astroke_depth, int _try_,
	int color_BGR_B, int color_BGR_G, int color_BGR_R,
	int _x_src_step1,
	int _x_canvas_step1,

	unsigned char * _src_ptr,
	unsigned char *_accu_canvas_ptr,
	unsigned char * _ing_canvas_ptr,
	int _mode,
	int _tbrush_reset,
	int ing_pmap_flag,
	Mat & _ing_pmap_canvas
) {
	static int tbrush_cnt = 0;
	static int saved_depth = -1;
	if (_tbrush_reset == 1) tbrush_cnt = 0;
	if (saved_depth != astroke_depth)
	{
		saved_depth = astroke_depth;

	}

	cv::Mat *bestBrush_8UC1_resized;

	//	cv::Mat bestBrush_embossed_resized;//R

	int Alpha;

	unsigned char * ing_ROI_canvas_data = _ing_canvas_ROI.data;
	unsigned char * ing_pmap_canvas_data = _ing_pmap_canvas.data;
	unsigned char * src_canvas_ROI_data = _src_canvas_ROI.data;

	unsigned char *before_ROI_canvas_clone_data = _before_ROI_canvas_clone.data;
	unsigned char * changed_canvas_ROI_clone_data = _changed_canvas_ROI_clone.data;
	unsigned char * changed_canvas_ROI_data = _changed_canvas_ROI.data;

	//int canvas_ROI_clone_width = _before_ROI_canvas_clone.size().width;
	//int canvas_ROI_clone_height = _before_ROI_canvas_clone.size().height;
	//int canvas_ROI_clone_step1 = (int)_before_ROI_canvas_clone.step1();
	//int canvas_ROI_clone_channels = (int)_before_ROI_canvas_clone.channels();

	int canvas_ROI_width = _changed_canvas_ROI.size().width;
	int canvas_ROI_height = _changed_canvas_ROI.size().height;
	int canvas_ROI_step1 = (int)_changed_canvas_ROI.step1();
	int canvas_ROI_channels = (int)_changed_canvas_ROI.channels();

	int canvas_clone_ROI_step1 = (int)_changed_canvas_ROI_clone.step1();
	int canvas_clone_ROI_channels = (int)_changed_canvas_ROI_clone.channels();

	Mat src_ROI_canvas_clone_resized_8UC1;
	Mat a;
	int s_w, s_h;
	//if (canvas_ROI_width > g_brush_thumbnail_size)
	s_w = canvas_ROI_width;
	//else
	//s_w = g_brush_thumbnail_size;
	//if (canvas_ROI_height > g_brush_thumbnail_size)
	s_h = canvas_ROI_height;
	//else
	//s_h = g_brush_thumbnail_size;

	resize(_src_canvas_ROI, a, cv::Size(s_w, s_h));
	cv::cvtColor(a, src_ROI_canvas_clone_resized_8UC1, COLOR_RGB2GRAY);
	int src_resized_8UC1_step1 = (int)src_ROI_canvas_clone_resized_8UC1.step1();

	string f_name = "p" + to_string(astroke_depth) + "/" + m_t_ + to_string(astroke_depth) + "_t_" + to_string(_try_);
	string br_tag = "/br/" + f_name;

	if (_mode == 1) {
		if (tbrush_cnt < DEBUG_BRUSH_CNT) {
		///	debug_image(f_name + "_0src_ROI_ ", _src_canvas_ROI);
		//	debug_image(f_name + "_1RsizROI_ ", src_ROI_canvas_clone_resized_8UC1);
		//	debug_image(f_name + "_2bef_ROI_ ", _before_ROI_canvas_clone);
		//	debug_image(f_name + "_3chg_ROI_", _changed_canvas_ROI_clone);
		}

	}
	int brush_no;
	unsigned char * bestBrush_8UC1_resized_data;
	render_Brush_pgm *bestBrush_pgm_resized;
	int brush_render_depth;

//	if (render_method == RENDER_ENHANCE)
//		brush_render_depth = enhance_mode_base_depth+astroke_depth;
//	else
//		brush_render_depth = render_depth;

	int b_width;
	
		int b_height;
		int b_step1;
		int b_channels;
#ifdef _USE_PGM
		brush_no = JudgementBrush_pgm_bsize(src_ROI_canvas_clone_resized_8UC1, /*added by cwlee*/astroke_depth,
			s_w, s_h, src_resized_8UC1_step1, br_tag);
		bestBrush_pgm_resized = brush_pgm_resized_array[astroke_depth][brush_no];
		bestBrush_8UC1_resized_data = bestBrush_pgm_resized->brush_8UC1_data;
		b_width = bestBrush_pgm_resized->brush_size;
		b_height = bestBrush_pgm_resized->brush_size;
		b_step1 = (int)(bestBrush_pgm_resized->brush_8UC1.step1());
		b_channels = bestBrush_pgm_resized->brush_8UC1.channels();

	
#else
	brush_no = JudgementBrush(src_ROI_canvas_resized_8UC1, /*added by cwlee*/astroke_depth,
		s_w, s_h, src_resized_8UC1_step1, br_tag);
	render_Brush *brush_pgm_resized = brush_pgm_resized_array[astroke_depth][brush_no];
	bestBrush_8UC1_resized_data = brush_pgm_resized->brush_8UC1_data;
	int b_width = bestBrush_8UC1_resized->size().width;
	int b_height = bestBrush_8UC1_resized->size().height;
	int b_step1 = (int)bestBrush_8UC1_resized->step1();
	int b_channels = bestBrush_8UC1_resized->channels();
#endif

	//	bestBrush_embossed_resized = brush_resized_array[astroke_depth][brush_no]->bump;
	//unsigned char * bestBrush_data_8UC1_resized = (unsigned char *)bestBrush_8UC1_resized->data;
	//unsigned char * bestBrush_embossed_resized_data = bestBrush_embossed_resized.data;

	


	for (int bx = 0; bx < b_width; bx++)
	{

		for (int by = 0; by < b_height; by++)
		{

			//	int canvas_ROI_Index_3c = by *canvas_clone_ROI_step1 + bx * canvas_clone_ROI_channels;
			int canvas_clone_ROI_Index_3c = by *canvas_clone_ROI_step1 + bx * canvas_clone_ROI_channels;
			int gray_bIndex_1c = by*b_step1 + bx*b_channels;
			p_peek_1c(bestBrush_8UC1_resized_data, gray_bIndex_1c, Alpha);
			if (Alpha < g_alpha_TH) {
				p_poke(changed_canvas_ROI_clone_data, canvas_clone_ROI_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
			}

		}

	}


	//#ifdef DEBUG_ALPHA
	if (_mode == 1) {
		if (tbrush_cnt < DEBUG_BRUSH_CNT) {

			//	debug_image("br/" + f_name + "_brN_" + to_string(brush_no), bestBrush_8UC1_resized);
	//		debug_image(f_name + "_6acc_ROI_", _changed_canvas_ROI_clone);
			tbrush_cnt++;
		}
	}
//#endif
	int l_judge_image = 0;

		int  int_result;
	if (l_judge_image == 0) {

		 int_result = JudgementImage(src_canvas_ROI_data, changed_canvas_ROI_clone_data, before_ROI_canvas_clone_data,
			b_width, b_height,
			centered_SrtPoint_canvas,
			//_fetch_color_Point,
			astroke_depth,
			b_width, b_height, b_channels, b_step1,
			canvas_ROI_width, canvas_ROI_height, canvas_ROI_channels, canvas_ROI_step1,
			m_tag,
			canvas_clone_ROI_step1
		);
	}
	else int_result = CHANGED_BETTER;

	int p_step_1c = (int)paint_map_canvas_8UC1[0].step1();

	if (int_result == CHANGED_BETTER || _mode == 1) {

		for (int by = 0; by < b_height; by++)
		{
			for (int bx = 0; bx < b_width; bx++)
			{
			//	int canvas_ROI_clone_Index_3c = by *canvas_ROI_clone_step1 + bx * canvas_ROI_clone_channels;
				int canvas_Index_1c = (centered_SrtPoint_canvas.y+by) *p_step_1c + (centered_SrtPoint_canvas.x+bx) ;
				int canvas_Index_3c = (centered_SrtPoint_canvas.y + by) *canvas_ROI_step1 + (centered_SrtPoint_canvas.x+bx) * canvas_ROI_channels;
				int gray_bIndex_1c = by*b_width + bx;
			//	int bIndex_1c = by *b_step1 + bx * b_channels;

					Alpha = bestBrush_8UC1_resized_data[gray_bIndex_1c];
					if (Alpha < g_alpha_TH) {
						if ( ing_pmap_flag == 1	)
							p_poke(ing_pmap_canvas_data, canvas_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						p_poke(_ing_canvas_ptr, canvas_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						p_poke(_accu_canvas_ptr, canvas_Index_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						p_poke(paint_map_canvas_8UC1_data[astroke_depth], canvas_Index_1c,0);
						for (int d = 0; d < render_depth; d++) {
							p_poke(paint_map_accu_canvas_8UC1_data[d], canvas_Index_1c, 0);
						}
					}
			}//for x
		}//for y
		if (_mode == 1) {
			if (tbrush_cnt < DEBUG_BRUSH_CNT) {

				//debug_image(f_name + "_6ing_ROI_", _changed_ROI_clone);
		//		debug_image(f_name + "_6acc_ing_", _accu_canvas);
		//		debug_image(f_name + "_6ing_ing_", _ing_canvas_ROI);
		//		debug_image(f_name + "_6ing_ROI_", _changed_canvas_ROI);
			}
		}
	} // if changed better
	if (_mode == 1) {
		if (tbrush_cnt < DEBUG_BRUSH_CNT) {
		//	debug_image(f_name + "paint_" + m_t_ + to_string(astroke_depth), paint_map_canvas_8UC1[astroke_depth]);

		}
	}
#ifdef DEBUG_ALPHA
	alpha_channel.release();
	hsv_v.release();
#endif
	return int_result;
}

