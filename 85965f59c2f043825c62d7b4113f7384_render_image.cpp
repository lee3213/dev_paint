#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"

int render_::func_render_a_pixel(int x, int y, int astroke_depth) {
	Point centered_SrtPoint, centered_EndPoint;
	Point centered_SrtPoint_canvas, centered_EndPoint_canvas;
	int brush_area_h_size_half;
	int brush_area_w_size_half;
	int adjusted_h_size;
	int adjusted_w_size;
	Point fetch_color_Point, fetch_color_canvas_point;
	int color_BGR_B, color_BGR_G, color_BGR_R;
	//	int times;
	int brush_area_w_size, brush_area_h_size;
	Size Brush_size;
	Mat before_canvas_ROI;
	Mat before_canvas_ROI_clone;

	Mat changed_canvas_ROI;
	Mat changed_canvas_ROI_clone;

	Mat ing_ROI_canvas;

	cv::Mat src_canvas_ROI;
	Mat mat_accu;

	Mat mat_ing;
	brush_area_h_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
	brush_area_w_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
	Brush_size = Size(brush_area_w_size, brush_area_h_size);

	brush_area_h_size_half = brush_area_h_size / 2 + brush_area_h_size % 2;
	brush_area_w_size_half = brush_area_w_size / 2 + brush_area_w_size % 2;

	fetch_color_Point.x = x - x_canvas_bezel_size;
	fetch_color_Point.y = y - x_canvas_bezel_size;
	int fetch_Index = (fetch_color_Point.x + (fetch_color_Point.y*x_src_image_size.width))*(x_image_channels);
	int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*x_src_image_size.width);

	int fetch_canvas_Index = ((fetch_color_Point.x + x_canvas_bezel_size)
		+ (fetch_color_Point.y + x_canvas_bezel_size)*(x_canvas_size_width))*x_image_channels;

	p_peek(x_src_ptr, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

	p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);
	if (brush_area_w_size_half > x_canvas_bezel_size)
		adjusted_w_size = x_canvas_bezel_size;
	else
		adjusted_w_size = brush_area_w_size_half;

	if (brush_area_h_size_half > x_canvas_bezel_size)
		adjusted_h_size = x_canvas_bezel_size;
	else
		adjusted_h_size = brush_area_w_size_half;

	centered_SrtPoint.x = fetch_color_Point.x - adjusted_w_size;
	centered_SrtPoint.y = fetch_color_Point.y - adjusted_h_size;

	centered_EndPoint.x = fetch_color_Point.x + (adjusted_w_size);// to overcome odd brush size
	centered_EndPoint.y = fetch_color_Point.y + (adjusted_h_size);// to overcome odd brush size

	centered_SrtPoint_canvas.x = centered_SrtPoint.x + x_canvas_bezel_size;
	centered_SrtPoint_canvas.y = centered_SrtPoint.y + x_canvas_bezel_size;
	centered_EndPoint_canvas.x = centered_EndPoint.x + x_canvas_bezel_size;
	centered_EndPoint_canvas.y = centered_EndPoint.y + x_canvas_bezel_size;

	Rect centered_ROI_canvas_Rect = Rect(centered_SrtPoint_canvas, centered_EndPoint_canvas);
	src_canvas_ROI = src_canvas(centered_ROI_canvas_Rect);
	//	src_canvas_ROI_clone = src_canvas(centered_ROI_canvas_Rect).clone();

	mat_accu = accu_canvas[astroke_depth];
	mat_ing = ing_canvas[astroke_depth];
	//before_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
	before_canvas_ROI_clone.release();
	before_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

	changed_canvas_ROI = mat_accu(centered_ROI_canvas_Rect);
	changed_canvas_ROI_clone.release();
	changed_canvas_ROI_clone = mat_accu(centered_ROI_canvas_Rect).clone();

	ing_ROI_canvas = mat_ing(centered_ROI_canvas_Rect);
	int result = P_Rendering(
		src_canvas_ROI,
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

		x_image_step1,
		x_canvas_step1,
		x_src_ptr,
		accu_canvas_data[astroke_depth],
		ing_canvas_data[astroke_depth],
		1,
		0
	);
	if (result == CHANGED_BETTER)
		r_s_changed_count[astroke_depth]++;

	before_canvas_ROI.release();
	before_canvas_ROI_clone.release();

	changed_canvas_ROI.release();
	changed_canvas_ROI_clone.release();

	ing_ROI_canvas.release();

	src_canvas_ROI.release();
	return result;
}
				
int  render_::pad_p_map(Mat & a_pmap_canvas_8UC1,string tag, Rect x_src_canvas_Rect_full,int _depth)
{
	//int times;
	//partition_Node* strk_p;
	int x, y, astroke_depth=_depth;
	int _x_width;
	int _y_height;
	int sum_x;
	int sum_y;
	int c;
	int ret;
	//_x_width = x_canvas_size_width / 10;
	//_y_height = x_canvas_size_height / 10;
	//int x_modulo = x_canvas_size_width - (_x_width * 10);
	//int y_modulo = x_canvas_size_height - (_y_height * 10);
		Mat nonZeroCoordinates;
		Mat nonzero_ROI;
		Rect r_, r_canvas;
	
		unsigned char * pp=paint_map_accu_canvas_8UC1[astroke_depth].data;
		int tot_cnt = 0;
		int effective_try = 0;
		int __saved_depth;
		int saved_c;
		int erased = 0;
		Mat accu;
		for(int layer=_depth;layer<(render_depth);layer++){
			int ssize = (int)render_dropped_set[layer].Region_list.size();
			//r_cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aRegion_set[i].size() << endl;
			if (ssize == 0)
				continue;
			accu = accu_canvas[_depth].clone();
			for (list<partition_Node*>::iterator partition_it = render_dropped_set[layer].Region_list.begin();
				partition_it != render_dropped_set[layer].Region_list.end(); partition_it++)
			{
				__saved_depth = (*partition_it)->depth;

				//cv::rectangle(render_::overlay_dropped_map[__saved_depth], Rect((*partition_it)->srtPoint,
			//		(*partition_it)->endPoint), RGB(0, 0, 0));
			  r_= Rect((*partition_it)->srtPoint,(*partition_it)->endPoint);
			  r_canvas = r_;
			  r_canvas.x = r_.x + x_canvas_bezel_size;
			  r_canvas.y = r_.y + x_canvas_bezel_size;
			  rectangle(accu, r_canvas, 0);
				nonzero_ROI = a_pmap_canvas_8UC1(r_);
				
				findNonZero(nonzero_ROI, nonZeroCoordinates);
				c = (int)nonZeroCoordinates.total();
				tot_cnt += c;
				sum_x = 0; sum_y = 0;
				saved_c = c;
				if (c > x_skip) {
					do {

						x = nonZeroCoordinates.at<Point>(0).x + r_.x;
						y = nonZeroCoordinates.at<Point>(0).y + r_.y;
						rectangle(overlay_dropped_map[layer], Rect(x, y, x + 2, x + 2), 0);
						int index = x_canvas_size_width*y + x;
						int val;
						p_peek_1c(pp, index, val);
						if (val != 0) {
							ret = func_render_a_pixel(x, y, astroke_depth);
							effective_try++;
							p_peek_1c(pp, index, val);
							if (val != 0) {
								val = 0;
								p_poke_1c(pp, index, val);

							}
						}
						nonzero_ROI = a_pmap_canvas_8UC1(r_);
						findNonZero(nonzero_ROI, nonZeroCoordinates);
						c = (int)nonZeroCoordinates.total();
					
						
					} while (c > x_skip);
					erased += saved_c - c;
				}//if c > skip
				}// list
				debug_image("pmap/__paint_map_a5_" + tag_[render_method] + to_string(layer) + "_" + to_string(0) + "_" + to_string(layer), paint_map_accu_canvas_8UC1[layer]);
				debug_image("pmap/_accu_" +m_t+"_"+ to_string(layer), accu);
				debug_image("pmap/_fd" + m_t, layer, overlay_dropped_map[layer]);
			}//layer
		debug_image("pmap/_ac_" + to_string(_depth) + m_t , accu_canvas[_depth]);
		r_cout << "tot_cnt "  <<tot_cnt <<
			" effective_try "<<effective_try << " saved "<<saved_c<<"  "
			
			<<fixed<<setw(5)<<setprecision(1)<<
			(float)(( (float)saved_c / ( float)tot_cnt )*(float)100.0)<< endl;
	//times = paint_a_stroke(strk_p, 1, 1, Point(x, y), astroke_depth);
		return tot_cnt;
}

int render_::pmap_count_zero(Mat & a_map_canvas_8UC1,string tag,Rect x_src_canvas_Rect_full) {
//	double minval, maxval;
//	Point minloc, maxloc;
	//int i_minval, i_maxval;
	Mat ROI=a_map_canvas_8UC1(x_src_canvas_Rect_full);

	//cv::minMaxLoc(ROI, &minval, &maxval, &minloc, &maxloc);
	//i_minval = (int)minval;
	//i_maxval = (int)maxval;

	

	//int diff = i_maxval - i_minval;
	//if (diff == 0) diff = 1;
	//int step = (int)256 / diff;
//	Mat p_map;
//	a_map_canvas_8UC1.convertTo(p_map, -1, step);
//	debug_image("ing/" + tag, p_map);

	//r_cout <<tag<< " minLoc " << minloc.x << ", " << minloc.y << " : " << (int)i_minval ;
//	r_cout << " maxLoc " << maxloc.x << ", " << maxloc.y << " : " << (int)i_maxval ;
	//r_cout << " diff = " << diff << ", step = " << step ;


	//Mat paint_ROI = a_map_canvas_8UC1(x_src_canvas_Rect_full);
	int zero_pixel_cnt = (x_src_image_size.height*x_src_image_size.width) - cv::countNonZero(ROI);
	float ratio=(float)((float)zero_pixel_cnt / (float)(x_src_image_size.height*x_src_image_size.width))*(float)100.0;
	r_cout <<tag<< " zero pixel = " << zero_pixel_cnt << " / " << (x_src_image_size.height*x_src_image_size.width) <<
		" = " << std::fixed << setprecision(2) <<ratio << endl;
	return (int)(ratio);
}
void render_::pmap_overlay_fill(int uu_depth, Mat a_map_canvas_8UC1, int color) {
	Point Strk_srtPoint, Strk_endPoint;
	Size Strk_size;
	int Strk_w_size, Strk_h_size;
	Point Strk_point_canvas;
	for (list<partition_Node*>::iterator partition_it = render_region_set[uu_depth].Region_list.begin();
		partition_it != render_region_set[uu_depth].Region_list.end(); partition_it++) {
		partition_Node* strk_p = (*partition_it);
		Strk_srtPoint = (strk_p)->srtPoint;
		Strk_endPoint = (strk_p)->endPoint;
		Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
		Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;
		//	stroke_area[uu_depth] += Strk_w_size*Strk_h_size;
		Strk_size = Size(Strk_w_size, Strk_h_size);
		//astroke_depth = (strk_p)->depth;
		Strk_point_canvas = Point(Strk_srtPoint.x + x_canvas_bezel_size, Strk_srtPoint.y + x_canvas_bezel_size);
		Rect Strk_canvas_ROI_rect(Strk_point_canvas, Strk_size);
		Mat a_map_canvas_8UC1_ROI = a_map_canvas_8UC1(Strk_canvas_ROI_rect);
		//depth_map_canvas_8UC1_ROI.setTo(astroke_depth);//assign a value to Matrix
		a_map_canvas_8UC1_ROI.setTo(color);
	}
}
int   render_::render_image()

{
	Point Strk_srtPoint, Strk_endPoint;
	Size Strk_size;
	int Strk_w_size, Strk_h_size;
	int saved_depth = -1;
	Point Strk_point_canvas;
	//int x_image_width = g_src_image_width;
//	int x_image_height = g_src_image_height;
	int x_image_channels = g_src_image_channels;
	int x_image_step1 = (int)g_src_image_step1;

	int astroke_depth = -1;
	Rect x_src_canvas_Rect_full;

	//Mat beforeImg_canvas;
#ifdef _DEBUG_RENDER
	cv::Mat painting_area_canvas[MAX_DEPTH];
	cv::Mat changed_map_canvas[MAX_DEPTH];
#endif
	int times;
	int times_0=0;
	int layer_0 = 0;
	partition_Node* strk_p;
	src_canvas.create(x_canvas_size_height, x_canvas_size_width, CV_8UC3);
	src_canvas.setTo(255);
	
	
	
	 x_src_canvas_Rect_full.width= x_src_image_size.width; 
	 x_src_canvas_Rect_full.height = x_src_image_size.height;
	 x_src_canvas_Rect_full.x =x_canvas_bezel_size;
	 x_src_canvas_Rect_full.y =x_canvas_bezel_size;
		
	 
	Mat src_ROI_full = src_canvas(x_src_canvas_Rect_full);
	 x_srcImg_.copyTo(src_ROI_full);

	//debug_image("src_canvas", src_canvas);
	//debug_image("src_canvas_ROI", src_ROI_canvas_full);

	accu_canvas[0].create(x_canvas_size_height, x_canvas_size_width, CV_8UC3);
	accu_canvas[0].setTo(255);
	accu_canvas_data[0] = (unsigned char*)accu_canvas[0].data;
	//accu_canvas[n]=accu_canvas[n-1]; later

	for (int i = 0; i < MAX_DEPTH; i++) {
		r_try_map_1c[i].create(x_src_image_size.height, x_src_image_size.width, CV_8UC1);
		r_try_map_1c[i].setTo(255);
		r_try_map_1c_data[i] = r_try_map_1c[i].data;

		grid_try_sum[i] = 0;
		r_s_changed_count[i] = 0;

		ing_canvas[i].create(x_canvas_size_height, x_canvas_size_width, CV_8UC3);
		ing_canvas_data[i] = (unsigned char*)ing_canvas[i].data;
		ing_canvas[i].setTo(255);

		paint_map_canvas_8UC1[i].create(x_canvas_size_height, x_canvas_size_width, CV_8UC1);
		paint_map_canvas_8UC1_data[i] = paint_map_canvas_8UC1[i].data;
		paint_map_canvas_8UC1[i].setTo(0);

		paint_map_accu_canvas_8UC1[i].create(x_canvas_size_height, x_canvas_size_width, CV_8UC1);
		paint_map_accu_canvas_8UC1_data[i] = paint_map_accu_canvas_8UC1[i].data;
		paint_map_accu_canvas_8UC1[i].setTo(0);

		extended_try[i] = 0;
#ifdef _DEBUG_RENDER
		painting_area_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		painting_area_canvas[i].setTo(255);

		changed_map_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		changed_map_canvas[i].setTo(255);

		painting_area_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		painting_area_canvas[i].setTo(255);
		rectangle_canvas(painting_area_canvas[i], Rect(Strk_srtPoint, Strk_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
	}

	depth_map_canvas_8UC1.create(x_canvas_size_height, x_canvas_size_width, CV_8UC1);
	depth_map_canvas_8UC1_data = depth_map_canvas_8UC1.data;
	depth_map_canvas_8UC1.setTo(255);
	//int filayer = -1;
	//for(int i=0;i<render_depth;i++)
	//	r_cout << m_tag << " : "<<render_depth<<" size: "<< render_region_set[i].Region_list.size()<<endl;
//	for (int i = 0; i < render_depth; i++) {
//		r_cout << "0 "<<i << ", " << render_brush_size[i] << endl;
//	}

//	saved_depth = -1;

	int stroke_area[MAX_DEPTH];
	int stroke_area_ratio[MAX_DEPTH];


	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {

		
		stroke_area[uu_depth] = 0;
		stroke_area_ratio[uu_depth] = 0;
		for (list<partition_Node*>::iterator partition_it = render_region_set[uu_depth].Region_list.begin();
			partition_it != render_region_set[uu_depth].Region_list.end(); partition_it++) {
			partition_Node* strk_p = (*partition_it);
			Strk_srtPoint = (strk_p)->srtPoint;
			Strk_endPoint = (strk_p)->endPoint;
			Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
			Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;
			stroke_area[uu_depth] += Strk_w_size*Strk_h_size;
			Strk_size = Size(Strk_w_size, Strk_h_size);
			astroke_depth = (strk_p)->depth;
			Strk_point_canvas = Point(Strk_srtPoint.x + x_canvas_bezel_size, Strk_srtPoint.y + x_canvas_bezel_size);
			Rect Strk_canvas_ROI_rect(Strk_point_canvas, Strk_size);
			Mat depth_map_canvas_8UC1_ROI = depth_map_canvas_8UC1(Strk_canvas_ROI_rect);
			depth_map_canvas_8UC1_ROI.setTo(astroke_depth);//assign a value to Matrix
		}
			stroke_area_ratio[uu_depth] = (int)((float)(stroke_area[uu_depth]) / (float)(stroke_area[0])*100.0);
			r_cout << setw(3) << uu_depth << " : " << setw(7) << render_region_set[uu_depth].Region_list.size() << " : area : " <<
				setw(7) << stroke_area[uu_depth] << " : " <<
				fixed << setw(7) << setprecision(1) << (float)(stroke_area[uu_depth]) / (float)(stroke_area[0])*100.0 <<
				setw(7) << " : " << stroke_area_ratio[uu_depth]<<endl;
	//		r_cout << "a" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
	}
//	r_cout << "b" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
	x_changed_depth = -1;

	
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		pmap_overlay_fill(0, paint_map_canvas_8UC1[uu_depth], 255);
		pmap_overlay_fill(0, paint_map_accu_canvas_8UC1[uu_depth], 255);

		//debug_image("ing/__paint_map_1_" + tag_[render_method] + to_string(uu_depth), paint_map_canvas_8UC1[0]);
		if (stroke_area_ratio[uu_depth] < 85 && x_changed_depth < 0)
		{
			x_changed_depth = uu_depth;
			r_cout << "changed depth = " << x_changed_depth << endl;
				
		}
	}

//	r_cout << "c" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;

	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		
		for (list<partition_Node*>::iterator it_partition_it = render_region_set[uu_depth].Region_list.begin();
				it_partition_it != render_region_set[uu_depth].Region_list.end(); it_partition_it++)
		{
		//	r_cout << "e" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
		 strk_p = (*it_partition_it);
			Strk_srtPoint= (strk_p)->srtPoint;
			Strk_endPoint = (strk_p)->endPoint;
			Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
			Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;
		//	r_cout << "f" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
			Strk_size=Size(Strk_w_size, Strk_h_size);
			
			render_stroke_no=(strk_p)->no;
			astroke_depth = (strk_p)->depth;
			if (astroke_depth > MAX_DEPTH) {
				cerr << "astroke depth >" << MAX_DEPTH << " : " << (strk_p)->depth << endl;
				r_cout << "astroke depth >" << MAX_DEPTH << " : " << (strk_p)->depth << endl;
				continue;
			}
			if (render_brush_size[astroke_depth] < 0) {
				cerr << "m_brush[" << astroke_depth << "]" << render_brush_size[astroke_depth] << endl;
				r_cout << "m_brush[" << astroke_depth << "]" << render_brush_size[astroke_depth] << endl;
				
				for (int i = 0; i < render_depth; i++) {
					r_cout << "1 " << i << ", " << render_brush_size[i] << endl;
					cerr << i << ", " << render_brush_size[i] << endl;
				}
				continue;
			}
		//	r_cout << "g" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
			if (astroke_depth != saved_depth) {
				if (astroke_depth > 0) {
			//		r_cout << "d" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
					accu_canvas[astroke_depth] = accu_canvas[astroke_depth - 1].clone();
			//		r_cout << "y" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
				}
				accu_canvas_data[astroke_depth] = (unsigned char*)accu_canvas[astroke_depth].data;
				saved_depth = astroke_depth;
				x_tbrush_cnt = 0;
			}

			
			times=paint_a_stroke(strk_p,1,0,Point(0,0),astroke_depth);
			if (astroke_depth == (x_changed_depth-2)) {
				times_0 += paint_a_stroke(strk_p,0,0, Point(0, 0), astroke_depth);
				times_0 += paint_a_stroke(strk_p, 0,0, Point(0, 0), astroke_depth);
				times_0 += paint_a_stroke(strk_p, 0,0, Point(0, 0), astroke_depth);
			}
			
#ifdef _DEBUG_RENDER
			rectangle_canvas(painting_area_canvas[astroke_depth], Rect(Strk_srtPoint, Strk_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
		
		
		}//end of it astroke
		string tag = "p_map_" + m_tag_ + to_string(uu_depth);
		pmap_count_zero(paint_map_canvas_8UC1[uu_depth], tag, x_src_canvas_Rect_full);

		debug_image("ing/__paint_map_a_" + tag_[render_method] + to_string(uu_depth), paint_map_accu_canvas_8UC1[uu_depth]);
		debug_image("ing/__paint_map_i_" + tag_[render_method] + to_string(uu_depth), paint_map_canvas_8UC1[uu_depth]);
				//paint_map_canvas_8UC1[uu_depth].copyTo(paint_map_accu_canvas_8UC1[uu_depth]);
		int ret;
		int tot_cnt;
		if (uu_depth == (x_changed_depth-1)) {//apply paint map 
			pmap_overlay_fill(x_changed_depth, paint_map_accu_canvas_8UC1[uu_depth], 0);
			ret = pmap_count_zero(paint_map_canvas_8UC1[uu_depth], tag, x_src_canvas_Rect_full);
			debug_image("ing/__paint_map_a4_" + tag_[render_method] + to_string(uu_depth) + "_" + to_string(ret), paint_map_accu_canvas_8UC1[uu_depth]);
			int how_many_loop = 0;
		//	do {
				tot_cnt=pad_p_map(paint_map_accu_canvas_8UC1[uu_depth], tag, x_src_canvas_Rect_full, uu_depth);
				
				ret = pmap_count_zero(paint_map_canvas_8UC1[uu_depth], tag, x_src_canvas_Rect_full);
			//	debug_image("ing/__paint_map_a5_" + tag_[render_method] + to_string(uu_depth)+"_"+to_string(ret), paint_map_accu_canvas_8UC1[uu_depth]);
				how_many_loop++;
			//} while (ret > 1);
			cout << "how many loop " << how_many_loop << endl;
		}
		r_cout << "times_0 = " << times_0 << endl;
		r_cout << setw(7) /*m_tag*/ << " :L " << "st size" << "times" << "times, pscale" << "st_size*strk_times*p_scale" << " , Br_size " << endl;
		r_cout << setw(7) << m_tag <<setw(3) << uu_depth << " : "
			<<setw(5)<< render_region_set[uu_depth].Region_list.size()<<", "<<
			setw(5)<<x_strk_times[uu_depth]<<", "
			<<setw(5)<<times<<", "<<setw(6)<< g_paint_try_scale[uu_depth]<<" :  "<<setw(7)<<
			render_region_set[uu_depth].Region_list.size()*x_strk_times[uu_depth]*g_paint_try_scale[uu_depth]<<
			" , " << 
			setw(5) << render_region_set[uu_depth].Region_list.size() <<
			",  " << setw(4) << x_paint_area_brush_count <<
			endl;
		r_cout << "try cnt "<< grid_try_sum[uu_depth]<< "<"<<setw(6)<<extended_try[uu_depth]<<">"<<
			std::fixed << setw(7) << std::setprecision(2) << (float((float)extended_try[uu_depth]/(float)grid_try_sum[uu_depth])*100.0)<<
			"%  changed count = " << r_s_changed_count[uu_depth];
		r_cout << " % = " << std::fixed << setw(7)<<std::setprecision(2)<<
			((float)r_s_changed_count[uu_depth] / (float)grid_try_sum[uu_depth])*100.0 << endl;
		g_file_clog << m_tag << " ," << setw(3) << uu_depth << " ,size, " << render_region_set[uu_depth].Region_list.size() <<
			 " paint scale : " << setw(3) << g_paint_try_scale[uu_depth] <<
			"," << setw(4) << x_paint_area_brush_count 
			<< endl;
		debug_image("ing/_i_" + to_string(uu_depth) + m_t +to_string(g_paint_try_scale[uu_depth]),  ing_canvas[uu_depth]);

#ifdef _DEBUG_RENDER
		debug_image("ing/pa_" + to_string(uu) + "_" + m_tag + to_string(called), painting_area_canvas[uu_depth]);

		debug_image("ing/ch_" + m_tag + to_string(called), uu, changed_map_canvas[uu_depth]);

#endif

		debug_image("ing/_ac_" + to_string(uu_depth) + m_t , accu_canvas[uu_depth]);
		debug_image("ing/try_" + to_string(uu_depth) + m_t , r_try_map_1c[uu_depth]);
		//paint_map_accu_8UC1[uu_depth] += paint_map_canvas_8UC1[uu_depth];
		//paint_map_accu_8UC1[uu_depth+1] = paint_map_accu_8UC1[uu_depth];
		
		//tag = "p_map_a_" + m_tag + to_string(uu_depth);
		//pmap_count_zero(paint_map_accu_8UC1[uu_depth],tag);
		
	}//end of for depth uu
	
	
//	int ret = draw_grid_2(accu_canvas[render_depth-1].clone(), render_region_set, string("") + m_tag, render_depth, -1, 255, m_tag);
	//ret = draw_grid_2(r_try_map_1c[render_depth-1], render_region_set, string("try_") + m_tag, render_depth, -1, 255, m_tag);
//	r_cout << "Merge SKIP count : " << g_merge_skip_count << endl;
	//r_cout << " depth : " << render_depth << endl;
	//r_cout << "size mismatch " << size_mismatch << endl;
	
	result_image=accu_canvas[render_depth-1];
	/*
	time((&e_time));
	char s_buff[20];
	localtime_s(&t_e, &e_time);
	strftime(s_buff, 20, "%Y-%m-%d %H:%M:%S", &t_s);
	char e_buff[20];
	strftime(e_buff, 20, "%Y-%m-%d %H:%M:%S", &t_e);

	r_cout << setw(15) << m_tag<<"-->"<<s_buff << " : "<<e_buff << endl;
	csv_log << "R End, "<<m_tag << "," << s_buff << "," << e_buff << endl;
		*/
#ifdef DEPTH_MAP
	double minval, maxval;
//	int i_minloc, i_maxloc;
	int i_minval, i_maxval;
	cv::minMaxLoc(depth_map_canvas_8UC1(x_src_canvas_Rect_full), &minval, &maxval);// , &i_minloc, &i_maxloc);
	i_minval = (int)minval;
	i_maxval = (int)maxval;

	int diff = i_maxval - i_minval;
	if (diff == 0) diff = i_maxval;
	int step = (int)256 / diff;
	Mat p_map;
	depth_map_canvas_8UC1.convertTo(p_map, -1, step);

	debug_image("depth_map"+m_tag, p_map);
#endif
	r_cout.flush();
		return 0;

}
