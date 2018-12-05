#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"

void  render_::p_peek_canvas_1c(unsigned char * p, int p_x, int p_y, int &p_0) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width);

	p_0 = p[index];

}
int render_::paint_a_stroke(partition_Node* region_p, int layer_more, int _mode, Point _p, int _depth) {
	//partition_Node* region_p = (*region_iter);
	
	Point Region_srtPoint, Region_endPoint;
	Point fetch_color_Point, fetch_color_canvas_point;

	Point centered_SrtPoint, centered_EndPoint;
	Point centered_SrtPoint_canvas, centered_EndPoint_canvas;
	//	int _paint_area_w_size, paint_area_h_size;

	int brush_area_w_size, brush_area_h_size;
	int color_BGR_B, color_BGR_G, color_BGR_R;

	int adjusted_h_size;
	int adjusted_w_size;

	Size Brush_size;
	int size_mismatch = 0;
	int saved_depth = -1;
	
	Size Region_size;
	int Region_w_size, Region_h_size;

	int astroke_depth;
	Point Strk_point_canvas;
	//int times;

	Mat before_canvas_ROI;
	Mat before_canvas_ROI_clone;

	Mat changed_canvas_ROI;
	Mat changed_canvas_ROI_clone;

	Mat ing_ROI_canvas;
	Mat mat_accu;

	Mat mat_ing;
	cv::Mat centered_src_canvas_ROI;
	int brush_area_h_size_half;
		int brush_area_w_size_half;
	static int layer_more_depth=-1; // check first time layer
	static int check_depth = -1;//check last depth

	random_device rand_x[MAX_DEPTH];
	random_device rand_y[MAX_DEPTH];
	astroke_depth = region_p->depth;
	render_stroke_no = (region_p)->no;
	if (layer_more == 1) {
		
		if (layer_more_depth == -1) {//first time
			layer_more_depth = astroke_depth;
			check_depth = astroke_depth;
			x_tbrush_cnt = 0;
		}
		
	}
	else {

		if (layer_more_depth != -1)
		if (layer_more_depth != astroke_depth)
			layer_more_depth = 1000 + astroke_depth;
	}
	Region_srtPoint = (region_p)->srtPoint;
	Region_endPoint = (region_p)->endPoint;
	Region_w_size = Region_endPoint.x - Region_srtPoint.x;
	Region_h_size = Region_endPoint.y - Region_srtPoint.y;
	
	//if (render_method == RENDER_ENHANCE) {
		//Region_h_size += 8;
	//	Region_w_size += 8;
//	}
	Region_size = Size(Region_w_size, Region_h_size);

	render_stroke_no = (region_p)->no;

	
	int times;
	brush_area_h_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
	brush_area_w_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth

	Brush_size = Size(brush_area_w_size, brush_area_h_size);

	 brush_area_h_size_half = brush_area_h_size / 2;
	 brush_area_w_size_half = brush_area_w_size / 2 ;


	 if (brush_area_w_size_half > x_canvas_bezel_size)
		 adjusted_w_size = x_canvas_bezel_size;
	 else
		 adjusted_w_size = brush_area_w_size_half;

	 if (brush_area_h_size_half > x_canvas_bezel_size)
		 adjusted_h_size = x_canvas_bezel_size;
	 else
		 adjusted_h_size = brush_area_h_size_half;

	times = ((Region_w_size * Region_h_size) /
		(adjusted_h_size * adjusted_w_size));

	if (times == 0) 
		times = 1;

	x_strk_times[astroke_depth] = times;
	//	if (layer_more == 0)
	//		x_paint_area_brush_count = times*g_paint_try_scale[astroke_depth]*2;
	//else
	x_paint_area_brush_count = times*g_paint_try_scale[astroke_depth];
	
	uniform_int_distribution<int> diStrk_x(0,Region_w_size);
	uniform_int_distribution<int> diStrk_y(0, Region_h_size);


	grid_try_sum[astroke_depth] += x_paint_area_brush_count;//
	int extended_paint_area_brush_count = x_paint_area_brush_count;
	int extend_retry = 0;
	 mat_accu = accu_canvas[astroke_depth];
	 mat_ing = ing_canvas[astroke_depth];

	 string f_name_btdt = "p" + to_string(astroke_depth) + "/" + m_t_ + to_string(astroke_depth) + "_t_" + to_string(0);
	// debug_image(f_name_btdt + "__ACC_ACC_" + to_string(layer_more) + "_", accu_canvas[astroke_depth]);

	int extended_paint_area_brush_count_limit = extended_paint_area_brush_count * 3;
	
	for (render_try = 0; render_try < extended_paint_area_brush_count; render_try++)
	{
		
			//r_cout << "z" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width <<" "<<render_try<< endl;
		
		
		bool modified = true;
		//	int retry_cnt = 1;

		random_x = diStrk_x(rand_x[astroke_depth]);
		random_y = diStrk_y(rand_y[astroke_depth]);
		fetch_color_Point.x = Region_srtPoint.x + random_x;
		fetch_color_Point.y = Region_srtPoint.y + random_y;

		if (layer_more == layer_more_1) {

			int depth_value;
			render_::p_peek_canvas_1c(depth_map_canvas_8UC1_data, fetch_color_Point.x, fetch_color_Point.y, depth_value);
			if (depth_value != 255 && (depth_value >= (x_changed_depth))) {
				extended_paint_area_brush_count++;
				extend_retry++;
				
				if (extended_paint_area_brush_count < extended_paint_area_brush_count_limit )
					continue;
				else {
					layer_more = 0;
					check_depth = 1000;
				}
			}
		}

		int fetch_Index = (fetch_color_Point.x + (fetch_color_Point.y*x_src_image_size.width))*(x_src_channels);
		int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*x_src_image_size.width);

		int fetch_canvas_Index = ((fetch_color_Point.x + x_canvas_bezel_size)
			+ (fetch_color_Point.y + x_canvas_bezel_size)*(x_canvas_size_width) )*x_src_channels;

		p_peek(x_src_ptr, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);
		if (layer_more == layer_more_1 || check_depth == astroke_depth) {
				p_poke(x_retry_map_1c_data, fetch_Index_1c, 0);
		}
		p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);
		//r_cout << "h" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << " " << render_try << endl;
		
		

		centered_SrtPoint.x = fetch_color_Point.x - adjusted_w_size;
		centered_SrtPoint.y = fetch_color_Point.y - adjusted_h_size;

		centered_EndPoint.x = fetch_color_Point.x + (brush_area_h_size-adjusted_w_size);// to overcome odd brush size
		centered_EndPoint.y = fetch_color_Point.y + (brush_area_w_size-adjusted_h_size);// to overcome odd brush size

		centered_SrtPoint_canvas.x = centered_SrtPoint.x + x_canvas_bezel_size;
		centered_SrtPoint_canvas.y = centered_SrtPoint.y + x_canvas_bezel_size;
		centered_EndPoint_canvas.x = centered_EndPoint.x + x_canvas_bezel_size;
		centered_EndPoint_canvas.y = centered_EndPoint.y + x_canvas_bezel_size;

		Rect centered_ROI_canvas_Rect = Rect(centered_SrtPoint_canvas, centered_EndPoint_canvas);

#ifdef _DEBUG_RENDER
		rectangle(painting_area_canvas[astroke_depth], centered_ROI_canvas_Rect, Scalar(0, 0, 0));//BLACK
#endif
		centered_src_canvas_ROI = x_src_canvas(centered_ROI_canvas_Rect);
		//	src_canvas_ROI_clone = src_canvas(centered_ROI_canvas_Rect).clone();


		//before_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
		before_canvas_ROI_clone.release();
		before_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

		changed_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
		changed_canvas_ROI_clone.release();
		changed_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

		ing_ROI_canvas = mat_ing(centered_ROI_canvas_Rect);

		//Mat ing_ROI_clone = ing_canvas[astroke_depth](centered_ROI_canvas_Rect).clone();


		int result = P_Rendering(
			centered_src_canvas_ROI,
			before_canvas_ROI_clone,
			changed_canvas_ROI_clone,
			ing_ROI_canvas,
			changed_canvas_ROI,
			mat_accu,
			centered_ROI_canvas_Rect,
			fetch_color_Point,
			centered_SrtPoint,
			centered_SrtPoint_canvas,
			centered_EndPoint_canvas,
			adjusted_w_size, adjusted_h_size,

			astroke_depth, render_try,
			color_BGR_B, color_BGR_G, color_BGR_R,

			x_src_step1,
			x_canvas_step1,
			x_src_ptr,
			accu_canvas_data[astroke_depth],
			ing_canvas_data[astroke_depth],
			1,//debug_image on
			0
		);
		//r_cout << "i" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << " " << render_try << endl;
		//	current_fetched_map_data,fetched_color_data,r,g,b);

		if (result == BEFORE_BETTER) {//result==0
									  //	beforeImg_canvas.copyTo(accu_canvas[astroke_depth]);
									  //	accu_canvas_data[astroke_depth] = (unsigned char*)accu_canvas[astroke_depth].data;
									  //	Nothing to do
		}
		else {//result==1/CHANGED_BETTER, 
			  //changed_canvas_ROI.copyTo(before_canvas_ROI);
			  //	ing_canvas[astroke_depth](centered_ROI_canvas_Rect) = ing_ROI_canvas;
			r_s_changed_count[astroke_depth]++;
			//accu_canvas[astroke_depth](centered_ROI_canvas_Rect)=changed_canvas_ROI;


#ifdef _DEBUG_RENDER
			rectangle(changed_map_canvas[astroke_depth], canvas_centered_rect, Scalar(0, 0, 255));//RED
#endif					
		}
		if (x_tbrush_cnt < DEBUG_BRUSH_CNT) {
			string f_name_tdt = "p" + to_string(astroke_depth) + "/" + m_t_ + to_string(astroke_depth) + "_t_" + to_string(render_try);
		//	debug_image(f_name_tdt + "_4ING_ING_" + to_string(layer_more) + "_", ing_canvas[astroke_depth]);
			//
			//debug_image(f_name_tdt + "_5ACC_ACC_" + to_string(layer_more) + "_", accu_canvas[astroke_depth]);
		}
		x_tbrush_cnt++;
	}
	//end of retry
	extended_try[astroke_depth] += extended_paint_area_brush_count;
	

	if (layer_more_depth > 999 ) {
		r_cout << layer_more_depth - 1000 << " " << render_stroke_no << " extend_retry = " <<
			extended_paint_area_brush_count << " : " << extend_retry <<
		" limit " << extended_paint_area_brush_count_limit << endl;
	debug_image("ing/__retry_map_" + tag_[render_method] +  to_string(layer_more_depth - 1000), x_retry_map_1c);
	layer_more_depth = -1;
}
	before_canvas_ROI.release();
	before_canvas_ROI_clone.release();

	changed_canvas_ROI.release();
	changed_canvas_ROI_clone.release();

	ing_ROI_canvas.release();

	centered_src_canvas_ROI.release();

	return times;
}