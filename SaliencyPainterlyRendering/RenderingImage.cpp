#include "stdafx.h"
#include "RenderingImage.h"
#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"

#include "render_.h"

#include "time.h"




int   render_::PainterlyRendering()

{

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
	Rect src_Rect_full(Point(canvas_size_bezel_size, canvas_size_bezel_size),
		Size(g_src_image_width, g_src_image_height));
	Mat src_ROI_canvas_full = src_canvas(src_Rect_full);
	m_srcImg_.copyTo(src_ROI_canvas_full);
	debug_image("src_canvas", src_canvas);
	debug_image("src_canvas_ROI", src_ROI_canvas_full);

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
	int first_layer = -1;
	for(int i=0;i<mm_depth;i++)
		r_cout << m_tag << " : "<<mm_depth<<" size: "<< mm_aStroke_set[i].size()<<endl;
//	for (int i = 0; i < mm_depth; i++) {
//		r_cout << "0 "<<i << ", " << brush_size[i] << endl;
//	}

//	saved_depth = -1;

	int size_mismatch = 0;
	int saved_depth=-1;
	
	for (int uu = 0; uu < mm_depth; uu++) {
		
		int paint_area_brush_count;
		if (uu < g_first_layer) {
			if (uu >= 1) {
				rst_accu_canvas[uu] = rst_accu_canvas[uu - 1].clone();
				rst_accu_canvas_data[uu] = (unsigned char*)rst_accu_canvas[uu].data;
			}
			continue;
		}

		for (list<Img_node*>::iterator St_it = mm_aStroke_set[uu].begin(); St_it != mm_aStroke_set[uu].end(); St_it++)
		{
			int st_w_size, st_h_size;
			int astroke_depth;
			St_srtPoint.x = (*St_it)->info.srtPoint.x, St_srtPoint.y = (*St_it)->info.srtPoint.y;
			St_endPoint.x = (*St_it)->info.endPoint.x, St_endPoint.y = (*St_it)->info.endPoint.y;

			astroke_depth = (*St_it)->depth;
			//stroke_no = (*St_it)->no;
			if (astroke_depth != saved_depth) {
				if (astroke_depth != 0) {
					if (saved_depth > astroke_depth) {
						r_cout << "<< " << m_tag << " " << saved_depth << " : " << astroke_depth << endl;
						return -1;
					}
					else {
						rst_accu_canvas[astroke_depth] = rst_accu_canvas[astroke_depth - 1].clone();
						rst_accu_canvas_data[astroke_depth] = (unsigned char*)rst_accu_canvas[astroke_depth].data;
					}
				}
				saved_depth = astroke_depth;
			}
			st_w_size = St_endPoint.x - St_srtPoint.x;
			st_h_size = St_endPoint.y - St_srtPoint.y;
			r_s_painting_area[astroke_depth] += st_w_size*st_h_size;
			//int st_w_size_half = st_w_size / 2;
			//int st_h_size_half = st_h_size / 2;

			if (astroke_depth > MAX_DEPTH) {
				cerr << "astroke depth >" << MAX_DEPTH << " : " << (*St_it)->no << endl;
				r_cout << "astroke depth >" << MAX_DEPTH << " : " << (*St_it)->no << endl;

				continue;
			}
			if (brush_size[astroke_depth] < 0) {
				cerr << "m_brush[" << astroke_depth << "]" << brush_size[astroke_depth] << endl;
				r_cout << "m_brush[" << astroke_depth << "]" << brush_size[astroke_depth] << endl;
				r_cout << "stroke =" << (*St_it)->no << endl;
				for (int i = 0; i < mm_depth; i++) {
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
			
				paint_area_brush_count = (st_w_size * st_h_size *g_paint_area_scale[astroke_depth]) /
					(brush_area_h_size * brush_area_w_size);
		

			if (paint_area_brush_count == 0) {
				r_cout << "paint_area_brush_count == 0 depth " << astroke_depth
					<< " st_size : " << st_w_size << ", " << st_h_size << " g_paint_area_Scale " <<
					g_paint_area_scale[astroke_depth] << (st_w_size * st_h_size *g_paint_area_scale[astroke_depth]) << " m_brush_size " << brush_size[astroke_depth] <<
					" : " <<
					(brush_area_h_size * brush_area_w_size) <<
					endl;
				paint_area_brush_count = 2;
				size_mismatch++;

			}
			
				uniform_int_distribution<int> dist_x(0, (int)(st_w_size));
				uniform_int_distribution<int> dist_y(0, (int)(st_h_size));
				random_x = new int[paint_area_brush_count];
				random_y = new int[paint_area_brush_count];

				for (int painting_count = 0; painting_count < paint_area_brush_count; painting_count++) {
					random_x[painting_count] = dist_x(rand_x[astroke_depth]);
					random_y[painting_count] = dist_y(rand_y[astroke_depth]);
				}

			for (int painting_count = 0; painting_count < paint_area_brush_count; painting_count++)
			{

			//	int random_x;
			//	int random_y;
				bool modified = true;
				int retry_cnt = 1;
				
				
				
					fetch_color_Point.x = St_srtPoint.x + random_x[painting_count];
					fetch_color_Point.y = St_srtPoint.y + random_y[painting_count];

				

				r_s_grid_painting_try[astroke_depth] ++;//

				//	proof_box(fetch_color_Point, image_width, image_height);
					//int cIndex = 
				int fetch_Index = (fetch_color_Point.x + fetch_color_Point.y*image_width)*image_channels;
				int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*image_width);

				int canvas_Index = ((fetch_color_Point.x + canvas_size_bezel_size)
					+ (fetch_color_Point.y + canvas_size_bezel_size)*(image_width + canvas_size_bezel_size))*image_channels;

				//int  fetch_Index = fetch_color_Point.y * image_step1 + fetch_color_Point.x * image_channels;
		   //	 unsigned char* rstData = (unsigned char*)rstImg.data;
			   // unsigned char* srcData = (unsigned char*)srcImg.data;

				int color_BGR_B, color_BGR_G, color_BGR_R;
				p_peek(m_srcData, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

				p_poke(r_try_map_1c_data[astroke_depth], fetch_Index_1c, 0);


				centered_SrtPoint.x = fetch_color_Point.x - brush_area_w_size_half;
				centered_SrtPoint.y = fetch_color_Point.y - brush_area_h_size_half;

				centered_EndPoint.x = fetch_color_Point.x +(brush_area_w_size-brush_area_w_size_half);
				centered_EndPoint.y = fetch_color_Point.y +(brush_area_h_size-brush_area_h_size_half);

			//	modified = FixToInside(fetch_color_Point, St_srtPoint, St_endPoint, image_width, image_height, brush_area_h_size);
			//	if (modified) {

			//	}

				//시작점 및 종료점이 이미지 밖에있는지 확인
			//	ChkIsInside(bSrtPoint, bEndPoint, fetch_color_Point, bsize, image_width, image_height);		
				Point Point_X, Point_Y;
			//	Rect centered_rect = Rect(centered_SrtPoint, centered_EndPoint);

			//	rectangle_canvas(painting_area_canvas[astroke_depth], centered_rect, Scalar(0, 0, 0));//BLACK

			//	Rect src_rect = safe_Rect(centered_SrtPoint, centered_EndPoint, image_width, image_height);
				canvas_centered_SrtPoint.x = centered_SrtPoint.x + canvas_size_bezel_size ;
				canvas_centered_SrtPoint.y = centered_SrtPoint.y + canvas_size_bezel_size ;
				canvas_centered_EndPoint.x = centered_EndPoint.x + canvas_size_bezel_size ;
				canvas_centered_EndPoint.y = centered_EndPoint.y + canvas_size_bezel_size ;
				Rect canvas_centered_rect = Rect(canvas_centered_SrtPoint, canvas_centered_EndPoint);
#ifdef _DEBUG_RENDER
				rectangle(painting_area_canvas[astroke_depth], canvas_centered_rect, Scalar(0, 0, 0));//BLACK
#endif
				cv::Mat src_ROI_canvas = src_canvas(canvas_centered_rect);
				//	 mat_print(testImg_src, "testImg_Src", first_try);
			
				//렌더링

			//	beforeImg_canvas = rst_accu_canvas[astroke_depth].clone();
			//	unsigned char *before_canvas_data = beforeImg_canvas.data;
				Mat before_ROI_canvas = rst_accu_canvas[astroke_depth](canvas_centered_rect);
				Mat changed_ROI_clone = rst_accu_canvas[astroke_depth](canvas_centered_rect).clone();

				Mat ing_ROI_canvas = ing_canvas[astroke_depth](canvas_centered_rect);
				Mat ing_ROI_clone = ing_canvas[astroke_depth](canvas_centered_rect).clone();
				int result = P_Rendering(//srcImg, 
					//srcData, 
					src_ROI_canvas,
					before_ROI_canvas,
					changed_ROI_clone,
					ing_ROI_clone,
				//	rst_accu_canvas_data[astroke_depth],
				//	beforeImg_canvas,
					//testImg_resized,
					fetch_color_Point, centered_SrtPoint,
					canvas_centered_SrtPoint,canvas_centered_EndPoint,
					brush_area_w_size, brush_area_h_size,
					//m_tag,
					//stroke_no, 
					astroke_depth, painting_count, 
					color_BGR_B, color_BGR_G, color_BGR_R				
				);

				//	current_fetched_map_data,fetched_color_data,r,g,b);

				if (result == BEFORE_BETTER) {//result==0
				//	beforeImg_canvas.copyTo(rst_accu_canvas[astroke_depth]);
				//	rst_accu_canvas_data[astroke_depth] = (unsigned char*)rst_accu_canvas[astroke_depth].data;
				//	Nothing to do
				}
				else {//result==1/CHANGED_BETTER, 
					changed_ROI_clone.copyTo(before_ROI_canvas);
					r_s_changed_count[astroke_depth]++;
					rst_accu_canvas[astroke_depth](canvas_centered_rect).clone();
					ing_ROI_clone.copyTo(ing_ROI_canvas);

#ifdef _DEBUG_RENDER
					rectangle(changed_map_canvas[astroke_depth], canvas_centered_rect, Scalar(0, 0, 255));//RED
#endif					
				}

			//	beforeImg_canvas.release();
				//testImg_src.release();
				//testImg_resized.release();
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
		r_cout << setw(15) << m_tag << " :depth " << setw(3) << uu << " paint scale : " <<
			setw(3) << g_paint_area_scale[uu] << " , Br_size " << setw(5) << mm_aStroke_set[uu].size() <<
			" br_cnt " << setw(4) << paint_area_brush_count <<
			endl;
		g_file_clog << m_tag << " ," << setw(3) << uu << " ,size, " << mm_aStroke_set[uu].size() <<
			 " paint scale : " << setw(3) << g_paint_area_scale[uu] <<
			"," << setw(4) << paint_area_brush_count 
			<< endl;
		debug_image("ing/_i_" + to_string(uu) + m_t +to_string(g_paint_area_scale[uu]),  ing_canvas[uu]);
#ifdef _DEBUG_RENDER
		debug_image("ing/pa_" + to_string(uu) + "_" + m_tag + to_string(called), painting_area_canvas[uu]);

		debug_image("ing/ch_" + m_tag + to_string(called), uu, changed_map_canvas[uu]);

#endif

		debug_image("ing/_ac_" + to_string(uu) + m_t , rst_accu_canvas[uu]);
		debug_image("ing/try_" + to_string(uu) + m_t , r_try_map_1c[uu]);

	}//end of for depth uu
	

//	int ret = draw_grid_2(rst_accu_canvas[mm_depth-1].clone(), mm_aStroke_set, string("rst_") + m_tag, mm_depth, -1, 255, m_tag);
	//ret = draw_grid_2(r_try_map_1c[mm_depth-1], mm_aStroke_set, string("try_") + m_tag, mm_depth, -1, 255, m_tag);
//	r_cout << "Merge SKIP count : " << g_merge_skip_count << endl;
	r_cout << " depth : " << mm_depth << endl;
	r_cout << "size mismatch " << size_mismatch << endl;
	for (int i = 0; i < mm_depth; i++) {
		static int called = 0;
	
		called++;
		
	}

	result_image=rst_accu_canvas[mm_depth-1];
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
		return 0;

}
