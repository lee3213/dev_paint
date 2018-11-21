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
		" = " << setw(10) << std::fixed << setprecision(2) << ((float)zero_pixel_cnt / (float)(x_src_image_size.height*x_src_image_size.width))*100.0 << endl;
}

int   render_::PainterlyRendering()

{
	static int tbrush_cnt = 0;
	//int x_image_width = g_src_image_width;
//	int x_image_height = g_src_image_height;
	int x_image_channels = g_src_image_channels;
	int x_image_step1 = (int)g_src_image_step1;

	
	Rect x_src_canvas_Rect_full;

	Point Strk_srtPoint, Strk_endPoint;
	Point fetch_color_Point,fetch_color_canvas_point;

	Point centered_SrtPoint, centered_EndPoint;
	Point centered_SrtPoint_canvas, centered_EndPoint_canvas;
//	int _paint_area_w_size, paint_area_h_size;

	int brush_area_w_size, brush_area_h_size;
	Size Strk_size;
	Size Brush_size;
	int size_mismatch = 0;
	int saved_depth = -1;
	
	int Strk_w_size, Strk_h_size;

	Point Strk_point_canvas;
	int times;
	int paint_area_brush_count;
	int astroke_depth;
	cv::Mat src_canvas_ROI;
	//cv::Mat src_canvas_ROI_clone;

	random_device rand_x[MAX_DEPTH];
	random_device rand_y[MAX_DEPTH];

	//Mat beforeImg_canvas;
#ifdef _DEBUG_RENDER
	cv::Mat painting_area_canvas[MAX_DEPTH];
	cv::Mat changed_map_canvas[MAX_DEPTH];
#endif
	int color_BGR_B, color_BGR_G, color_BGR_R;
	
	
	src_canvas.create(x_canvas_size_height, x_canvas_size_width, CV_8UC3);
	src_canvas.setTo(255);
	
	x_src_image_size.width = (g_src_image_width);
	x_src_image_size.height= g_src_image_height;
	
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
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		for (list<Stroke_Node*>::iterator Stroke_it = render_Stroke_set[uu_depth].stroke_list.begin();
			Stroke_it != render_Stroke_set[uu_depth].stroke_list.end(); Stroke_it++) {
			Strk_srtPoint = (*Stroke_it)->srtPoint;
			Strk_endPoint = (*Stroke_it)->endPoint;
			Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
			Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;

			Strk_size = Size(Strk_w_size, Strk_h_size);
			astroke_depth = (*Stroke_it)->depth;
			Strk_point_canvas = Point(Strk_srtPoint.x + x_canvas_bezel_size, Strk_srtPoint.y + x_canvas_bezel_size);
			Rect Strk_canvas_ROI_rect(Strk_point_canvas, Strk_size);
			Mat depth_map_canvas_8UC1_ROI = depth_map_canvas_8UC1(Strk_canvas_ROI_rect);
			depth_map_canvas_8UC1_ROI = astroke_depth;//assign a value to Matrix
		}
	}
	
	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		
		for (list<Stroke_Node*>::iterator Stroke_it = render_Stroke_set[uu_depth].stroke_list.begin();
				Stroke_it != render_Stroke_set[uu_depth].stroke_list.end(); Stroke_it++)
		{
		
			
			Strk_srtPoint= (*Stroke_it)->srtPoint;
			Strk_endPoint = (*Stroke_it)->endPoint;
			Strk_w_size = Strk_endPoint.x - Strk_srtPoint.x;
			Strk_h_size = Strk_endPoint.y - Strk_srtPoint.y;

			Strk_size=Size(Strk_w_size, Strk_h_size);
			astroke_depth = (*Stroke_it)->depth;
			render_stroke_no=(*Stroke_it)->no;
			
			if (astroke_depth > MAX_DEPTH) {
				cerr << "astroke depth >" << MAX_DEPTH << " : " << (*Stroke_it)->depth << endl;
				r_cout << "astroke depth >" << MAX_DEPTH << " : " << (*Stroke_it)->depth << endl;
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

			if (astroke_depth != saved_depth) {
				if (astroke_depth > 0) {
					accu_canvas[astroke_depth] = accu_canvas[astroke_depth - 1].clone();
				}
				accu_canvas_data[astroke_depth] = (unsigned char*)accu_canvas[astroke_depth].data;
				saved_depth = astroke_depth;
				tbrush_cnt = 0;
			}

			

			
#ifdef _DEBUG_RENDER
			rectangle_canvas(painting_area_canvas[astroke_depth], Rect(Strk_srtPoint, Strk_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
			brush_area_h_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
			brush_area_w_size = render_brush_size[astroke_depth];// brush size(painting area) per each depth
			Brush_size = Size(brush_area_w_size, brush_area_h_size);

			int brush_area_h_size_half = brush_area_h_size / 2 + brush_area_h_size%2;
			int brush_area_w_size_half = brush_area_w_size / 2 + brush_area_w_size % 2;

			 times = ((Strk_w_size * Strk_h_size) /
					(brush_area_h_size * brush_area_w_size));

			 if (times == 0) times = 1;
			 x_strk_times[astroke_depth] = times;
				 paint_area_brush_count = times*g_paint_try_scale[astroke_depth];

				 uniform_int_distribution<int> diStrk_x(0, (int)(Strk_w_size));
				 uniform_int_distribution<int> diStrk_y(0, (int)(Strk_h_size));

			//	 random_x = new int[paint_area_brush_count*50];
			//	 random_y = new int[paint_area_brush_count*50];


				// for (int painting_try = 0; painting_try < paint_area_brush_count*50; painting_try++) {
					
			//	 }
/*
			if (paint_area_brush_count == 0) {
				r_cout << "bc == 0 d: " << astroke_depth
					<< " Strk_size : " << Strk_w_size << ", " << Strk_h_size <<"= "<<Strk_w_size*Strk_h_size<<
					"brush area size= " <<
					(brush_area_h_size * brush_area_w_size) << " try_Scale " <<
					g_paint_try_scale[astroke_depth] << " : "<<times<<
					" " 
					<< " m_render_brush_size " << render_brush_size[astroke_depth] <<
					" , bsc :" <<g_brush_scale[astroke_depth]<<
					endl;
				paint_area_brush_count = 2;
				//size_mismatch++;

			}
			*/
			

			
			grid_try_sum[astroke_depth] += paint_area_brush_count ;//
			int extended_paint_area_brush_count = paint_area_brush_count;
			for (render_try = 0; render_try <extended_paint_area_brush_count; render_try++)
			{
				string f_name_tdt = "p" + to_string(astroke_depth) + "/" + m_t_ + to_string(astroke_depth) + "_t_" + to_string(render_try);
				bool modified = true;
			//	int retry_cnt = 1;
				int adjusted_h_size;
				int adjusted_w_size;
				random_x = diStrk_x(rand_x[astroke_depth]);
				random_y = diStrk_y(rand_y[astroke_depth]);
				fetch_color_Point.x = Strk_srtPoint.x + random_x;
				fetch_color_Point.y = Strk_srtPoint.y + random_y;
				
				if (uu_depth < 2) {
					int depth_value = *(depth_map_canvas_8UC1_data + fetch_color_Point.x + fetch_color_Point.y*x_canvas_size_width);
					if (depth_value != 255 && depth_value > (x_last_depth-2)) {
						extended_paint_area_brush_count++;
							continue;
					}
				}
				int fetch_Index = (fetch_color_Point.x + (fetch_color_Point.y*x_src_image_size.width))*(x_image_channels);
				int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*x_src_image_size.width);

				//int fetch_canvas_Index = ((fetch_color_Point.x +x_canvas_bezel_size)
					//+ (fetch_color_Point.y +x_canvas_bezel_size)*(x_src_image_size.width +x_canvas_bezel_size))*x_image_channels;

				p_peek(x_src_ptr, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

				p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);

				if (brush_area_w_size_half <x_canvas_bezel_size)
					adjusted_w_size =x_canvas_bezel_size;
				else
					adjusted_w_size = brush_area_w_size_half;

				if (brush_area_h_size_half <x_canvas_bezel_size)
					adjusted_h_size =x_canvas_bezel_size;
				else
					adjusted_h_size = brush_area_w_size_half;
				
				centered_SrtPoint.x = fetch_color_Point.x - adjusted_w_size;
				centered_SrtPoint.y = fetch_color_Point.y - adjusted_h_size;

				centered_EndPoint.x = fetch_color_Point.x +(adjusted_w_size-brush_area_w_size_half);// to overcome odd brush size
				centered_EndPoint.y = fetch_color_Point.y +(adjusted_h_size-brush_area_h_size_half);// to overcome odd brush size

	
			//	Point Point_X, Point_Y;
			

				centered_SrtPoint_canvas.x = centered_SrtPoint.x +x_canvas_bezel_size;
				centered_SrtPoint_canvas.y = centered_SrtPoint.y +x_canvas_bezel_size;
				centered_EndPoint_canvas.x = centered_EndPoint.x +x_canvas_bezel_size;
				centered_EndPoint_canvas.y = centered_EndPoint.y +x_canvas_bezel_size;
			
				Rect centered_ROI_canvas_Rect = Rect(centered_SrtPoint_canvas, centered_EndPoint_canvas);

#ifdef _DEBUG_RENDER
				rectangle(painting_area_canvas[astroke_depth], centered_ROI_canvas_Rect, Scalar(0, 0, 0));//BLACK
#endif
				src_canvas_ROI = src_canvas(centered_ROI_canvas_Rect);
			//	src_canvas_ROI_clone = src_canvas(centered_ROI_canvas_Rect).clone();
			

				Mat before_canvas_ROI = accu_canvas[astroke_depth](centered_ROI_canvas_Rect);
				Mat before_canvas_ROI_clone = accu_canvas[astroke_depth](centered_ROI_canvas_Rect).clone();

				Mat changed_canvas_ROI = accu_canvas[astroke_depth](centered_ROI_canvas_Rect);
				Mat changed_canvas_ROI_clone = accu_canvas[astroke_depth](centered_ROI_canvas_Rect).clone();

				Mat ing_ROI_canvas = ing_canvas[astroke_depth](centered_ROI_canvas_Rect);

				//Mat ing_ROI_clone = ing_canvas[astroke_depth](centered_ROI_canvas_Rect).clone();
				
				
				int result = P_Rendering(
					src_canvas_ROI,
					before_canvas_ROI_clone,
					changed_canvas_ROI_clone,
					ing_ROI_canvas,
					changed_canvas_ROI,
					accu_canvas[astroke_depth],
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
					ing_canvas_data[astroke_depth]
				);
			
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
				if (tbrush_cnt < DEBUG_BRUSH_CNT) {
					debug_image(f_name_tdt + "_4ING_ING_", ing_canvas[astroke_depth]);
					debug_image(f_name_tdt + "_5ACC_ACC_", accu_canvas[astroke_depth]);
				}
				tbrush_cnt++;
			}
			//end of retry
			extended_try[uu_depth] += extended_paint_area_brush_count;
			//delete[] random_x;
		//	delete[] random_y;
		}//end of it astroke

	//	time_t one_pass_time_e;
		/*time((&one_pass_time_e));
		char s_buff[20];
		tm t_s, t_e;
		localtime_s(&t_s, &one_pass_time_s);
		localtime_s(&t_e,&one_pass_time_e);
		strftime(s_buff, 20, "%Y-%m-%d %H:%M:%S",&t_s );
		char e_buff[20];
		strftime(e_buff, 20, "%Y-%m-%d %H:%M:%S",&t_e );
		*/
		r_cout << setw(10) << m_tag << " :L " << setw(3) << uu_depth << " : "
			<<setw(5)<< render_Stroke_set[uu_depth].stroke_list.size()<<
			setw(5)<<x_strk_times[uu_depth]<<"::  "
			<<setw(5)<<times<<"times, paint scale : " <<
			" = "<<setw(6)<<
			render_Stroke_set[uu_depth].stroke_list.size()*x_strk_times[uu_depth]*g_paint_try_scale[uu_depth]<<
			" , Br_size " << 
			setw(5) << render_Stroke_set[uu_depth].stroke_list.size() <<
			", "<<setw(5)<<g_brush_scale[uu_depth]<<
			" br_cnt " << setw(4) << paint_area_brush_count <<
			endl;
		r_cout << "try cnt "<< grid_try_sum[uu_depth]<< "<"<<setw(6)<<extended_try[uu_depth]<<">"<<
			std::fixed << setw(7) << std::setprecision(2) << (float((float)extended_try[uu_depth]/(float)grid_try_sum[uu_depth])*100.0)<<
			"%  changed count = " << r_s_changed_count[uu_depth];
		r_cout << " % = " << std::fixed << setw(7)<<std::setprecision(2)<<
			((float)r_s_changed_count[uu_depth] / (float)grid_try_sum[uu_depth])*100.0 << endl;
		g_file_clog << m_tag << " ," << setw(3) << uu_depth << " ,size, " << render_Stroke_set[uu_depth].stroke_list.size() <<
			 " paint scale : " << setw(3) << g_paint_try_scale[uu_depth] <<
			"," << setw(4) << paint_area_brush_count 
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
		return 0;

}
