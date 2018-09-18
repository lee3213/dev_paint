#include "stdafx.h"
#include "RenderingImage.h"
#include"image_util.h"
#include "debug_image.h"
#include "p_rendering.h"


#include "draw_grid2.h"






cv::Rect safe_Rect_margin(Point s, Point e, int i_width, int i_height, Point &S, Point &E) {
	Rect rr;
/*	if (s.x < 0) { S.x = s.x; s.x = 0;
	}
	if (s.x >= i_width) { S.x = s.x - i_width; 
	s.x = i_width - 1; 
	}
	if (e.x < 0) {
		E.x = e.x;
		e.x = 0;
	}
	if (e.x >= i_width) {
		E.x = e.x - i_width;
		e.x = i_width - 1;
	}

	if (s.y < 0) {
	 S.y = s.y;	
	 s.y = 0;
	}
	if (s.y >= i_height) {
		S.y = s.y - i_height;
		s.y = i_height - 1;
	}
	if (e.y < 0) {
		E.y = e.y;
		e.y = 0;
	}
	if (e.y >= i_height) {
		E.y = e.y - i_height;
		e.y = i_height - 1;
	}
	*/
	rr = Rect(s, e);
	return rr;
}

bool fetch_in_box(Point &s, int i_width, int i_height) {

	Rect rr;
	if (s.x < 0) return false;
	if (s.x >= i_width) return false;


	if (s.y < 0) return false;
	if (s.y >= i_height) return false;
	return true;

}

void ChkIsInside(Point &bSrtPoint, Point &bEndPoint, Point &tryPoint, int bsize, int width, int height)
{
	if (bSrtPoint.x < 0)
	{
		bSrtPoint.x = 0;
	//	tryPoint.x = (int)(bsize * 0.5) - 1;
	//	bEndPoint.x = bsize - 1;
	}
	if (bSrtPoint.x + bsize > width)
		bEndPoint.x = width - 1;

	 if (bEndPoint.x >= width)
	{
	//	bSrtPoint.x = width - bsize - 1;
	//	tryPoint.x = width - (int)(bsize * 0.5) - 1;
		bEndPoint.x = width - 1;
	}

	if (bSrtPoint.y < 0)
	{
		bSrtPoint.y = 0;
	//	tryPoint.y = (int)(bsize * 0.5) - 1;
		//bEndPoint.y = bsize - 1;
	}
	if (bSrtPoint.y + bsize > height)
		bEndPoint.y = height - 1;
	if (bEndPoint.y >= height)
	{
		//bSrtPoint.y = height - bsize - 1;
	//	tryPoint.y = height - (int)(bsize * 0.5) - 1;
		bEndPoint.y = height - 1;
	}
}







cv::Mat PainterlyRendering(cv::Mat &srcImg,  vector<QuadTree::Img_node*> aStroke_set, string tag,
	int _m_brush_size[], int _depth,Mat _grid_map[], int _Sgrid_painting_try[], list<Brush> &_brush_set,
	int _QT_grid_count[], int _changed_count[], long int  _painting_area[]) {

	int image_width = srcImg.size().width;
	int image_height = srcImg.size().height;
	int image_channels = srcImg.channels();
	int image_step1 = (int)srcImg.step1();

	//string tag = g_method;
	Point St_srtPoint, St_endPoint, fetch_color_Point, centered_SrtPoint, centered_EndPoint;
	int paint_grid_w_size, paint_grid_h_size;

	//cv::Mat bestBrush;
//	int density;

	random_device rand_x[5];
	random_device rand_y[5];

#ifdef G_DEBUG_TOUCH
	g_touch.create(image_height, image_width, CV_8UC3);
	g_touch_data = g_touch.data;
	PaintBackGround(g_touch, 0, 0, 0);
#endif
	//String f_path_t;
	//String f_path3;
	int stroke_no;
	int ing = 0;
	int saved_depth = -1;
//	cv::String f_path;
	int nth ;
//	std::string p;

	nth = 0;

	//saved_depth = -1;
	//int brush_no;
//	bool debug_level;
	int painting_count;
	int astroke_depth;
	int process_ratio;
	int saved_process_ratio = -1;
	
//	int rand_x, rand_y;
	//int bmax = g_BrushMaxSize, bmin = g_BrushMinSize;

	cv::Mat ing_canvas;
	unsigned char* ing_canvas_data;
//	for (int i = 0; i < _depth; i++) {
	//	cout << "brush_size"<<i << ", " << m_brush_size[i] << endl;
//	}

	cv::Mat rstImg;
	rstImg.create(image_height, image_width, CV_8UC3);
	PaintBackGround(rstImg, 255, 255, 255);
	unsigned char* rstData = (unsigned char*)rstImg.data;
	unsigned char* srcData = (unsigned char*)srcImg.data;
	cv::Mat St_painting_area_canvas;
	unsigned char * St_painting_area_data;
	St_painting_area_canvas.create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
	St_painting_area_data = St_painting_area_canvas.data;
	PaintBackGround(St_painting_area_data, g_canvas_size_width, g_canvas_size_height, 255, 255, 255);

	cv::Mat Canvas;
	unsigned char * Canvas_data;
	Canvas.create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
	Canvas_data = Canvas.data;
	PaintBackGround(Canvas, 255, 255, 255);
	
	Mat beforeImg;
	//unsigned char* rstData;// = (unsigned char*)rstImg.data;
	//unsigned char* srcData = (unsigned char*)srcImg.data;



#ifdef G_DEBUG_TOUCH
	unsigned char * current_fetched_map_data;
#endif
	

	
#ifdef G_DEBUG_TOUCH
	cv::Mat fetched_map[MAX_DEPTH]; // randominized fetched point
#endif
	cv::Mat try_map_1c[MAX_DEPTH]; // randominized try grid map
	unsigned char * current_try_map_data_1c;
	cv::Mat changed_map_canvas[MAX_DEPTH];
	//int try_count[MAX_DEPTH];
	//int changed_count[MAX_DEPTH]			;

	//	uniform_int_distribution<int> distribution(0, m_brush_size[saved_depth]);	// 생성 범위
	for (astroke_depth = 0; astroke_depth < _depth; astroke_depth++) {
			
	
#ifdef G_DEBUG_TOUCH
		fetched_map[astroke_depth].create(image_height, image_width, CV_8UC3);
		PaintBackGround(fetched_map[astroke_depth], 0, 0, 0);
#endif

		try_map_1c[astroke_depth].create(image_height,image_width,CV_8UC1);
		try_map_1c[astroke_depth].setTo(255);
			
		changed_map_canvas[astroke_depth].create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
		changed_map_canvas[astroke_depth].setTo(255);
	
		_Sgrid_painting_try[astroke_depth] = 0;
	//	try_count[astroke_depth] = 0;
		_changed_count[astroke_depth] = 0;
	
	}
	
#ifdef DEBUG_ASTROKE_REGION
	//prepare 
	debug_image_single *astroke_region = new debug_image_single(Size(rstImg.size().width,rstImg.size().height),
		(int)aStroke.size(), CV_8UC3);

	p.append(format("%s/st/st_%s_",g_para_method_path.c_str(),tag.c_str()));
	cout << p.c_str() << endl;
	astroke_region->set_file_format(p);

#endif

	ing_canvas.create(g_canvas_size_height,g_canvas_size_width,CV_8UC3);
	ing_canvas_data = (unsigned char*)ing_canvas.data;
	PaintBackGround(ing_canvas, 255, 255, 255);

	//int level_try = 0;
	//cv::Mat level_cropped;
	saved_depth = -1;
	//f_path3 = format("%s/ing/ing_%s_%02d_i%04d.ppm", g_para_method_path.c_str(), tag.c_str(), saved_depth, ing);
	//cv::imwrite(f_path3, debug_image_depth);
	//------------------------------------------------------------------------------------------------------------//

	for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set.begin(); St_it != aStroke_set.end(); St_it++, ing++)
	{
		int st_w_size, st_h_size;
		St_srtPoint.x = (*St_it)->info.srtPoint.x, St_srtPoint.y = (*St_it)->info.srtPoint.y;
		St_endPoint.x = (*St_it)->info.endPoint.x, St_endPoint.y = (*St_it)->info.endPoint.y;

		astroke_depth = (*St_it)->depth;
		stroke_no = (*St_it)->no;

		st_w_size = St_endPoint.x - St_srtPoint.x;
		st_h_size = St_endPoint.y - St_srtPoint.y;

		_painting_area[astroke_depth] += st_w_size*st_h_size;
		//int st_w_size_half = st_w_size / 2;
		//int st_h_size_half = st_h_size / 2;

		if (astroke_depth > MAX_DEPTH) {
			cerr << "astroke depth >" << MAX_DEPTH <<" : "<<stroke_no<< endl;
			cout << "astroke depth >" << MAX_DEPTH << " : " << stroke_no << endl;

			continue;
		}
		if (_m_brush_size[astroke_depth] < 0) {
			cerr << "m_brush[" << astroke_depth << "]" << _m_brush_size[astroke_depth]<<endl;
			cout << "m_brush[" << astroke_depth << "]" << _m_brush_size[astroke_depth] << endl;
			cout << "stroke =" << stroke_no << endl;
			for (int i = 0; i < _depth; i++) {
				cout << i << ", " << _m_brush_size[i] << endl;
				cerr << i << ", " << _m_brush_size[i] << endl;

			}
			continue;
		}
		paint_grid_h_size = _m_brush_size[astroke_depth];// brush size(painting area) per each depth
		paint_grid_w_size = _m_brush_size[astroke_depth];// brush size(painting area) per each depth
		int paint_grid_h_size_half = paint_grid_h_size / 2;
		int paint_grid_w_size_half = paint_grid_w_size / 2;

		int paint_grid_count = (st_w_size * st_h_size *g_paint_grid_scale) /
			(_m_brush_size[astroke_depth] * _m_brush_size[astroke_depth]);

		if (paint_grid_count == 0) {
			cout << "paint_grid_count == 0 depth " << astroke_depth << " st_size : " << st_w_size << ", " << st_h_size << " g_paint_grid_Scale " <<
				g_paint_grid_scale << " m_brush_size " << _m_brush_size[astroke_depth] << endl;
			paint_grid_count = 4;
		}
	
	
		rectangle_canvas(St_painting_area_canvas, Rect(St_srtPoint, St_endPoint), Scalar(0, 0, 255));//RED QT outline

		
	
		if (saved_depth != astroke_depth )// new depth
		{	
			if (saved_depth != -1) {
				debug_image("ing/painting_area" + to_string(saved_depth) + "_" + tag,  St_painting_area_canvas);
				debug_image("ing/ing_" + to_string(saved_depth) + "_" + tag, ing_canvas);
				debug_image("ing/accu_" + to_string(saved_depth) + "_" + tag, rstImg);
				debug_image("ing/try_map_" + to_string(saved_depth) + "f_" + tag,  try_map_1c[saved_depth]);
			
				PaintBackGround(St_painting_area_data, g_canvas_size_width, g_canvas_size_height, 255, 255, 255);
				PaintBackGround(ing_canvas, 255, 255, 255);
	
				int ret = draw_grid_2(rstImg.clone(), aStroke_set,"rst_"+ tag, _depth,saved_depth,255);
				ret = draw_grid_2(try_map_1c[saved_depth], aStroke_set, "try_"+tag, _depth, saved_depth,255);
		}
			//level_try = 0;
			current_try_map_data_1c = try_map_1c[astroke_depth].data;
		
		}
	
	//	level_try++;
		saved_depth = astroke_depth;
	

		process_ratio = (int)(ing / (double)aStroke_set.size() * 100.);
	


#ifdef DEBUG_ASTROKE_REGION
		astroke_region->draw_rectangle(b,srtPoint,endPoint);
#endif
		
		 //srand((unsigned)time(NULL));
		
		

		 //g_brush_debug_setup(Size(bsize,bsize), g_Sgrid_painting_try[astroke_depth], astroke_depth);
		 //int first_try = 0;
		
		 uniform_int_distribution<int> dist_x(0, (int)(st_w_size));
		 uniform_int_distribution<int> dist_y(0, (int)(st_h_size));

		// cout << astroke_depth << ": " <<  k_x.min() << ", " << k_x.max() <<
		//	 ", "<<k_y.min() << ", " << k_y.max() << endl;

		 for (painting_count = 0; painting_count < paint_grid_count; painting_count++)
		 {

			 //	cout << bestBrush.size()<< endl;

			// try_count[astroke_depth]++;
//			 string f_path_a = format("%s/p%d/%s_d%d_s%d_t%d_", g_para_method_path.c_str(), astroke_depth, tag.c_str(), astroke_depth, stroke_no, painting_count);

#ifdef DEBUG_SELECTED_BRUSH
			 selected_brush->image_add(bestBrush);
#endif

			 int random_x;
			 int random_y;

			 //		= *g_distribution[astroke_depth];
			 //	auto random_x_bind = bind(k, rd);
				 //g_engine_x[astroke_depth] );
				 //auto random_y_bind = bind(k, rd);
				 //g_engine_y[astroke_depth]);
			 bool in_out = true;
			 int retry_cnt = 1;
			 do {
				 random_x = dist_x(rand_x[astroke_depth]);
				 random_y = dist_y(rand_y[astroke_depth]);
				 //clog << random_x << ", " << random_y << endl;

				 fetch_color_Point.x = St_srtPoint.x + random_x;
				 fetch_color_Point.y = St_srtPoint.y + random_y;

				 in_out = fetch_in_box(fetch_color_Point, image_width, image_height);


				 if (in_out == false) {
					 cout << "error in_out" +tag<< endl;

					 //skip rest of do block to reduce computing

					 retry_cnt++;
				 }
				 if (retry_cnt > 20)in_out = true;
			 } while (in_out == false);

			 _Sgrid_painting_try[astroke_depth] ++;//

			 //	proof_box(fetch_color_Point, image_width, image_height);
				 //int cIndex = 
			 int fetch_Index = (fetch_color_Point.x + fetch_color_Point.y*image_width)*image_channels;
			 int fetch_Index_1c = (fetch_color_Point.x + fetch_color_Point.y*image_width);
			// int canvas_Index = ((fetch_color_Point.x + g_canvas_size_bezel_size)
		//		 + (fetch_color_Point.y + g_canvas_size_bezel_size)*(image_width + g_canvas_size_bezel_size))*image_channels;
			 //int  fetch_Index = fetch_color_Point.y * image_step1 + fetch_color_Point.x * image_channels;
		//	 unsigned char* rstData = (unsigned char*)rstImg.data;
			// unsigned char* srcData = (unsigned char*)srcImg.data;

			 int color_BGR_B, color_BGR_G, color_BGR_R;
			 p_peek(srcData, fetch_Index, color_BGR_B, color_BGR_G, color_BGR_R);

			 p_poke(current_try_map_data_1c, fetch_Index_1c, 255);
			
			
			 centered_SrtPoint.x = fetch_color_Point.x - paint_grid_w_size_half; 
			 centered_SrtPoint.y = fetch_color_Point.y - paint_grid_w_size_half;
			 centered_EndPoint.x = fetch_color_Point.x + paint_grid_h_size_half;
			 centered_EndPoint.y = fetch_color_Point.y + paint_grid_h_size_half;


			 //시작점 및 종료점이 이미지 밖에있는지 확인
		 //	ChkIsInside(bSrtPoint, bEndPoint, fetch_color_Point, bsize, image_width, image_height);		
			 Point Point_X, Point_Y;
			 Rect canvas_rect = Rect(centered_SrtPoint, centered_EndPoint);

			 rectangle_canvas(St_painting_area_canvas, canvas_rect, Scalar(0, 0, 0));//BLACK

			 //changed source location by cwlee 18. 7. 24
			 //한 Stroke->info의 크기가 한 Grid의 Size
		 //	Rect rect(centered_SrtPoint, centered_EndPoint);

			 //cv::Mat brush_choice_Img_src, brush_choice_Img_gray, brush_choice_Img;
			 /*
			 brush_choice_Img_src.create(paint_grid_w_size,paint_grid_h_size,CV_8UC3);
			 brush_choice_Img.setTo(255);
		 //		= srcImg(safe_rect).clone();
			 unsigned char * brush_choice_Img_src_data = brush_choice_Img_src.data;
			 cv::cvtColor(brush_choice_Img_src, brush_choice_Img_gray, COLOR_RGB2GRAY);

			 cv::resize(brush_choice_Img_gray, brush_choice_Img, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));
		 */
			 Rect rect = safe_Rect(centered_SrtPoint, centered_EndPoint,image_width,image_height);

			 cv::Mat testImg_src = srcImg(rect).clone();
		//	 mat_print(testImg_src, "testImg_Src", first_try);
			 Mat testImg_resized;
			 resize(testImg_src, testImg_resized, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));
			 cv::cvtColor(testImg_resized, testImg_resized, COLOR_RGB2GRAY);

			

#ifdef DEBUG_ASTROKE_REGION
			astroke_region->draw_rectangle(b,bSrtPoint,bEndPoint);
#endif
			//렌더링
			
			beforeImg = rstImg.clone();
		
			unsigned char *beforeData = beforeImg.data;

			int result = P_Rendering(//srcImg, 
				srcData, rstData, beforeImg, testImg_resized, _brush_set,
				fetch_color_Point, centered_SrtPoint, 
				paint_grid_w_size, paint_grid_h_size, tag,
				stroke_no, ing, astroke_depth, painting_count, color_BGR_B, color_BGR_G, color_BGR_R,
				ing_canvas_data);

			//	current_fetched_map_data,fetched_color_data,r,g,b);

			if (result == BEFORE_BETTER) {//result==0
				beforeImg.copyTo(rstImg);
				 rstData = (unsigned char*)rstImg.data;

			}
			else {//result==1
				_changed_count[astroke_depth]++;//CHANGED_BETTER 
				Rect safe_rect = Rect(centered_SrtPoint, centered_EndPoint);
				rectangle_canvas(changed_map_canvas[astroke_depth],safe_rect, Scalar(0, 0, 255));//RED
			}
		
			beforeImg.release();
			testImg_src.release();
			testImg_resized.release();
		}
		//end of retry

	/*	
		if (!(ing % 5)) {
			f_path_t = format("%s/ing/%s_%03d_%03d_rst.ppm", g_para_method_path.c_str(),tag.c_str(), astroke_depth, ing);
		
			cout << "f_path " << f_path_t.c_str() << endl;
			cv::imwrite(f_path_t, rstImg);
		}
		*/

#ifdef DEBUG_SELECTED_BRUSH
		selected_brush->image_final();
		delete selected_brush;
#endif
#ifdef DEBUG_ASTROKE_REGION
		astroke_region->next_roi();
#endif
#ifdef DEBUG_IMAGE_GRID
		_debug_image_grid->image_final();
		_debug_image_grid_resized->image_final();
#endif	
	}//end of it astroke


	//g_debug_brush->image_final_split(50);

	debug_image("ing/painting_area"+ to_string(saved_depth)+"f_"+tag, St_painting_area_canvas);
	debug_image("ing/ing_"+ to_string(saved_depth)+"f_"+tag, ing_canvas);
	debug_image("ing/accu_"+ to_string(saved_depth)+"f_"+tag,  rstImg);
	debug_image("ing/try_map_"+ to_string(saved_depth)+"f_"+tag,  try_map_1c[saved_depth]);
	
	int ret = draw_grid_2(rstImg.clone(), aStroke_set, "rst_"+tag, _depth, saved_depth,255);
	ret = draw_grid_2(try_map_1c[saved_depth], aStroke_set, "try_" + tag, _depth, saved_depth,255);

#ifdef G_DEBUG_TOUCH

	f_path_t = format("%s/ing/t_%s_d%03d_g%03d_i%03d_touch.ppm", g_para_method_path.c_str(), tag.c_str(), saved_depth, g_Sgrid_painting_try[astroke_depth], ing);
	//cout << "f_path " << f_path_t.c_str() << endl;
	cv::imwrite(f_path_t, g_touch);
#endif

#ifdef DEBUG_ASTROKE_REGION
	astroke_region->image_final_split(10);
#endif
	
	//Mat fetched_color;
	//unsigned char * fetched_color_data;

	//fetched_color.create(image_height, image_width, CV_8UC3);
	//fetched_color_data = (unsigned char *)fetched_color.data;
	//fetched_color.setTo(0);
	cout<< "Merge SKIP count : "<< g_merge_skip_count <<endl;
	cout << "Merge_method : " << g_merge_method<<endl;

	//debug_image("fetched_color_"+tag, _depth, fetched_color);
	//overlap(fetched_color, _grid_map[_depth-1]);
	//debug_image("fetched_grid_F_"+tag, fetched_color);
	for (int i = 0; i < _depth; i++) {
		

		debug_image("ing/changed_" + tag, i, changed_map_canvas[i]);
	}
#ifdef G_DEBUG_TOUCH
	f_path_t = format("%s/ing/%s_d%03d_g%03d_i%03d_touch.ppm", g_para_method_path.c_str(), tag.c_str(), saved_depth, g_Sgrid_painting_try[astroke_depth], ing);
	cv::imwrite(f_path_t, g_touch);
#endif

	return rstImg;
}