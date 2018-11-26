#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"

void render_::func_p_map(Mat & a_map_canvas_8UC1,string tag,Rect x_src_canvas_Rect_full) {
	double minval, maxval;
	Point minloc, maxloc;
	int i_minval, i_maxval;
	Mat ROI=a_map_canvas_8UC1(x_src_canvas_Rect_full);
	cv::minMaxLoc(ROI, &minval, &maxval, &minloc, &maxloc);
	i_minval = (int)minval;
	i_maxval = (int)maxval;


	int diff = i_maxval - i_minval;
	int step = (int)256 / diff;
	Mat p_map;
	a_map_canvas_8UC1.convertTo(p_map, -1, step);
	debug_image("ing/" + tag, p_map);

	r_cout <<tag<< " minLoc " << minloc.x << ", " << minloc.y << " : " << (int)i_minval ;
	r_cout << " maxLoc " << maxloc.x << ", " << maxloc.y << " : " << (int)i_maxval ;
	r_cout << " diff = " << diff << ", step = " << step ;


	//Mat paint_ROI = a_map_canvas_8UC1(x_src_canvas_Rect_full);
	int zero_pixel_cnt = (x_src_image_size.height*x_src_image_size.width) - cv::countNonZero(ROI);
	r_cout << " zero pixel = " << zero_pixel_cnt << " / " << (x_src_image_size.height*x_src_image_size.width) <<
		" = " << std::fixed << setprecision(2) << ((float)zero_pixel_cnt / (float)(x_src_image_size.height*x_src_image_size.width))*100.0 << endl;
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
	//	r_cout << m_tag << " : "<<render_depth<<" size: "<< render_Stroke_set[i].stroke_list.size()<<endl;
//	for (int i = 0; i < render_depth; i++) {
//		r_cout << "0 "<<i << ", " << render_brush_size[i] << endl;
//	}

//	saved_depth = -1;

	int stroke_area[MAX_DEPTH];
	int stroke_area_ratio[MAX_DEPTH];

	if (x_retry_map_1c.size().height == 0) {
		r_cout << "v" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;

	}
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {

		if (x_retry_map_1c.size().height == 0) {
		//	r_cout << "x" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;

		}
		stroke_area[uu_depth] = 0;
		stroke_area_ratio[uu_depth] = 0;
		for (list<partition_Node*>::iterator partition_it = render_Stroke_set[uu_depth].stroke_list.begin();
			partition_it != render_Stroke_set[uu_depth].stroke_list.end(); partition_it++) {
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
			r_cout << setw(3) << uu_depth << " : " << setw(7) << render_Stroke_set[uu_depth].stroke_list.size() << " : area : " <<
				setw(7) << stroke_area[uu_depth] << " : " <<
				fixed << setw(7) << setprecision(1) << (float)(stroke_area[uu_depth]) / (float)(stroke_area[0])*100.0 <<
				setw(7) << " : " << stroke_area_ratio[uu_depth]<<endl;
	//		r_cout << "a" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
	}
//	r_cout << "b" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
	x_changed_depth = 5;
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		if (stroke_area_ratio[uu_depth] < 85)
		{
			x_changed_depth = uu_depth;
			r_cout << "changed depth = " << x_changed_depth << endl;
			break;
		}
	}

//	r_cout << "c" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;

	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		
		for (list<partition_Node*>::iterator it_partition_it = render_Stroke_set[uu_depth].stroke_list.begin();
				it_partition_it != render_Stroke_set[uu_depth].stroke_list.end(); it_partition_it++)
		{
		//	r_cout << "e" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
			partition_Node* strk_p = (*it_partition_it);
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

			
			times=paint_a_stroke(strk_p,1);
			if (astroke_depth == (x_changed_depth-2)) {
				times_0 += paint_a_stroke(strk_p,0);
				times_0 += paint_a_stroke(strk_p, 0);
				times_0 += paint_a_stroke(strk_p, 0);
			}
			
#ifdef _DEBUG_RENDER
			rectangle_canvas(painting_area_canvas[astroke_depth], Rect(Strk_srtPoint, Strk_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
		
		
		}//end of it astroke
		
	
		r_cout << "times_0 = " << times_0 << endl;
		r_cout << setw(7) /*m_tag*/ << " :L " << "st size" << "times" << "times, pscale" << "st_size*strk_times*p_scale" << " , Br_size " << endl;
		r_cout << setw(7) << m_tag <<setw(3) << uu_depth << " : "
			<<setw(5)<< render_Stroke_set[uu_depth].stroke_list.size()<<", "<<
			setw(5)<<x_strk_times[uu_depth]<<", "
			<<setw(5)<<times<<", "<<setw(6)<< g_paint_try_scale[uu_depth]<<" :  "<<setw(7)<<
			render_Stroke_set[uu_depth].stroke_list.size()*x_strk_times[uu_depth]*g_paint_try_scale[uu_depth]<<
			" , " << 
			setw(5) << render_Stroke_set[uu_depth].stroke_list.size() <<
			",  " << setw(4) << x_paint_area_brush_count <<
			endl;
		r_cout << "try cnt "<< grid_try_sum[uu_depth]<< "<"<<setw(6)<<extended_try[uu_depth]<<">"<<
			std::fixed << setw(7) << std::setprecision(2) << (float((float)extended_try[uu_depth]/(float)grid_try_sum[uu_depth])*100.0)<<
			"%  changed count = " << r_s_changed_count[uu_depth];
		r_cout << " % = " << std::fixed << setw(7)<<std::setprecision(2)<<
			((float)r_s_changed_count[uu_depth] / (float)grid_try_sum[uu_depth])*100.0 << endl;
		g_file_clog << m_tag << " ," << setw(3) << uu_depth << " ,size, " << render_Stroke_set[uu_depth].stroke_list.size() <<
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
		string tag = "p_map_" + m_tag+to_string(uu_depth);
		func_p_map(paint_map_canvas_8UC1[uu_depth],tag,x_src_canvas_Rect_full);
		//tag = "p_map_a_" + m_tag + to_string(uu_depth);
		//func_p_map(paint_map_accu_8UC1[uu_depth],tag);
		
	}//end of for depth uu
	
	
//	int ret = draw_grid_2(accu_canvas[render_depth-1].clone(), render_Stroke_set, string("") + m_tag, render_depth, -1, 255, m_tag);
	//ret = draw_grid_2(r_try_map_1c[render_depth-1], render_Stroke_set, string("try_") + m_tag, render_depth, -1, 255, m_tag);
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
