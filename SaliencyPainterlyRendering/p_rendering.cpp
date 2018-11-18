//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
#include "stdafx.h"

#include "p_rendering.h"
#include "util.h"
#include "render_.h"
#include "rgb2hsv.h"
#include "time_stamp.h"

/*

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
	int astroke_depth, int painting_try, 
	int color_BGR_B, int color_BGR_G, int color_BGR_R//BGR order
	//unsigned char * _ing_canvas_data

	)
*/
	int render_::P_Rendering(Mat & _src_ROI_canvas, Mat & _before_ROI_canvas, cv::Mat & _changed_ROI_clone, 
		cv::Mat & _ing_ROI, Point _fetch_color_Point, Point centered_SrtPoint, Point canvas_centered_SrtPoint,
		Point canvas_centered_EndPoint, int brush_area_w_size, int brush_area_h_size, 
		int astroke_depth, int painting_try, int color_BGR_B, int color_BGR_G, int color_BGR_R,
		int _depth,int _try_)


{

	//cv::Mat bestBrush_src;
	//cv::Mat bestBrush_resized;
	//cv::Mat bestBrush_gray_src;
	cv::Mat bestBrush_gray_resized;
	//cv::Mat bestBrush_embossed_src;
	cv::Mat bestBrush_embossed_resized;//R
	//cv::Mat paint_by_brush(brush_area_h_size,brush_area_w_size,CV_8UC3);
	//int br_BGR_B, br_BGR_G, br_BGR_R;
	//Point point;
	int Alpha;
#ifdef DEBUG_ALPHA
	Scalar s_rgb, s_hsv;
	Mat alpha_channel;
	Mat hsv_v;
#endif
	unsigned char * ing_ROI_clone_q_data = _ing_ROI.data;

	unsigned char * src_ROI_canvas_data = _src_ROI_canvas.data;
	
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
	Mat testImg_resized_8UC1;
	cv::cvtColor(testImg_src, testImg_resized_8UC1, COLOR_RGB2GRAY);
	resize(testImg_resized_8UC1, testImg_resized_8UC1, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));
	//if (testImg_resized_8UC1.channels() == 3)
		

	int brush_no = JudgementBrush(testImg_resized_8UC1, /*added by cwlee*/astroke_depth,
		g_brush_thumbnail_size, g_brush_thumbnail_size, g_brush_set);

	bestBrush_gray_resized= brush_resized_set[astroke_depth].at(brush_no)->brush_gray;
	bestBrush_embossed_resized = brush_resized_set[astroke_depth].at(brush_no)->bump;
	unsigned char * bestBrush_data_gray_resized = (unsigned char *)bestBrush_gray_resized.data;
	unsigned char * bestBrush_embossed_resized_data = bestBrush_embossed_resized.data;


	static int tbrush_cnt = 0;
	//bestBrush_resized.create(brush_area_h_size, brush_area_w_size, CV_8UC3);
	//bestBrush_resized.setTo(255);
	//unsigned char * bestBrush_data_resized = (unsigned char *)bestBrush_resized.data;
	int b_width = brush_area_w_size;
	int b_height = brush_area_h_size;
	int b_step1 = (int)bestBrush_gray_resized.step1();
	int b_channels = bestBrush_gray_resized.channels();

//#ifdef DEBUG_BRUSH
	 if (tbrush_cnt < 20) {
			 debug_image("br/IP0_brush_gray_resized_" + m_tag_ + to_string(_try_)+"_"+to_string(brush_no), bestBrush_gray_resized);
			debug_image("p"+to_string(_depth)+"/changed_ROI_" + m_tag_ + to_string(_try_), _changed_ROI_clone);
			debug_image("p" + to_string(_depth) + "/src_ROI_" + m_tag_ + to_string(_try_), _src_ROI_canvas);

			//debug_image("br/IP0_brush_embossed_resized_" + m_tag + to_string(brush_no), tbrush_cnt, bestBrush_embossed_resized);
	 }

//#endif

	for (int by = 0; by < brush_area_h_size; by++)
	{
		for (int bx = 0; bx < brush_area_w_size; bx++)
		{
			int bIndex_3c = by *b_step1 + bx * b_channels;
			int gray_bIndex_1c = by*brush_area_w_size + bx;


				Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
				if (Alpha < g_alpha_TH) {
					//paint_by_brush_ptr[bIndex_3c + _BGR_B] = color_BGR_B;
				//	paint_by_brush_ptr[bIndex_3c + _BGR_G] = color_BGR_G;
					//paint_by_brush_ptr[bIndex_3c + _BGR_R] = color_BGR_R;
					//br_BGR_R = paint_by_brush[bIndex_3c + _BGR_R];
					//br_BGR_G = paint_by_brush[bIndex_3c + _BGR_G];
					//br_BGR_B = paint_by_brush[bIndex_3c + _BGR_B];

					p_poke(changed_ROI_clone_data, bIndex_3c, color_BGR_B, color_BGR_G, color_BGR_R);
				}
				
		}
				
	}


//#ifdef DEBUG_ALPHA
	if (tbrush_cnt < 20) {
		debug_image("p"+to_string(_depth)+"/before_ROI_" + m_tag_ +to_string(_try_),  _before_ROI_canvas);
		debug_image("p"+to_string(_depth)+"/changed_ROI_" + m_tag_+to_string(_try_), _changed_ROI_clone);
		tbrush_cnt++;
	}
//#endif
	
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

				int gray_bIndex_1c = by*brush_area_w_size + bx;
				int bIndex_3c = by *b_step1 + bx * b_channels;

					Alpha = bestBrush_data_gray_resized[gray_bIndex_1c];
					if (Alpha < g_alpha_TH) {
						//	br_BGR_R = pabestBrush_data_resized[bIndex_3c + _BGR_R];
						//	br_BGR_G = bestBrush_data_resized[bIndex_3c + _BGR_G];
						//	br_BGR_B = bestBrush_data_resized[bIndex_3c + _BGR_B];
						p_poke(ing_ROI_clone_q_data, bIndex_3c, color_BGR_B, color_BGR_G, color_BGR_R);
						//					p_poke(paint_map_data, bIndex_3c, astroke_depth, astroke_depth, astroke_depth);
					}
			}//for x
		}//for y
	} // if changed better


#ifdef DEBUG_ALPHA
	alpha_channel.release();
	hsv_v.release();
#endif
	return int_result;
}

