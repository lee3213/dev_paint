#include "stdafx.h"

#include "QuadTree.h"
#include "Judgement.h"

#include "extern.h"
#include "image_util.h"
#include "util.h"
#include "debug_image.h"
//#define Ts (0.0)
/////////////////////////////////////////////////////


bool compareDistance(DisData a, double b);
//void TakeColorDistance(cv::Mat &testImg, list<DisData> &colorDis);

bool compareDistance_rev(DisData a, double b);

//int  render_::TakeColorDistance_thumbnail(cv::Mat &testImg, int thumb_width, int thumb_height,int src_resized_8UC1_step1, string tag,int depth)
int  render_::JudgementBrush(cv::Mat &testImg_canvas, int depth, int t_image_width, int t_image_height, int _canvas_step1, string tag)
{
	vector<DisData> colorDis;
	colorDis.resize(g_BrushNumber);
	DisData newDistance;
	unsigned char* testData = (unsigned char*)testImg_canvas.data;
	
	int t_width = testImg_canvas.size().width;
	int t_height = testImg_canvas.size().height;
	int t_step1 = (int)testImg_canvas.step1();
	unsigned char* indexData;
	int loop_w, loop_h;
	//string f_path;
	int brush_no;
	static int called_cnt ;
	static int saved_depth = -1;
	if (saved_depth != depth) {
		saved_depth = depth;
		called_cnt = 0;
	}
	//colorDis.reserve(g_BrushNumber);
	int nth = 0;
//	bool flag=false;
//	unsigned char * ptr;
	//if (t_width < g_brush_thumbnail_size) {
		loop_w = t_width;
		loop_h = t_height;
	//	flag = true;
	//}
	//else {
	//	loop_w = brush_minimum_size;
	//	loop_h = brush_minimum_size;
	//}
	for (int i=0;i<g_BrushNumber;i++)
	{
		render_Brush * b_ptr = brush_resized_array[depth][i];

	//	if (flag == true) {
			indexData = b_ptr->brush_thumbnail_data;
	//	}
	//	else {
		//	indexData = b_ptr->brush_thumbnail_minimum_data;
	//	}
		int pixel_cnt = 0;
		double cDis = 0;

		for (int y = 0; y < loop_h; y++)
		{
			for (int x = 0; x < loop_w; x++)
			{
				size_t index = y * t_step1 + x;
			//	if (indexData[index] <= 240) {
				if (indexData[index] < g_alpha_TH) {// test effective pixel 
					cDis += abs(testData[index] - indexData[index]);
					pixel_cnt++;
				}//
				//}
			}
		}
		
	//	cDis /= thumb_height*thumb_width;
		cDis /= pixel_cnt;

		vector<DisData>::iterator it2 = lower_bound(colorDis.begin(), colorDis.end(), (double)cDis, compareDistance);

		newDistance.distance = cDis;
		newDistance.nth = nth;
		colorDis.insert(it2, newDistance);
		//cout << " distance " << setw(10)<<cDis<< "b no "<<(*it).brush_no << " nth " << nth << endl;
		nth++;
	}
#ifdef DEBUG_JUDGEMENT
	if (called_cnt < 40) {
		for (int i = 0; i < 5; i++)
		{
			string f_name = tag + "_" + to_string(i);
			debug_image(f_name, _brush_set.at(i)->brush_thumbnail);
		}
	}
#endif
	called_cnt++;
	if (g_brush_choice == 0) {
		nth = rand() % 5;
	}
	else
		nth = 0;
	brush_no = colorDis.at(nth).nth;
	//g_file_clog << __FILE__ << " , " << tag << " , " << depth << brush_no << endl;
	return brush_no;
}

int  render_::JudgementBrush_pgm(cv::Mat &testImg_canvas, int depth, int t_image_width, int t_image_height, int _canvas_step1, string tag)
{
	vector<DisData> colorDis;
	colorDis.resize(g_BrushNumber);
	DisData newDistance;
	unsigned char* testData = (unsigned char*)testImg_canvas.data;

	int t_width = testImg_canvas.size().width;
	int t_height = testImg_canvas.size().height;
	int t_step1 = (int)testImg_canvas.step1();
	unsigned char* indexData;
	int loop_w, loop_h;
	//string f_path;
	int brush_no;
	static int called_cnt;
	static int saved_depth = -1;
	if (saved_depth != depth) {
		saved_depth = depth;
		called_cnt = 0;
	}
	//colorDis.reserve(g_BrushNumber);
	int nth = 0;
	//	bool flag=false;
	//	unsigned char * ptr;
	//if (t_width < g_brush_thumbnail_size) {
	loop_w = t_width;
	loop_h = t_height;
	//	flag = true;
	//}
	//else {
	//	loop_w = brush_minimum_size;
	//	loop_h = brush_minimum_size;
	//}
	for (int i = 0; i<g_BrushNumber; i++)
	{
		render_Brush_pgm * b_ptr = brush_pgm_resized_array[depth][i];

		//	if (flag == true) {
		indexData = b_ptr->brush_thumbnail_data;
		//	}
		//	else {
		//	indexData = b_ptr->brush_thumbnail_minimum_data;
		//	}
		int pixel_cnt = 0;
		double cDis = 0;

		for (int y = 0; y < loop_h; y++)
		{
			for (int x = 0; x < loop_w; x++)
			{
				size_t index = y * t_step1 + x;
				//	if (indexData[index] <= 240) {
				if (indexData[index] < g_alpha_TH) {// test effective pixel 
					cDis += abs(testData[index] - indexData[index]);
					pixel_cnt++;
				}//
				 //}
			}
		}

		//	cDis /= thumb_height*thumb_width;
		cDis /= pixel_cnt;

		vector<DisData>::iterator it2 = lower_bound(colorDis.begin(), colorDis.end(), (double)cDis, compareDistance);

		newDistance.distance = cDis;
		newDistance.nth = nth;
		colorDis.insert(it2, newDistance);
		//cout << " distance " << setw(10)<<cDis<< "b no "<<(*it).brush_no << " nth " << nth << endl;
		nth++;
	}
#ifdef DEBUG_JUDGEMENT
	if (called_cnt < 40) {
		for (int i = 0; i < 5; i++)
		{
			string f_name = tag + "_" + to_string(i);
			debug_image(f_name, _brush_set.at(i)->brush_thumbnail);
		}
	}
#endif
	called_cnt++;
	if (g_brush_choice == 0) {
		nth = rand() % 5;
	}
	else
		nth = 0;
	brush_no = colorDis.at(nth).nth;
	//g_file_clog << __FILE__ << " , " << tag << " , " << depth << brush_no << endl;
	return brush_no;
}




/*

{
	int brush_no = TakeColorDistance_thumbnail(testImg_canvas, width, height, _canvas_step1,tag, depth);
	return brush_no;
}
*/
bool compareDistance(DisData a, double b)
{
	return a.distance > b ? true : false;
}
bool compareDistance_rev(DisData a, double b)
{
	return a.distance < b ? true : false;
}


//rstImg = JudgementImage(rstImg, tempImg, bsize, srcData, rstData, tempData, bSrtPoint);
//int  result = JudgementImage(srcData, changedData, beforeData, brush_area_w_size, brush_area_h_size, centered_SrtPoint,
	//astroke_depth, s_width, s_height, s_channels);

int JudgementImage(unsigned char * src_ROI_ptr, unsigned char * changed_ROI_clone_canvas_Data_p, unsigned char * before_ROI_clone_canvas_Data_p, 
	int brush_area_w_size, int brush_area_h_size,
	Point centered_SrtPoint,//, Point _fetch_color_point,
	int astroke_depth,
	int b_w, int b_h, int b_c, int b_step1,
	int canvas_w, int canvas_h, int canvas_c,int canvas_ROI_step1,
	string tag,
	int canvas_clone_ROI_step1)
{
//	unsigned char * beforeData_p = beforeImg.data;
	//unsigned char * changedData_p = changedImg.data;
	int src2rst = 0, src2before = 0;
	

//	string f_path;
#ifdef DEBUG_
	static int called_cnt = 0;
	static int saved_depth =- 1;
	if (saved_depth != astroke_depth) {
		called_cnt = 0;
		saved_depth = astroke_depth;
	}
	cv::Mat be(brush_area_w_size, brush_area_h_size, CV_8UC3);
	unsigned char * be_data = be.data;
	PaintBackGround(be_data, brush_area_w_size, brush_area_h_size, 0, 0, 0);
	
	cv::Mat ch(brush_area_w_size, brush_area_h_size, CV_8UC3);
	unsigned char * ch_data = ch.data;
	PaintBackGround(ch_data, brush_area_w_size, brush_area_h_size, 0, 0, 0);

	cv::Mat sr(brush_area_w_size, brush_area_h_size, CV_8UC3);
	unsigned char * sr_data = sr.data;
	PaintBackGround(sr_data, brush_area_w_size, brush_area_h_size, 0, 0, 0);

#endif
//	int s_channel = s_c;
//	int s_height = s_h;
//	int s_width = s_w;
//	int s_step1 = s_c*s_w;
//	int s_Idx_x, s_Idx_y;
	//int c_step1 = ccanvas_c*c_w;
	for (int by = 0; by < brush_area_h_size; by++)
	{
		for (int bx = 0; bx < brush_area_w_size; bx++)
		{
		
		//	int src_index = ((by)*src_resized_8UC3_step1 +bx*3;
			int canvas_ROI_index = (by*canvas_ROI_step1) + (bx)* canvas_c;
		//	int b_index = (by*b_step1) + bx*b_c;
			int canvas_clone_ROI_index = (by*canvas_clone_ROI_step1) + bx*canvas_c;
		//	int c_index= (s_Idx_y)*c_step1 + (s_Idx_x)* c_c;
			int s_0, s_1, s_2;
			int b_0, b_1, b_2;
			int c_0, c_1, c_2;


			p_peek(src_ROI_ptr,canvas_ROI_index, s_0, s_1, s_2);
			p_peek(before_ROI_clone_canvas_Data_p, canvas_clone_ROI_index, b_0, b_1, b_2);
			p_peek(changed_ROI_clone_canvas_Data_p, canvas_clone_ROI_index, c_0, c_1, c_2);
#ifdef DEBUG_
			p_poke(sr_data, b_index, s_0, s_1, s_2);
			p_poke(ch_data, b_index, c_0, c_1, c_2);
			p_poke(be_data, b_index, b_0, b_1, b_2);

		//	rectangle(sr, 
		//		Point(brush_area_w_size/2 - 2, brush_area_w_size/2 - 2),
			//	Point(brush_area_w_size/2 + 2, brush_area_w_size/2 + 2),

			//	Scalar(0,0,255));
#endif

			src2rst += abs(s_2 - c_2)+ abs(s_1 - c_1)+ abs(s_0- c_0);

			src2before += abs(s_2 - b_2) + abs(s_1 - b_1) + abs(s_0 - b_0);

			//src2rst += abs(srcData[index + 2] - changedData_p[index + 2]);
		//	src2rst += abs(srcData[index + 1] - changedData_p[index + 1]);
		//	src2rst += abs(srcData[index + 0] - changedData_p[index + 0]);

			//src2before += abs(srcData[index + 2] - beforeData_p[index + 2]);
			//src2before += abs(srcData[index + 1] - beforeData_p[index + 1]);
			//src2before += abs(srcData[index + 0] - beforeData_p[index + 0]);
		}
	}
#ifdef DEBUG_
	if (called_cnt < 30) {
		debug_image(string("p") + to_string(astroke_depth) + string("/") + to_string(called_cnt) + string("_0sr_" + tag), sr);
		debug_image(string("p") + to_string(astroke_depth) + string("/") + to_string(called_cnt) + string("_2ch_")+tag, ch);
		debug_image(string("p") + to_string(astroke_depth) + string("/") + to_string(called_cnt) + string("_1be_")+tag, be);
		
		called_cnt++;
	}
	ch.release();
	be.release();
	sr.release();
#endif
	//csv_log << ", "<<", "<<src2rst << "," << src2before <<","<<src2rst-src2before<< endl;
	
	if (src2rst <= src2before)
		return CHANGED_BETTER ;
	else
		return BEFORE_BETTER;
}


/*
void TakeColorDistance(vector <Brush*> &brush, cv::Mat &testImg, list<DisData> &colorDis)
{
	DisData newDistance;
	unsigned char* testData = (unsigned char*)testImg.data;
	int width = testImg.size().width;
	int height = testImg.size().height;

	int nth = 0;
	for (vector <Brush*>::iterator it = brush.begin(); it != brush.end(); it++, nth++)
	{
		unsigned char* indexData = (unsigned char*)(*it)->index_brush.data;
		double cDis = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				size_t index = y * testImg.step1() + x;
				if (indexData[index] != 255) 
			
				cDis += abs(testData[index] - indexData[index]);
			}
		}
		cDis /= (width * height);
		list<DisData>::iterator it2 = lower_bound(colorDis.begin(), colorDis.end(), (double)cDis, compareDistance_rev);
		
		newDistance.distance = cDis;
		newDistance.nth = nth;
		colorDis.insert(it2, newDistance);
		//cout << " distance " << setw(10)<<cDis<< "b no "<<(*it).brush_no << " nth " << nth << endl;
	}
}
*/
