#include "stdafx.h"

#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"
#include "define_local.h"
Size x_src_image_size;
Rect x_src_Rect_full;

void render_::func_p_map(Mat & a_map_8UC1,string tag) {
	double minval, maxval;
	Point minloc, maxloc;
	int i_minval, i_maxval;
	cv::minMaxLoc(a_map_8UC1, &minval, &maxval, &minloc, &maxloc);
	i_minval = (int)minval;
	i_maxval = (int)maxval;

	int diff = i_maxval - i_minval;
	int step = (int)256 / diff;
	Mat p_map;
	a_map_8UC1.convertTo(p_map, -1, step);
	debug_image("ing/" + tag, p_map);
	r_cout <<tag<< " minLoc " << minloc.x << ", " << minloc.y << " : " << (int)i_minval ;
	r_cout << " maxLoc " << maxloc.x << ", " << maxloc.y << " : " << (int)i_maxval ;
	r_cout << " diff = " << diff << ", " << "step = " << step ;


	Mat paint_ROI = a_map_8UC1(x_src_Rect_full);
	int zero_pixel_cnt = (x_src_image_size.height*x_src_image_size.width) - cv::countNonZero(paint_ROI);
	r_cout << "zero pixel = " << zero_pixel_cnt << " / " << (x_src_image_size.height*x_src_image_size.width) <<
		" = " << setw(10) << setprecision(2) << ((float)zero_pixel_cnt / (float)(x_src_image_size.height*x_src_image_size.width))*100.0 << endl;
}

int   render_::PainterlyRendering()

{
	static int tbrush_cnt = 0;
	int image_width = g_src_image_width;
	int image_height = g_src_image_height;
	int image_channels = g_src_image_channels;
	int image_step1 = (int)g_src_image_step1;
	

	Point St_srtPoint, St_endPoint, fetch_color_Point;
	Point centered_SrtPoint, centered_EndPoint;
	Point canvas_centered_SrtPoint, canvas_centered_EndPoint;
//	int _paint_area_w_size, paint_area_h_size;

	int brush_area_w_size, brush_area_h_size;


	random_device rand_x[MAX_DEPTH];
	random_device rand_y[MAX_DEPTH];

	//Mat beforeImg_canvas;
#ifdef _DEBUG_RENDER
	cv::Mat painting_area_canvas[MAX_DEPTH];
	cv::Mat changed_map_canvas[MAX_DEPTH];
#endif
	
	
	
	src_canvas.create(canvas_size_height, canvas_size_width, CV_8UC3);
	src_canvas.setTo(255);
	x_src_image_size.width = (g_src_image_width);
	x_src_image_size.height= g_src_image_height;
	 x_src_Rect_full.width= x_src_image_size.width; 
	 x_src_Rect_full.height = x_src_image_size.height;
	 x_src_Rect_full.x = canvas_bezel_size;
	 x_src_Rect_full.y = canvas_bezel_size;
		
	 
	Mat src_ROI_canvas_full = src_canvas(x_src_Rect_full);
	 m_srcImg_.copyTo(src_ROI_canvas_full);
//	debug_image("src_canvas", src_canvas);
//	debug_image("src_canvas_ROI", src_ROI_canvas_full);

	rst_accu_canvas[0].create(canvas_size_height, canvas_size_width, CV_8UC3);
	rst_accu_canvas[0].setTo(255);
	rst_accu_canvas_data[0] = (unsigned char*)rst_accu_canvas[0].data;

	for (int i = 0; i < MAX_DEPTH; i++) {
		r_try_map_1c[i].create(image_height, image_width, CV_8UC1);
		r_try_map_1c[i].setTo(255);
		r_try_map_1c_data[i] = r_try_map_1c[i].data;

		r_s_grid_painting_try[i] = 0;
		r_s_changed_count[i] = 0;

		ing_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		ing_canvas_data[i] = (unsigned char*)ing_canvas[i].data;
		ing_canvas[i].setTo(255);
		
		

		paint_map_8UC1[i].create(canvas_size_height, canvas_size_width, CV_8UC1);
		paint_map_8UC1_data[i] = paint_map_8UC1[i].data;
		paint_map_8UC1[i].setTo(0);

		paint_map_accu_8UC1[i].create(canvas_size_height, canvas_size_width, CV_8UC1);
		paint_map_accu_8UC1_data[i] = paint_map_accu_8UC1[i].data;
		paint_map_accu_8UC1[i].setTo(0);
#ifdef _DEBUG_RENDER
		painting_area_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		painting_area_canvas[i].setTo(255);

		changed_map_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		changed_map_canvas[i].setTo(255);

		painting_area_canvas[i].create(canvas_size_height, canvas_size_width, CV_8UC3);
		painting_area_canvas[i].setTo(255);
		rectangle_canvas(painting_area_canvas[i], Rect(St_srtPoint, St_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
	}
	paint_map_8UC1[MAX_DEPTH].create(canvas_size_height, canvas_size_width, CV_8UC1);
	paint_map_8UC1[MAX_DEPTH].setTo(0);
	depth_map_8UC1.create(canvas_size_height, canvas_size_width, CV_8UC1);
	depth_map_8UC1_data = depth_map_8UC1.data;
	depth_map_8UC1.setTo(255);
	//int first_layer = -1;
	//for(int i=0;i<render_depth;i++)
	//	r_cout << m_tag << " : "<<render_depth<<" size: "<< mm_aStroke_set[i].stroke_list.size()<<endl;
//	for (int i = 0; i < render_depth; i++) {
//		r_cout << "0 "<<i << ", " << brush_size[i] << endl;
//	}

//	saved_depth = -1;

	int size_mismatch = 0;
	int saved_depth=-1;
	cv::Mat src_ROI_canvas;
	cv::Mat src_ROI_canvas_clone;
	int st_w_size, st_h_size;
	Size stroke_size;
	int times;

	for (int uu_depth = 0; uu_depth < render_depth; uu_depth++) {
		
		int paint_area_brush_count;
	
		for (list<Stroke_Node*>::iterator Stroke_it = mm_aStroke_set[uu_depth].stroke_list.begin();
				Stroke_it != mm_aStroke_set[uu_depth].stroke_list.end(); Stroke_it++)
		{
		
			int astroke_depth;
			St_srtPoint= (*Stroke_it)->srtPoint;
			St_endPoint = (*Stroke_it)->endPoint;
			st_w_size = St_endPoint.x - St_srtPoint.x;
			st_h_size = St_endPoint.y - St_srtPoint.y;
			astroke_depth = (*Stroke_it)->depth;
			render_stroke=(*Stroke_it)->no;
			if (astroke_depth != saved_depth) {
				if (astroke_depth != 0) { 
					if (saved_depth > astroke_depth) {
						r_cout << "<< " << m_tag << " " << saved_depth << " : " << astroke_depth << endl;
						return -1;
					}
					else { 
						rst_accu_canvas[astroke_depth] = rst_accu_canvas[astroke_depth - 1].clone();
						rst_accu_canvas_data[astroke_depth] = (unsigned char*)rst_accu_canvas[astroke_depth].data;
						ing_canvas_data[astroke_depth] = (unsigned char*)ing_canvas[astroke_depth].data;
						tbrush_cnt = 0;
					}
				}
				saved_depth = astroke_depth;
			}

		
			r_s_painting_area[astroke_depth] += st_w_size*st_h_size;
			//int st_w_size_half = st_w_size / 2;
			//int st_h_size_half = st_h_size / 2;

			if (astroke_depth > MAX_DEPTH) {
				cerr << "astroke depth >" << MAX_DEPTH << " : " << (*Stroke_it)->depth << endl;
				r_cout << "astroke depth >" << MAX_DEPTH << " : " << (*Stroke_it)->depth << endl;

				continue;
			}

			if (brush_size[astroke_depth] < 0) {
				cerr << "m_brush[" << astroke_depth << "]" << brush_size[astroke_depth] << endl;
				r_cout << "m_brush[" << astroke_depth << "]" << brush_size[astroke_depth] << endl;
			//	r_cout << "stroke =" << (*Stroke_it)->no << endl;
				for (int i = 0; i < render_depth; i++) {
					r_cout << "1 " << i << ", " << brush_size[i] << endl;
					cerr << i << ", " << brush_size[i] << endl;

				}
				continue;
			}
#ifdef _DEBUG_RENDER
			rectangle_canvas(painting_area_canvas[astroke_depth], Rect(St_srtPoint, St_endPoint), Scalar(0, 0, 255));//RED QT outline
#endif
			brush_area_h_size = brush_size[astroke_depth];// brush size(painting area) per each depth
			brush_area_w_size = brush_size[astroke_depth];// brush size(painting area) per each depth
			int brush_area_h_size_half = brush_area_h_size / 2 + brush_area_h_size%2;
			int brush_area_w_size_half = brush_area_w_size / 2 + brush_area_w_size % 2;
			 times = ((st_w_size * st_h_size) /
				(brush_area_h_size * brush_area_w_size));
			 if (times == 0) times = 1;
			
				 paint_area_brush_count = times*g_paint_try_scale[astroke_depth];
/*
			if (paint_area_brush_count == 0) {
				r_cout << "bc == 0 d: " << astroke_depth
					<< " st_size : " << st_w_size << ", " << st_h_size <<"= "<<st_w_size*st_h_size<<
					"brush area size= " <<
					(brush_area_h_size * brush_area_w_size) << " try_Scale " <<
					g_paint_try_scale[astroke_depth] << " : "<<times<<
					" " 
					<< " m_brush_size " << brush_size[astroke_depth] <<
					" , bsc :" <<g_brush_scale[astroke_depth]<<
					endl;
				paint_area_brush_count = 2;
				//size_mismatch++;

			}
			*/
			Rect depth_ROI_rect(Point(St_srtPoint.x+canvas_bezel_size, St_srtPoint.y + canvas_bezel_size),
				Size(st_w_size, st_h_size));
			Mat depth_map_8UC1_ROI = depth_map_8UC1(depth_ROI_rect);
			depth_map_8UC1_ROI = astroke_depth;
				uniform_int_distribution<int> dist_x(0, (int)(st_w_size));
				uniform_int_distribution<int> dist_y(0, (int)(st_h_size));
				random_x = new int[paint_area_brush_count];
				random_y = new int[paint_area_brush_count];

				
				for (int painting_try = 0; painting_try < paint_area_brush_count; painting_try++) {
					random_x[painting_try] = dist_x(rand_x[astroke_depth]);
					random_y[painting_try] = dist_y(rand_y[astroke_depth]);
				}

				r_s_grid_painting_try[astroke_depth] += paint_area_brush_count ;//
			
			for (render_try = 0; render_try < paint_area_brush_count; render_try++)
			{

			//	int random_x;
			//	int random_y;
				bool modified = true;
				int retry_cnt = 1;
				int adjust_h_size;
				int adjust_w_size;
					fetch_color_Point.x = St_srtPoint.x + random_x[render_try];
					fetch_color_Point.y = St_srtPoint.y + random_y[render_try];

				int fetch_Index = (fetch_color_Point.x + fetch_color_Point.y*image_width)*image_channels;
				int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*image_width);

				int canvas_Index = ((fetch_color_Point.x + canvas_bezel_size)
					+ (fetch_color_Point.y + canvas_bezel_size)*(image_width + canvas_bezel_size))*image_channels;

			
				int color_BGR_B, color_BGR_G, color_BGR_R;
				p_peek(m_srcData, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

				p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);

				if (brush_area_w_size_half > canvas_bezel_size)
					adjust_w_size = canvas_bezel_size;
				else
					adjust_w_size = brush_area_w_size_half;

				if (brush_area_h_size_half > canvas_bezel_size)
					adjust_h_size = canvas_bezel_size;
				else
					adjust_h_size = brush_area_w_size_half;

				centered_SrtPoint.x = fetch_color_Point.x - adjust_w_size;
				centered_SrtPoint.y = fetch_color_Point.y - adjust_h_size;

				centered_EndPoint.x = fetch_color_Point.x +(brush_area_w_size-brush_area_w_size_half);// to overcome odd brush size
				centered_EndPoint.y = fetch_color_Point.y +(brush_area_h_size-brush_area_h_size_half);// to overcome odd brush size

	
			//	Point Point_X, Point_Y;
			
				canvas_centered_SrtPoint.x = centered_SrtPoint.x + canvas_bezel_size;
				canvas_centered_SrtPoint.y = centered_SrtPoint.y + canvas_bezel_size ;
				canvas_centered_EndPoint.x = centered_EndPoint.x + canvas_bezel_size;
				canvas_centered_EndPoint.y = centered_EndPoint.y + canvas_bezel_size ;
				Rect canvas_centered_ROI_rect = Rect(canvas_centered_SrtPoint, canvas_centered_EndPoint);

#ifdef _DEBUG_RENDER
				rectangle(painting_area_canvas[astroke_depth], canvas_centered_ROI_rect, Scalar(0, 0, 0));//BLACK
#endif
				src_ROI_canvas = src_canvas(canvas_centered_ROI_rect);
				src_ROI_canvas_clone = src_canvas(canvas_centered_ROI_rect).clone();
			//	debug_image("p" + to_string(astroke_depth) + "/src_ROI_canvas_" + m_tag_ + to_string(painting_try), src_ROI_canvas);

				Mat before_canvas_ROI = rst_accu_canvas[astroke_depth](canvas_centered_ROI_rect);
				Mat before_canvas_ROI_clone = rst_accu_canvas[astroke_depth](canvas_centered_ROI_rect).clone();

				Mat changed_canvas_ROI = rst_accu_canvas[astroke_depth](canvas_centered_ROI_rect);
				Mat changed_canvas_ROI_clone = rst_accu_canvas[astroke_depth](canvas_centered_ROI_rect).clone();

				Mat ing_ROI_canvas = ing_canvas[astroke_depth](canvas_centered_ROI_rect);

				//Mat ing_ROI_clone = ing_canvas[astroke_depth](canvas_centered_ROI_rect).clone();
				string f_name = "p" + to_string(astroke_depth) + "/" + m_t_ + to_string(astroke_depth) + "_t_" + to_string(render_try);
				
				int result = P_Rendering(
					src_ROI_canvas_clone,
					before_canvas_ROI_clone,
					changed_canvas_ROI_clone,
					ing_ROI_canvas,

					fetch_color_Point, centered_SrtPoint,
					canvas_centered_SrtPoint, canvas_centered_EndPoint,
					brush_area_w_size, brush_area_h_size,

					astroke_depth, render_try,
					color_BGR_B, color_BGR_G, color_BGR_R,
					astroke_depth,
					render_try,
					changed_canvas_ROI,
					rst_accu_canvas_data[astroke_depth],
					ing_canvas_data[astroke_depth]
				);
			
				//	current_fetched_map_data,fetched_color_data,r,g,b);

				if (result == BEFORE_BETTER) {//result==0
				//	beforeImg_canvas.copyTo(rst_accu_canvas[astroke_depth]);
				//	rst_accu_canvas_data[astroke_depth] = (unsigned char*)rst_accu_canvas[astroke_depth].data;
				//	Nothing to do
				}
				else {//result==1/CHANGED_BETTER, 
					//changed_canvas_ROI.copyTo(before_canvas_ROI);
				//	ing_canvas[astroke_depth](canvas_centered_ROI_rect) = ing_ROI_canvas;
					r_s_changed_count[astroke_depth]++;
					//rst_accu_canvas[astroke_depth](canvas_centered_ROI_rect)=changed_canvas_ROI;
				

#ifdef _DEBUG_RENDER
					rectangle(changed_map_canvas[astroke_depth], canvas_centered_rect, Scalar(0, 0, 255));//RED
#endif					
				}
				if (tbrush_cnt < DEBUG_BRUSH_CNT) {
					debug_image(f_name + "_4ING_ING_", ing_canvas[astroke_depth]);
					debug_image(f_name + "_5ACC_ACC_", rst_accu_canvas[astroke_depth]);
				}
				tbrush_cnt++;
			}
			//end of retry
			delete[] random_x;
			delete[] random_y;
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
		r_cout << setw(10) << m_tag << " :depth " << setw(3) << uu_depth << " : "
			<<setw(5)<< mm_aStroke_set[uu_depth].stroke_list.size()
			<<setw(5)<<times<<"times, paint scale : " <<
			setw(3) << g_paint_try_scale[uu_depth] << " , Br_size " << 
			setw(5) << mm_aStroke_set[uu_depth].stroke_list.size() <<
			", "<<setw(5)<<g_brush_scale[uu_depth]<<
			" br_cnt " << setw(4) << paint_area_brush_count <<
			endl;
		r_cout << "try cnt "<< r_s_grid_painting_try[uu_depth]<< " changed count = " << r_s_changed_count[uu_depth];
		r_cout << " ratio = " << setw(7)<<std::setprecision(2)<<((float)r_s_changed_count[uu_depth] / (float)r_s_grid_painting_try[uu_depth])*100.0 << endl;
		g_file_clog << m_tag << " ," << setw(3) << uu_depth << " ,size, " << mm_aStroke_set[uu_depth].stroke_list.size() <<
			 " paint scale : " << setw(3) << g_paint_try_scale[uu_depth] <<
			"," << setw(4) << paint_area_brush_count 
			<< endl;
		debug_image("ing/_i_" + to_string(uu_depth) + m_t +to_string(g_paint_try_scale[uu_depth]),  ing_canvas[uu_depth]);

#ifdef _DEBUG_RENDER
		debug_image("ing/pa_" + to_string(uu) + "_" + m_tag + to_string(called), painting_area_canvas[uu_depth]);

		debug_image("ing/ch_" + m_tag + to_string(called), uu, changed_map_canvas[uu_depth]);

#endif

		debug_image("ing/_ac_" + to_string(uu_depth) + m_t , rst_accu_canvas[uu_depth]);
		debug_image("ing/try_" + to_string(uu_depth) + m_t , r_try_map_1c[uu_depth]);
		paint_map_accu_8UC1[uu_depth] += paint_map_8UC1[uu_depth];
		paint_map_accu_8UC1[uu_depth+1] = paint_map_accu_8UC1[uu_depth];
		string tag = "p_map_" + m_tag+to_string(uu_depth);
		func_p_map(paint_map_8UC1[uu_depth],tag);
		tag = "p_map_a_" + m_tag + to_string(uu_depth);
		func_p_map(paint_map_accu_8UC1[uu_depth],tag);
		
	}//end of for depth uu
	

//	int ret = draw_grid_2(rst_accu_canvas[render_depth-1].clone(), mm_aStroke_set, string("rst_") + m_tag, render_depth, -1, 255, m_tag);
	//ret = draw_grid_2(r_try_map_1c[render_depth-1], mm_aStroke_set, string("try_") + m_tag, render_depth, -1, 255, m_tag);
//	r_cout << "Merge SKIP count : " << g_merge_skip_count << endl;
	//r_cout << " depth : " << render_depth << endl;
	//r_cout << "size mismatch " << size_mismatch << endl;
	
	result_image=rst_accu_canvas[render_depth-1];
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
	int i_minloc, i_maxloc;
	int i_minval, i_maxval;
	cv::minMaxLoc(depth_map_8UC1, &minval, &maxval);// , &i_minloc, &i_maxloc);
	i_minval = (int)minval;
	i_maxval = (int)maxval;

	int diff = i_maxval - i_minval;
	int step = (int)256 / diff;
	Mat p_map;
	depth_map_8UC1.convertTo(p_map, -1, step);

	debug_image("depth_map"+m_tag, p_map);
		return 0;

}
