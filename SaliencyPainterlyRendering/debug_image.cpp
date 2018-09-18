#include "stdafx.h"
#include "debug_image.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;
#include "image_util.h"
//#define _DEBUG_IMAGE YES

void get_ext(char *ext, int channel_type, int ftype) {

	if (ftype == FILE_PPM) {
		if (channel_type == 3)
			sprintf_s(ext, 10,"ppm");
		else if (channel_type == 1)
			sprintf_s(ext,10, "pgm");
	}
	else sprintf_s(ext,10, "jpg");
	
}
void debug_image_abs(string _path, string fname,Mat &a, int ftype )
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;
	string whole_path;
	char ext[10];
	get_ext(ext, b.channels(), ftype);
	
	whole_path = _path +"/"+fname+ string(".")+string(ext);
#ifdef _DEBUG_IMAGE 
	cout << whole_path << endl;
#endif
	imwrite(whole_path.c_str(), b);
}
void debug_image_abs(string str, Mat &a, int ftype)
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;
	string path;
	char ext[10];
	get_ext(ext, b.channels(),ftype);
	path = str + string(".")+ string(ext);
#ifdef _DEBUG_IMAGE 
	cout << path << endl;
#endif
	imwrite(path.c_str(), b);
}

void debug_image_abs(char * p, Mat &a, int ftype )
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;

	string path;
	char ext[10];
	get_ext(ext, b.channels(), ftype);
	path = string(p) + string(".")+string(ext);
#ifdef _DEBUG_IMAGE 
	cout << path << endl;
#endif
	imwrite(path.c_str(), b);
}
void debug_image_method(string str, Mat &a, int ftype )
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;

	string path;
	//path = g_para_method_path + string("/") + str + string(".ppm");
#ifdef _DEBUG_IMAGE 
	cout << path << endl;
#endif
	char ext[10];
	get_ext(ext, b.channels(), ftype);

	path = format("%s/%s.%s", g_para_method_path.c_str(), str.c_str(),ext);
	imwrite(path.c_str(), b);
}

void debug_image_method(char * p, Mat &a, int ftype )
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;

	string path;
//	path = g_para_method_path + string("/") + string(p) + string(".ppm");
#ifdef _DEBUG_IMAGE 
	cout << path << endl;
#endif
	char ext[10];
	get_ext(ext, b.channels(), ftype);

	path = format("%s/%s.%s", g_para_method_path.c_str(), p,ext);
	imwrite(path.c_str(), b);
}

void debug_image(string str, Mat &a, int ftype )
{

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}

	else b = a;
	string path;
//	path = g_para_method_image_path + string("/") + str + string(".ppm");
#ifdef _DEBUG_IMAGE 
	cout << path << endl;
#endif
	char ext[10];
	get_ext(ext, b.channels(), ftype);

		path = format("%s/%s.%s", g_para_method_image_path.c_str(), str.c_str(),ext);
	imwrite(path.c_str(), b);
}


void debug_image(char * p, Mat &a, int ftype )
{
	string path;
	//str = format("%s/%s.ppm", g_para_method_image_path.c_str(), p);
	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout << type2str(a.type());
		a.convertTo(b, CV_8UC3, 255.);
	}
	else b = a;
#ifdef _DEBUG_IMAGE 
	cout << str.c_str() << endl;
#endif
	char ext[10];
	get_ext(ext, a.channels(), ftype);

		path = format("%s/%s.%s", g_para_method_image_path.c_str(), p,ext);
	imwrite(path.c_str(), b);
}
void debug_image(char * p, int d, vector<Mat> a, int ftype )
{
	string path;
	for (int nth = 0; nth < a.size(); nth++) {
	//	str = format("%s/%s_%d_%d.ppm", g_para_method_image_path.c_str(), p, d, nth);
		Mat b;
		if ((a.at(nth).type() & CV_MAT_DEPTH_MASK) == CV_32F) {
			//cout <<p<<" "<< type2str(a.type())<<endl;
			a.at(nth).convertTo(b, CV_8UC3, 255.);
		}
		else b = a.at(nth);
		//#ifdef _DEBUG_IMAGE 
		//	cout << str.c_str() << endl;
		//#endif
		char ext[10];
		get_ext(ext, b.channels(), ftype);
		
			path = format("%s/%s_%d.%s", g_para_method_image_path.c_str(), p, d,ext);
		imwrite(path.c_str(), b);
	}
}
void debug_image(char * p, int d, Mat &a, int ftype)
{
	string path;
	

	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
		//cout <<p<<" "<< type2str(a.type())<<endl;
		a.convertTo(b, CV_8UC3, 255.);
	}
	else b = a;
	char ext[10];
	get_ext(ext, b.channels(), ftype);
	path = format("%s/%s_%d.%s", g_para_method_image_path.c_str(), p, d,ext);
//#ifdef _DEBUG_IMAGE 
//	cout << str.c_str() << endl;
//#endif
	imwrite(path.c_str(), b);
}
void debug_image(string p, int d, Mat &a, int ftype )
{
	string path;
	//str = format("%s/%s_%d.ppm", g_para_method_image_path.c_str(), p.c_str(), d);
	Mat b;
	if ((a.type() & CV_MAT_DEPTH_MASK) == CV_32F) {
	//	cout << p<<" "<<type2str(a.type())<<endl;
		a.convertTo(b, CV_8UC3, 255.);
	}
	else b = a;
	char ext[10];
	get_ext(ext, b.channels() ,ftype);
	path = format("%s/%s_%d.%s", g_para_method_image_path.c_str(), p.c_str(), d, ext);
	imwrite(path.c_str(), b);
}

