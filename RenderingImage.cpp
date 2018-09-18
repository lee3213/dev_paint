#include "stdafx.h"
#include "RenderingImage.h"
#include"image_util.h"
#include "debug_image.h"

cv::Mat ColorReversal(cv::Mat &srcImg);
//cv::Mat  PainterlyRendering(cv::Mat &srcImg, vector<QuadTree::Img_node*> aStroke,string tag);
//debug_image_single * debug_image_comp;

void g_brush_debug_setup(Size x, int howmany,int depth,string path)//by cwlee
{
	std::string a;
	a = format("%s/%s.jpg",g_para_method_path.c_str(),path.c_str())
		;
//	a.append("_%d.jpg");
//	g_debug_brush = new debug_image_single(x, howmany,CV_8UC3);
//	g_debug_brush->set_file_format(a);

}





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
void proof_box(Point &s, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0)
		cout << "error 1:" << s.x << endl;
	if (s.x >= i_width)
		cout << "error 2:" << s.x << endl;



	if (s.y < 0) cout << "error 3:" << s.y << endl;

	if (s.y >= i_height)
		cout << "error 4:" << s.y << endl;

}

void proof_box(Point &s, int i_width, int i_height,char * p) {
//Rect rr;
	if (s.x < 0)
		cout << "error 1:" <<p<< s.x << endl;
	if (s.x >= i_width)
		cout << "error 2:" <<p<< s.x << endl;



	if (s.y < 0) cout << "error 3:" <<p<< s.y << endl;

	if (s.y >= i_height)
		cout << "error 4:"  <<p<<s.y << endl;

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





//Rendering(srcImg, rstImg, bestBrush, tryPoint, bSrtPoint, bsize, tag);
int   P_Rendering(cv::Mat srcImg,
	unsigned char * srcData,
	unsigned char * changedData,
	unsigned char * beforeData,
	unsigned char * bestBrush_data,
	cv::Mat bestBrush,

	Point fetch_color_Point, Point centered_SrtPoint, int paint_grid_w_size, int paint_grid_h_size, 
	//String tag,
	int astroke_number, int ing, int astroke_depth, int painting_count, int f_0, int f_1, int f_2,
	unsigned char * debug_image_depth_Data,
	//string f_path_a,
	unsigned char * brush_gray)
//	unsigned char * current_fetched_map_data,unsigned char *fetched_color_data,)
{
	
	//unsigned char* brushData = (unsigned char*)bestBrush.data;

	
	int s_width = srcImg.size().width;
	int s_height = srcImg.size().height;
	int s_step1 = (int)srcImg.step1();
	int s_channels = (int)srcImg.channels();

	int b_step1 = (int)bestBrush.step1();
	int b_channels = bestBrush.channels();

//	unsigned char * srcData = srcImg.data;
//	unsigned char * brushData = bestBrush.data;

	String f_path;
	String f_path2;
//	string f_path_a;
	static int first_time_called = -1;
	
	//f_path_a = format("%s/p%d/%s_d%d_s%d_t%d_", g_para_method_path.c_str(), astroke_depth, tag.c_str(), astroke_depth, astroke_number, painting_count);


	//Point_2p_debug(fetch_color_Point, "fetch",bSrtPoint,"srt");
	proof_box(fetch_color_Point, s_width, s_height);
	int fetch_Index = fetch_color_Point.y * s_step1 + fetch_color_Point.x * s_channels;

	//Mat colored_brush(paint_grid_w_size,paint_grid_h_size, bestBrush.type());
//	unsigned char * colored_brush_ptr = colored_brush.data;
//	PaintBackGround(colored_brush, 0, 0, 0);

#ifdef DEBUG_COLORED_BRUSH
	/*
	*/
	cv::Mat bestBrush_clone;

if (depth_check(astroke_depth)){
	bestBrush.clone = bestBrush.clone();

		f_path = format("%s_2.jpg", f_path_a.c_str());
	//	cv::rectangle(bestBrush_clone, Point(0, 0), Point(bsize - 1, bsize - 1), RGB(0, 255, 0));
	//	cv::imwrite(f_path, bestBrush_clone);
		
	}

#endif
int br_0, br_1, br_2;
Point p;
static int ibrush_cnt = 0;
if (ibrush_cnt < 5) {
	debug_image(string("br/IP_bestBrush_")+tag, ibrush_cnt, bestBrush);
	ibrush_cnt++;
}
	for (int by = 0; by < paint_grid_h_size; by++)
	{
		for (int bx = 0; bx < paint_grid_w_size; bx++)
		{

			p.x = (centered_SrtPoint.x + bx);
			p.y = (centered_SrtPoint.y + by);
			bool c_flag = false;
			if (p.x < 0) c_flag = true;
			if (p.y < 0)c_flag=true;
			if (p.x >(s_width - 1)) c_flag=true;
			if (p.y >(s_height - 1))  c_flag=true;
			//브러시에 색상 입히기
			int bIndex = by *b_step1 + bx * b_channels;
			int gray_bIndex = by*paint_grid_w_size + bx;
			//for tranparent effect 
		
			if (brush_gray[gray_bIndex] > g_brush_Ts ) c_flag = true;
			
			if ((bestBrush_data[bIndex + 2] == 255 
				&& bestBrush_data[bIndex + 1] == 255 &&
				bestBrush_data[bIndex + 0] == 255))
				c_flag=true;
				//	
			if (c_flag == true) {
				p_poke(bestBrush_data, bIndex, 255, 255, 255);
				continue;
			}

			double Alpha = 1.;// bestBrush_data[bIndex] /200.;
			if (g_brush_style == BRUSH_ALPHA) {
					bestBrush_data[bIndex + 2] = (int)(f_2 *  Alpha);
					bestBrush_data[bIndex + 1] = (int)(f_1 *  Alpha);
					bestBrush_data[bIndex + 0] = (int)(f_0 *  Alpha);

			}
			else if (g_brush_style == BRUSH_COPY ){
				bestBrush_data[bIndex + 0] = f_0;
				bestBrush_data[bIndex + 1] = f_1;
				bestBrush_data[bIndex + 2] = f_2;
			}
			br_2 = bestBrush_data[bIndex + 2];
			br_1 = bestBrush_data[bIndex + 1];
			br_0 = bestBrush_data[bIndex + 0];

			//proof_box(p,s_width,s_height,"p");

			int sIndex = p.y * s_step1+p.x * s_channels;
			;
	
	
			//그리기
		//	if ((brushData[bIndex ] <= 200 ))
			{
				p_poke(changedData, sIndex, br_0, br_1, br_2);
				/*
				changedData[sIndex + 2] = brushData[bIndex + 2];
				changedData[sIndex + 1] = brushData[bIndex + 1];
				changedData[sIndex + 0] = brushData[bIndex + 0];
				*/

				//	cout <<debug_image.data << endl;
		//		p_poke(colored_brush_ptr, bIndex, br_0, br_1, br_2);
				//	painted_ptr[bIndex + 2] = brushData[bIndex + 2];
					//painted_ptr[bIndex + 1] = brushData[bIndex + 1];
				//	painted_ptr[bIndex + 0] = brushData[bIndex + 0];

				p_poke_canvas(debug_image_depth_Data, p.y,p.x, br_0, br_1, br_2);
			
			}
		
		}
	}
	static int rbrush_cnt = 0;
	if (rbrush_cnt < 5) {
		debug_image(string("br/IP2_bestBrush_")+tag, rbrush_cnt, bestBrush);
		rbrush_cnt++;
	}
	//판단알고리즘 적용
//#ifdef DEBUG_COLORED_BRUSH
	//if (depth_check(astroke_depth)) {
	static int called_cnt = 0;
	//if ( called_cnt < g_debug_called_cnt && astroke_depth >= g_debug_min_level) {
		if (depth_check(astroke_depth)) {
	//	f_path2 = format("%s_3.jpg",f_path_a.c_str());
		rectangle(bestBrush,Point( 0, 0),Point( paint_grid_w_size- 1, paint_grid_h_size- 1), RGB(0, 255, 0));
	//	cv::imwrite(f_path2, bestBrush);
		
		//	String f_path3;
		//	f_path3 = format("/rst/ing/rst_%s_%d_%d.jpg", tag.c_str(), stroke_no, nth_brush);
		//	cv::imwrite(f_path3, beforeImg);
	//	rectangle(painted_img, Point(0, 0), Point(bsize - 1, bsize - 1), RGB(0, 255, 0));
		
		//	f_path2 = format("%s_3_1_%d_%d_%d_%d_%d.jpg",f_path_a.c_str(),fetch_color_Point.x,fetch_color_Point.y,f_0,f_1,f_2);
		//	cv::imwrite(f_path2, colored_brush);
		
		//	cv::rectangle(debug_image, Rect(bSrtPoint,Point(bSrtPoint.x+bsize,bSrtPoint.y+bsize)),RGB(255,0,0 ));
		called_cnt++;
		}
	
	
//#endif
	

		int  result = JudgementImage( srcData, changedData, beforeData,paint_grid_w_size,paint_grid_h_size,centered_SrtPoint,
			astroke_depth, s_width, s_height, s_channels);

		if (result == CHANGED_BETTER) {
			for (int by = 0; by < paint_grid_h_size; by++)
			{
				for (int bx = 0; bx < paint_grid_w_size; bx++)
				{

					p.x = (centered_SrtPoint.x + bx);
					p.y = (centered_SrtPoint.y + by);
					bool c_flag = false;
					if (p.x < 0) c_flag = true;
					if (p.y < 0)c_flag = true;
					if (p.x > (s_width - 1)) c_flag = true;
					if (p.y > (s_height - 1))  c_flag = true;
					//브러시에 색상 입히기
					if (c_flag == true) {
						continue;
					}
					int bIndex = by *b_step1 + bx * b_channels;
					int sIndex = p.y * s_step1 + p.x * s_channels;
					;
#ifdef G_DEBUG_TOUCH
					p_peek(colored_brush_ptr, bIndex, br_0, br_1, br_2);
					if (br_0 != 255 && br_1 != 255 && br_2 != 255)
					{
						
						int p_0, p_1, p_2;
						p_peek(g_touch_data, sIndex, p_0, p_1, p_2);
						if (p_0 == 0) {
							p_0 = 20; p_1 = 20; p_2 = 20;
						}
						p_0+=3; p_1+=3; p_2+=3;
						if (p_0 > 250 || p_1 > 250 || p_2 > 250) {
							cout << "p is greater than 250" << endl;
						}

						p_poke(g_touch_data, sIndex, p_0, p_1, p_2);

					}
#endif
					}
			}
		}
	return result;
}
Mat brush_at_brush(list<Brush> & _brush_set, int no) {
	static int called = 0;
	if ( called < 30 )
		cout << " " << no << endl;
	list<Brush>::iterator it = _brush_set.begin();
	for (; it != _brush_set.end(); it++) {
		if ((*it).brush_no == no)
			return (*it).brush.clone();
	}
	return _brush_set.front().brush.clone();
}

Mat brush_at_brush_gray(list<Brush> & _brush_set, int no) {
	static int called = 0;
	if (called < 30)
		cout << " " << no << endl;
	list<Brush>::iterator it = _brush_set.begin();
	for (; it != _brush_set.end(); it++) {
		if ((*it).brush_no == no)
			return (*it).brush_gray.clone();
	}
	return _brush_set.front().brush_gray.clone();
}
cv::Mat PainterlyRendering(cv::Mat &srcImg,  vector<QuadTree::Img_node*> aStroke_set, string tag,
	int m_brush_size[], int _depth,Mat _grid_map[], int _St_grid_painting_count[], list<Brush> &_brush_set,
	int _QT_grid_count[]) {

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
	cv::String f_path;
	int nth = 0;
	std::string p;

	nth = 0;

	//saved_depth = -1;
	int brush_no;
//	bool debug_level;
	int painting_count;
	int astroke_depth;
	int process_ratio;
	int saved_process_ratio = -1;
	
//	int rand_x, rand_y;
	//int bmax = g_BrushMaxSize, bmin = g_BrushMinSize;

	cv::Mat debug_image_depth;
	unsigned char* debug_image_depth_Data;

	cv::Mat rstImg;
	rstImg.create(image_height, image_width, CV_8UC3);
	PaintBackGround(rstImg, 255, 255, 255);
	cv::Mat St_painting_area;
	unsigned char * St_painting_area_data;

	cv::Mat Canvas;
	unsigned char * Canvas_data;
	Canvas.create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
	Canvas_data = Canvas.data;
	PaintBackGround(Canvas, 255, 255, 255);
	
	//unsigned char* rstData;// = (unsigned char*)rstImg.data;
	//unsigned char* srcData = (unsigned char*)srcImg.data;



#ifdef G_DEBUG_TOUCH
	unsigned char * current_fetched_map_data;
#endif
	unsigned char * current_try_map_data;

	

	

	St_painting_area.create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
	St_painting_area_data = St_painting_area.data;
	 PaintBackGround(St_painting_area_data, g_canvas_size_width, g_canvas_size_height, 255, 255, 255);
	
#ifdef G_DEBUG_TOUCH
	cv::Mat fetched_map[MAX_DEPTH]; // randominized fetched point
#endif
	cv::Mat try_map[MAX_DEPTH]; // randominized try grid map
	cv::Mat changed_map[MAX_DEPTH];
	int try_count[MAX_DEPTH];
	int changed_count[MAX_DEPTH]			;

	//	uniform_int_distribution<int> distribution(0, m_brush_size[saved_depth]);	// 생성 범위
	for (astroke_depth = 0; astroke_depth < _depth; astroke_depth++) {
			
	
#ifdef G_DEBUG_TOUCH
		fetched_map[astroke_depth].create(image_height, image_width, CV_8UC3);
		PaintBackGround(fetched_map[astroke_depth], 0, 0, 0);
#endif

		try_map[astroke_depth].create(image_height,image_width,CV_8UC3);
		PaintBackGround(try_map[astroke_depth], 0, 0, 0);

		
		changed_map[astroke_depth].create(g_canvas_size_height, g_canvas_size_width, CV_8UC3);
		PaintBackGround(changed_map[astroke_depth], 255, 255, 255);

		try_count[astroke_depth] = 0;
		changed_count[astroke_depth] = 0;
	
	}
	
#ifdef DEBUG_ASTROKE_REGION
	//prepare 
	debug_image_single *astroke_region = new debug_image_single(Size(rstImg.size().width,rstImg.size().height),
		(int)aStroke.size(), CV_8UC3);

	p.append(format("%s/st/st_%s_",g_para_method_path.c_str(),tag.c_str()));
	cout << p.c_str() << endl;
	astroke_region->set_file_format(p);

#endif

	debug_image_depth.create(g_canvas_size_height,g_canvas_size_width,CV_8UC3);
	debug_image_depth_Data = (unsigned char*)debug_image_depth.data;
	PaintBackGround(debug_image_depth, 255, 255, 255);

	//int level_try = 0;
	//cv::Mat level_cropped;
	saved_depth = -1;
	//f_path3 = format("%s/ing/ing_%s_%02d_i%04d.jpg", g_para_method_path.c_str(), tag.c_str(), saved_depth, ing);
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

		//int st_w_size_half = st_w_size / 2;
		//int st_h_size_half = st_h_size / 2;


		paint_grid_h_size = m_brush_size[astroke_depth];// brush size(painting area) per each depth
		paint_grid_w_size = m_brush_size[astroke_depth];// brush size(painting area) per each depth
		int paint_grid_h_size_half = paint_grid_h_size / 2;
		int paint_grid_w_size_half = paint_grid_w_size / 2;

		int paint_grid_count = (st_w_size * st_h_size *g_paint_grid_scale) /
			(m_brush_size[astroke_depth] * m_brush_size[astroke_depth]);
		if (paint_grid_count == 0) {
			cout << "depth " << astroke_depth << " st_size : " << st_w_size << ", " << st_h_size << " g_paint_grid_Scale " <<
				g_paint_grid_scale << " m_brush_size " << m_brush_size[astroke_depth] << endl;
			paint_grid_count = 4;
		}
	
		rectangle_canvas(St_painting_area, Rect(St_srtPoint, St_endPoint), Scalar(0, 0, 255));//RED QT outline

		
	
		if (saved_depth != astroke_depth )// new depth
		{	
			if (saved_depth != -1) {
				debug_image("ing/painting_area"+tag, saved_depth,St_painting_area);
				PaintBackGround(St_painting_area_data, g_canvas_size_width, g_canvas_size_height, 255, 255, 255);
				debug_image("ing/ing"+tag, saved_depth, debug_image_depth);
				PaintBackGround(debug_image_depth, 255, 255, 255);
		
				
				debug_image("ing/accu_"+tag, saved_depth, rstImg);

				debug_image("ing/try_map_"+tag, saved_depth, try_map[saved_depth]);
			
			}
			//level_try = 0;
			current_try_map_data = try_map[astroke_depth].data;
		
		}
	
	//	level_try++;
		saved_depth = astroke_depth;
	

		process_ratio = (int)(ing / (double)aStroke_set.size() * 100.);
	


#ifdef DEBUG_ASTROKE_REGION
		astroke_region->draw_rectangle(b,srtPoint,endPoint);
#endif
		
		 //srand((unsigned)time(NULL));
		
		

		 //g_brush_debug_setup(Size(bsize,bsize), g_St_grid_painting_count[astroke_depth], astroke_depth);
		 int first_try = 0;
		
		 uniform_int_distribution<int> dist_x(0, (int)(st_w_size));
		 uniform_int_distribution<int> dist_y(0, (int)(st_h_size));

		// cout << astroke_depth << ": " <<  k_x.min() << ", " << k_x.max() <<
		//	 ", "<<k_y.min() << ", " << k_y.max() << endl;

		 for (painting_count = 0; painting_count < paint_grid_count; painting_count++)
		 {

			 //	cout << bestBrush.size()<< endl;

			 try_count[astroke_depth]++;
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

			 _St_grid_painting_count[astroke_depth] ++;//

			 //	proof_box(fetch_color_Point, image_width, image_height);
				 //int cIndex = 
			 int fetch_Index = (fetch_color_Point.x + fetch_color_Point.y*image_width)*image_channels;

			 int canvas_Index = ((fetch_color_Point.x + g_canvas_size_bezel_size)
				 + (fetch_color_Point.y + g_canvas_size_bezel_size)*(image_width + g_canvas_size_bezel_size))*image_channels;
			 //int  fetch_Index = fetch_color_Point.y * image_step1 + fetch_color_Point.x * image_channels;
			 unsigned char* rstData = (unsigned char*)rstImg.data;
			 unsigned char* srcData = (unsigned char*)srcImg.data;

			 int f_0, f_1, f_2;
			 p_peek(srcData, fetch_Index, f_0, f_1, f_2);

			 p_poke(current_try_map_data, fetch_Index, 255, 255, 255);
			
			 //p_poke(current_try_map_data, fetch_Index, 255, 255, 255);
			 centered_SrtPoint.x = fetch_color_Point.x - paint_grid_w_size_half, centered_SrtPoint.y = fetch_color_Point.y - paint_grid_w_size_half;
			 centered_EndPoint.x = fetch_color_Point.x + paint_grid_h_size_half, centered_EndPoint.y = fetch_color_Point.y + paint_grid_h_size_half;


			 //시작점 및 종료점이 이미지 밖에있는지 확인
		 //	ChkIsInside(bSrtPoint, bEndPoint, fetch_color_Point, bsize, image_width, image_height);		
			 Point Point_X, Point_Y;
			 Rect safe_rect = safe_Rect_margin(centered_SrtPoint, centered_EndPoint, image_width, image_height,
				 Point_X, Point_Y);
			 rectangle_canvas(St_painting_area, safe_rect, Scalar(0, 0, 0));//BLACK

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
			 Rect rect = safe_Rect(centered_SrtPoint, centered_EndPoint, image_width, image_height);

			 cv::Mat testImg = srcImg(rect).clone();
			 resize(testImg, testImg, cv::Size(g_brush_thumbnail_size, g_brush_thumbnail_size));
			 cv::cvtColor(testImg, testImg, COLOR_RGB2GRAY);

			 brush_no = JudgementBrush(testImg, /*added by cwlee*/astroke_depth,
				 g_brush_thumbnail_size, g_brush_thumbnail_size,_brush_set);
			 //, stroke_no, paint_grid_w_size, paint_grid_h_size,
				 //centered_SrtPoint,centered_EndPoint,first_try);//get one of similiar best 5
			 cv::Mat bestBrush;
			 cv::Mat bestBrush_gray;
			 bestBrush = brush_at_brush(_brush_set, brush_no);
			 bestBrush_gray= brush_at_brush_gray(_brush_set, brush_no);
			 cv::resize(bestBrush, bestBrush, cv::Size(paint_grid_w_size, paint_grid_h_size));
			 cv::resize(bestBrush_gray, bestBrush_gray, cv::Size(paint_grid_w_size, paint_grid_h_size));
			 static int jbrush_cnt = 0;
			 if ( jbrush_cnt < 5){
				 debug_image(string("br/J_bestBrush_")+tag,jbrush_cnt, bestBrush);
				 jbrush_cnt++;
			}
			//string f_path_a;
		//	unsigned char * bestBrush_resize_data = bestBrush_resized.data;

		//	cv::cvtColor(bestBrush_resized, bestBrush, CV_GRAY2RGB);//????? why???

			//rectangle(bestBrush_resize, Point(0, 0), Point(paint_grid_size - 1, paint_grid_size - 1), RGB(0, 255, 0));
		//	if (depth_check(astroke_depth)) {
				//f_path = format("%s_0.jpg", f_path_a.c_str());
//				cv::imwrite(f_path, bestBrush_resized);

			//	f_path = format("%s_1.jpg", f_path_a.c_str());
//				cv::imwrite(f_path, bestBrush);
		//	}

			////changed souece location by cwlee 18. 7. 24

#ifdef DEBUG_ASTROKE_REGION
			astroke_region->draw_rectangle(b,bSrtPoint,bEndPoint);
#endif
			//렌더링
			
			unsigned char * brushData = (unsigned char *)bestBrush.data;
			unsigned char * brushData_gray = (unsigned char *)bestBrush_gray.data;
			Mat beforeImg = rstImg.clone();
			unsigned char *beforeData = beforeImg.data;

			
			int result = P_Rendering(srcImg,srcData,rstData, beforeData,brushData,
				bestBrush, fetch_color_Point, centered_SrtPoint, paint_grid_w_size,paint_grid_h_size,
			 stroke_no, ing, astroke_depth, painting_count, f_0, f_1, f_2,
				debug_image_depth_Data,brushData_gray);

			//	current_fetched_map_data,fetched_color_data,r,g,b);

			if (result == BEFORE_BETTER) {
				beforeImg.copyTo(rstImg);
			}
			else {
				changed_count[astroke_depth]++;//CHANGED_BETTER 
				Rect safe_rect = safe_Rect_margin(centered_SrtPoint, centered_EndPoint, image_width, image_height,
					Point_X, Point_Y);
				rectangle_canvas(changed_map[astroke_depth],safe_rect, Scalar(0, 0, 255));//RED
			}
			//mat_print(rstImg, "Result Image");
			first_try++;
		}//end of retry

	/*	
		if (!(ing % 5)) {
			f_path_t = format("%s/ing/%s_%03d_%03d_rst.jpg", g_para_method_path.c_str(),tag.c_str(), astroke_depth, ing);
		
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

	debug_image("ing/painting_areaf_"+tag, saved_depth, St_painting_area);
	debug_image("ing/ingf_"+tag, saved_depth, debug_image_depth);
	debug_image("ing/accu_f_"+tag, saved_depth, rstImg);
	debug_image("ing/try_map_f_"+tag, saved_depth, try_map[saved_depth]);

#ifdef G_DEBUG_TOUCH

	f_path_t = format("%s/ing/t_%s_d%03d_g%03d_i%03d_touch.jpg", g_para_method_path.c_str(), tag.c_str(), saved_depth, g_St_grid_painting_count[astroke_depth], ing);
	//cout << "f_path " << f_path_t.c_str() << endl;
	cv::imwrite(f_path_t, g_touch);
#endif

#ifdef DEBUG_ASTROKE_REGION
	astroke_region->image_final_split(10);
#endif
	int g_sum=0, t_sum=0, c_sum=0;
	int Qt_sum = 0;
	Mat fetched_color;
	unsigned char * fetched_color_data;
	fetched_color.create(image_height, image_width, CV_8UC3);
	fetched_color_data = (unsigned char *)fetched_color.data;
	PaintBackGround(fetched_color, 0, 0, 0);
	for (int i = 0; i < _depth; i++) {

		//f_path = format("%s/ing/f_%s_fetched_d%03d_g%03d.jpg", g_para_method_path.c_str(),tag.c_str(),i, g_St_grid_painting_count[i]);
		//cv::imwrite(f_path, try_map[i]);
		overlap(fetched_color, try_map[i]);
		overlap(try_map[i], _grid_map[i]);
		debug_image("ing/try_"+tag, i,try_map[i]);
		debug_image("ing/changed_"+tag, i,changed_map[i]);

		cout << "QT[" << i << "] = " << setw(4) << _QT_grid_count[i] << "  ";
		cout << "grid[" << i << "] = " << setw(4)<<_St_grid_painting_count[i] << " ";
		cout << "try[" << i << "] = " <<setw(10)<< try_count[i] <<" ";
		cout << "changed[" << i << "]= " <<setw(4)<< changed_count[i] <<"  "<<
			std::fixed << std::setw(11) << std::setprecision(2)
			//<< std::setfill('0')
			<<
			((float)changed_count[i]/(float)try_count[i])*100.<<endl;
		g_sum += _St_grid_painting_count[i];
		t_sum += try_count[i];
		c_sum += changed_count[i];
		
		g_file_cstat << g_para_method + "," << g_image_name << ", " << "depth:pr_count changed: "+to_string(i)+","<< 

			try_count[i] <<","<<to_string(changed_count[i]) <<  endl;
	}
	
	cout << setw(10) <<Qt_sum<<setw(18)<< g_sum << setw(18) << t_sum << setw(18) << c_sum<<endl;

	debug_image("fetched_color_"+tag, _depth, fetched_color);
	overlap(fetched_color, _grid_map[_depth-1]);
	debug_image("fetched_grid_F_"+tag, fetched_color);

#ifdef G_DEBUG_TOUCH
	f_path_t = format("%s/ing/%s_d%03d_g%03d_i%03d_touch.jpg", g_para_method_path.c_str(), tag.c_str(), saved_depth, g_St_grid_painting_count[astroke_depth], ing);
	cv::imwrite(f_path_t, g_touch);
#endif

	return rstImg;
}