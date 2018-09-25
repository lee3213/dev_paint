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
void TakeColorDistance(cv::Mat &testImg, list<DisData> &colorDis);

bool compareDistance_rev(DisData a, double b);
int  TakeColorDistance_thumbnail(cv::Mat &testImg, int width, int height, list<Brush> &_brush_set)
{
	vector<DisData> colorDis;
	colorDis.resize(g_BrushNumber);
	DisData newDistance;
	unsigned char* testData = (unsigned char*)testImg.data;
	//int width = testImg.size().width;
	//	int height = testImg.size().height;
	string f_path;
	int brush_no;

	int nth = 0;
	for (list<Brush>::iterator it = _brush_set.begin(); it != _brush_set.end(); it++, nth++)
	{
		unsigned char* indexData = (unsigned char*)(*it).brush_thumbnail.data;

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
		vector<DisData>::iterator it2 = lower_bound(colorDis.begin(), colorDis.end(), (double)cDis, compareDistance);

		newDistance.distance = cDis;
		newDistance.nth = nth;
		colorDis.insert(it2, newDistance);
		//cout << " distance " << setw(10)<<cDis<< "b no "<<(*it).brush_no << " nth " << nth << endl;
	}
	nth = rand() % 5;
	brush_no = colorDis.at(nth).nth;
	return brush_no;
}
//rstImg = JudgementImage(rstImg, tempImg, bsize, srcData, rstData, tempData, bSrtPoint);
//int  result = JudgementImage(srcData, changedData, beforeData, brush_area_w_size, brush_area_h_size, centered_SrtPoint,
	//astroke_depth, s_width, s_height, s_channels);

int JudgementImage(unsigned char * srcData, unsigned char * changedData_p, unsigned char * beforeData_p, 
	int brush_area_w_size, int brush_area_h_size,
	Point centered_SrtPoint, Point _fetch_color_point,
	int astroke_depth, int s_w, int s_h, int s_c,
	int c_w, int c_h, int c_c,
	string tag)
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
	int s_channel = s_c;
	int s_height = s_h;
	int s_width = s_w;
	int s_step1 = s_c*s_w;
	int s_Idx_x, s_Idx_y;
	int c_step1 = c_c*c_w;
	for (int by = 0; by < brush_area_h_size; by++)
	{
		for (int bx = 0; bx < brush_area_w_size; bx++)
		{
			s_Idx_x = centered_SrtPoint.x + bx;
			s_Idx_y = centered_SrtPoint.y + by;
		
			if (s_Idx_x < 0) continue; //skip outside of image
			if (s_Idx_y < 0) continue;
			if (s_Idx_y > (s_height-1)) continue;
			if (s_Idx_x > (s_width-1)) continue;
		
			int s_index = (s_Idx_y) *s_step1  + (s_Idx_x ) * s_channel;
		
			int b_index = bx*s_channel + (by*brush_area_w_size*s_channel);
			int c_index= (s_Idx_y)*c_step1 + (s_Idx_x)* c_c;
			int s_0, s_1, s_2;
			int b_0, b_1, b_2;
			int c_0, c_1, c_2;


			p_peek(srcData, s_index, s_0, s_1, s_2);
			p_peek(beforeData_p, c_index, b_0, b_1, b_2);
			p_peek(changedData_p, c_index, c_0, c_1, c_2);
#ifdef DEBUG_
			p_poke(sr_data, b_index, s_0, s_1, s_2);
			p_poke(ch_data, c_index, c_0, c_1, c_2);
			p_poke(be_data, c_index, b_0, b_1, b_2);

			rectangle(sr, 
				Point(brush_area_w_size/2 - 2, brush_area_w_size/2 - 2),
				Point(brush_area_w_size/2 + 2, brush_area_w_size/2 + 2),

				Scalar(0,0,255));
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
	clog << ", "<<", "<<src2rst << "," << src2before <<","<<src2rst-src2before<< endl;
	
	if (src2rst <= src2before)
	return CHANGED_BETTER ;
	else
		return BEFORE_BETTER;
}



int  JudgementBrush(cv::Mat &testImg, int depth, int width, int height,list<Brush> _brush_set)
	/* list<Brush> &brush_set, int nBrushNumber,*/
//	 int stroke_no,int brush_area_w_size,int brush_area_h_size)// Point centered_SrtPoint,
//	Point centered_EndPoint,int first_try)
{
	
	//rangePoint.width = bEndPoint.x - bSrtPoint.x;
	//rangePoint.height= bEndPoint.y - bSrtPoint.y;

//	rangePoint.x = (*it)->info.endPoint.x - (*it)->info.srtPoint.x;
//	rangePoint.y = (*it)->info.endPoint.y - (*it)->info.srtPoint.y;

//	Point i_s, i_e;
//	Point i_r;
//	i_r = rangePoint;
//	i_s = (*it)->info.srtPoint;
//	i_e=(*it)->info.endPoint;

	//Rect rect((*it)->info.srtPoint.x, (*it)->info.srtPoint.y, rangePoint.x, rangePoint.y);
	
	


	//가장 적합한 Brush 확인(index Brush와 testImage간의 색상거리 확인)
	//list<DisData> colorDis;
	//colorDis.resize(BrushNumber);// room for total brush
//	TakeColorDistance(brush, testImg, colorDis);

	int brush_no=TakeColorDistance_thumbnail(testImg,width,height,_brush_set);


	

	return brush_no;
}

bool compareDistance(DisData a, double b)
{
	return a.distance > b ? true : false;
}
bool compareDistance_rev(DisData a, double b)
{
	return a.distance < b ? true : false;
}

void TakeColorDistance(list<Brush> &brush, cv::Mat &testImg, list<DisData> &colorDis)
{
	DisData newDistance;
	unsigned char* testData = (unsigned char*)testImg.data;
	int width = testImg.size().width;
	int height = testImg.size().height;

	int nth = 0;
	for (list<Brush>::iterator it = brush.begin(); it != brush.end(); it++, nth++)
	{
		unsigned char* indexData = (unsigned char*)(*it).index_brush.data;
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
