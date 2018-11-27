#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"

int   render_::render_image()

{
//	Point Strk_srtPoint, Strk_endPoint;
//	Size Strk_size;
//	int Strk_w_size, Strk_h_size;
	int saved_depth = -1;
//	Point Strk_point_canvas;
	//int x_src_width = g_src_image_width;
//	int x_src_height = g_src_image_height;
	//int x_src_channels = g_src_image_channels;
	//int x_src_step1 = (int)g_src_image_step1;

	int astroke_depth = -1;
	

	//Mat beforeImg_canvas;
#ifdef _DEBUG_RENDER
	cv::Mat painting_area_canvas[MAX_DEPTH];
	cv::Mat changed_map_canvas[MAX_DEPTH];
#endif
	int times;
	int times_0=0;
	int value_0 = 0;
	int from_layer_0 = 0;
	int pmap_SET0 = 0;
	partition_Node* region_p;
	

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


	int stroke_area[MAX_DEPTH];
	int stroke_area_ratio[MAX_DEPTH];

	Rect Strk_canvas_ROI_rect;
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {

		
		stroke_area[uu_depth] = 0;
		stroke_area_ratio[uu_depth] = 0;
		for (list<partition_Node*>::iterator partition_it = render_region_set[uu_depth].Region_list.begin();
			partition_it != render_region_set[uu_depth].Region_list.end(); partition_it++) {
			partition_Node* region_p = (*partition_it);
			
			canvas_rect(region_p, Strk_canvas_ROI_rect);
			stroke_area[uu_depth] += Strk_canvas_ROI_rect.height * Strk_canvas_ROI_rect.width;
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
		pmap_overlay_fill(from_layer_0, paint_map_canvas_8UC1[uu_depth], 255);// paint_map_canvas_8UC1[uu_depth].ROI(src image).setTo(255)
		pmap_overlay_fill(from_layer_0, paint_map_accu_canvas_8UC1[uu_depth], 255); //paint_map_accu_canvas_8UC1[uu_depth].ROI(src image).setTo(255)

		//debug_image("ing/__paint_map_1_" + tag_[render_method] + to_string(uu_depth), paint_map_canvas_8UC1[0]);
		if (stroke_area_ratio[uu_depth] < 45 && x_changed_depth < 0)
		{
			x_changed_depth = uu_depth;
			r_cout << "changed depth = " << x_changed_depth << endl;
				
		}
	}

//	r_cout << "c" << x_retry_map_1c.size().height << ", " << x_retry_map_1c.size().width << endl;
	r_cout << setw(7) /*m_tag*/ << " :L " << "st size" << "times" << "times, pscale" << "st_size*strk_times*p_scale" << " , Br_size " ;
	r_cout << "try cnt " << setw(7) << "<ex_try" << setw(7) << "  ratio" << "ch count  % ";
	Rect r_;
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		
		for (list<partition_Node*>::iterator region_iter = render_region_set[uu_depth].Region_list.begin();
				region_iter != render_region_set[uu_depth].Region_list.end(); region_iter++)
		{
		 region_p = (*region_iter);
		
			render_stroke_no=(region_p)->no;
			astroke_depth = (region_p)->depth;
		
			if (astroke_depth != saved_depth) {
				if ( astroke_depth > 0 )
					accu_canvas[astroke_depth] = accu_canvas[astroke_depth - 1].clone();// when new layer
				accu_canvas_data[astroke_depth] = (unsigned char*)accu_canvas[astroke_depth].data;
				saved_depth = astroke_depth;
				x_tbrush_cnt = 0;
			}

			int layer_more_0 = 0;
			int mode_0 = 0;
			r_ = Rect((region_p)->srtPoint, (region_p)->endPoint);
			times=paint_a_stroke(region_p,layer_more_0,mode_0,Point(0,0),astroke_depth);
			/* 
			if (astroke_depth == (x_changed_depth-2)) {
				times_0 += paint_a_stroke(region_p, layer_more_0, mode_0, Point(0, 0), astroke_depth);
				times_0 += paint_a_stroke(region_p, layer_more_0, mode_0, Point(0, 0), astroke_depth);
				times_0 += paint_a_stroke(region_p, layer_more_0, mode_0, Point(0, 0), astroke_depth);
			}
			*/
#ifdef _DEBUG_RENDER
			//	Strk_srtPoint= (region_p)->srtPoint;
			//	Strk_endPoint = (region_p)->endPoint;
			//	Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
			//	Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;
			//	Strk_size=Size(Strk_w_size, Strk_h_size);

			rectangle_canvas(painting_area_canvas[astroke_depth], Rect(Strk_srtPoint, Strk_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
		
		
		}//end of it region

		string pmap_path = "pmap/r" + to_string(render_method) + "/_" + m_tag_ + to_string(uu_depth)+"_";
	//	int zero_cnt=pmap_count_zero(paint_map_canvas_8UC1[uu_depth], pmap_tag, x_src_canvas_Rect_full);
		debug_image(pmap_path + "accu_" + "_" + to_string(uu_depth) + "_", accu_canvas[uu_depth]);
		debug_image(pmap_path+ "pmap_a_" + to_string(uu_depth), paint_map_accu_canvas_8UC1[uu_depth]);
		debug_image(pmap_path + "pmap_i_" + to_string(uu_depth), paint_map_canvas_8UC1[uu_depth]);
				//paint_map_canvas_8UC1[uu_depth].copyTo(paint_map_accu_canvas_8UC1[uu_depth]);
	
		int tot_cnt;
		if (uu_depth == from_layer_0) {//apply paint map 
			pmap_overlay_fill(x_changed_depth, paint_map_accu_canvas_8UC1[uu_depth], pmap_SET0);
			//ret = pmap_count_zero(paint_map_canvas_8UC1[uu_depth], pmap_path, x_src_canvas_Rect_full);
		
			debug_image(pmap_path+ "_" + to_string(uu_depth), paint_map_accu_canvas_8UC1[uu_depth]);
			
			tot_cnt=pad_p_map(paint_map_accu_canvas_8UC1[uu_depth], r_,pmap_path, uu_depth);
				
			//ret = pmap_count_zero(paint_map_canvas_8UC1[uu_depth], pmap_path, x_src_canvas_Rect_full);
			
		}
		debug_image("ing/_i_" + to_string(uu_depth) + m_t + to_string(g_paint_try_scale[uu_depth]), ing_canvas[uu_depth]);
		debug_image("ing/_ac_" + to_string(uu_depth) + m_t, accu_canvas[uu_depth]);
		debug_image("ing/try_" + to_string(uu_depth) + m_t, r_try_map_1c[uu_depth]);


		//r_cout << "times_0 = " << times_0 << endl;
	
		r_cout << setw(7) << m_tag << setw(3) << uu_depth << " : "
			<< setw(5) << render_region_set[uu_depth].Region_list.size() << ", " <<
			setw(5) << x_strk_times[uu_depth] << ", "
			<< setw(5) << times << ", " << setw(6) << g_paint_try_scale[uu_depth] << " :  " << setw(7) <<
			render_region_set[uu_depth].Region_list.size()*x_strk_times[uu_depth] * g_paint_try_scale[uu_depth] <<
			" , " <<
			setw(5) << render_region_set[uu_depth].Region_list.size() <<
			",  " << setw(4) << x_paint_area_brush_count;
		r_cout <<  grid_try_sum[uu_depth]<< ","<<setw(6)<<extended_try[uu_depth]<<","<<
			std::fixed << setw(7) << std::setprecision(2) << (float((float)extended_try[uu_depth]/(float)grid_try_sum[uu_depth])*100.0)<<
			"," << r_s_changed_count[uu_depth];
		r_cout << "," << std::fixed << setw(7)<<std::setprecision(2)<<
			((float)r_s_changed_count[uu_depth] / (float)grid_try_sum[uu_depth])*100.0 << endl;

		g_file_clog << m_tag << " ," << setw(3) << uu_depth << " ,size, " << render_region_set[uu_depth].Region_list.size() <<
			 " paint scale : " << setw(3) << g_paint_try_scale[uu_depth] <<
			"," << setw(4) << x_paint_area_brush_count 
			<< endl;
		
#ifdef _DEBUG_RENDER
		debug_image("ing/pa_" + to_string(uu) + "_" + m_tag + to_string(called), painting_area_canvas[uu_depth]);

		debug_image("ing/ch_" + m_tag + to_string(called), uu, changed_map_canvas[uu_depth]);

#endif

		
	
		
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
