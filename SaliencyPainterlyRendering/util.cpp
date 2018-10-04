#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "bsShowImage.h"
using namespace cv;

#include "extern.h"
#include <iostream>
std::string type2str(int type) {
	std::string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}

cv::Rect safe_Rect(Point s, Point e, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0) s.x = 0;
		if (s.x >= i_width) s.x = i_width - 1;
	if (e.x < 0) e.x = 0;
	if (e.x >= i_width) e.x = i_width - 1;

	if (s.y < 0) s.y = 0;
	if (s.y >= i_height) s.y = i_height - 1;
		if (e.y < 0) e.y = 0;
	if (e.y >= i_height) e.y = i_height - 1;
	rr = Rect(s, e);
	return rr;
}
void mat_copyTo(unsigned char *s, unsigned char * d, Rect s_rect, Rect d_rect, int width) {
	int s_p, d_p;
	for (int y = 0; y<s_rect.height; y++)
		for (int x = 0; x < s_rect.width; x++) {

			s_p = (s_rect.y + y)*width + (x + s_rect.x);
			d_p = (d_rect.y + y)*width + (x + d_rect.x);
			*(d + d_p) = *(s + s_p);

		}

}

void p_poke(unsigned char * p, int index, int p_0) {
	p[index] = p_0;
}
 void p_poke(unsigned char * p, int index, int p_0, int p_1, int p_2) {
	p[index] = p_0;
	p[index + 1] = p_1;
	p[index + 2] = p_2;
}
void p_peek(unsigned char * p, int index, int &p_0, int &p_1, int &p_2) {
	p_0 = p[index];
	p_1 = p[index + 1];
	p_2 = p[index + 2];
}
 void p_peek_1c(unsigned char * p, int index, int &p_0) {
	p_0 = p[index];
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
 /*
 bool fetch_in_box(Point &s, int i_width, int i_height) {

 Rect rr;
 if (s.x < 0) return false;
 if (s.x >= i_width) return false;


 if (s.y < 0) return false;
 if (s.y >= i_height) return false;
 return true;

 }
 */
 /*
 fetch_color_Point.x = St_srtPoint.x + random_x;
 fetch_color_Point.y = St_srtPoint.y + random_y;*/
 /*
 bool FixToInside(Point &b_SrtPoint, Point &b_EndPoint, Point &tryPoint, int image_width, int image_height,int bsize)
 {
 if (b_SrtPoint.x < 0)
 {
 b_SrtPoint.x = 0;
 //	tryPoint.x = (int)(bsize * 0.5) - 1;
 //	bEndPoint.x = bsize - 1;
 }
 if (b_SrtPoint.x + bsize > image_width)
 b_EndPoint.x = image_width - 1;

 if (b_EndPoint.x >= image_width)
 {
 //	bSrtPoint.x = width - bsize - 1;
 //	tryPoint.x = width - (int)(bsize * 0.5) - 1;
 b_EndPoint.x = image_width - 1;
 }

 if (b_SrtPoint.y < 0)
 {
 b_SrtPoint.y = 0;
 //	tryPoint.y = (int)(bsize * 0.5) - 1;
 //bEndPoint.y = bsize - 1;
 }
 if (b_SrtPoint.y + bsize > image_height)
 b_EndPoint.y = image_height - 1;
 if (b_EndPoint.y >= image_height)
 {
 //bSrtPoint.y = height - bsize - 1;
 //	tryPoint.y = height - (int)(bsize * 0.5) - 1;
 b_EndPoint.y = image_height - 1;
 }
 return true;
 }



 */


void mat_print(cv::Mat &amat, cv::String matname, int d) {
	cout << matname << endl;
	unsigned char * p;

	char buf[30];
	if (amat.cols == 0 || amat.rows == 0) {
		cout << "Empty Matrix, "<<d
			<< endl;
		cout << "-----------------------------------------------------------------------" << endl;
		return;
	}
	p = &(amat.data[0]);
	sprintf_s(buf, "%p", p);
	cout << "Data " << buf << ", "<<d;

	cout << "   ref :" << amat.u->refcount << endl;
	cout << "channels: " << amat.channels() << endl;
	cout << "ref :" << amat.u->refcount << endl;
	cout << "width :" << amat.size().width << endl;
	cout << "height :" << amat.size().height << endl;
	cout << "step1() : " << amat.step1() << endl;
	cout << "size() :" << amat.size() << endl;
	cout << "type() :" << amat.type() << ", " <<
		type2str(amat.type())
		<< endl;
	cout << "elem " << amat.elemSize() << ", " << amat.elemSize1() << endl;
	cout << "total " << amat.total() << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}
void mat_print(cv::Mat &amat, cv::String matname ) {
	cout << matname << endl;
	unsigned char * p;

	 char buf[30];
	 if (amat.cols == 0 || amat.rows == 0) {
		 cout << "Empty Matrix"
			 << endl;
		 cout << "-----------------------------------------------------------------------" << endl;
		 return;
	 }
	p =&(amat.data[0]);
	sprintf_s(buf, "%p", p);
	cout << "Data " << buf;

	cout << "   ref :" << amat.u->refcount << endl;
	cout << "channels: " << amat.channels() << endl;
	cout << "ref :" << amat.u->refcount << endl;
	cout << "width :" << amat.size().width << endl;
	cout << "height :" << amat.size().height << endl;
	cout << "step1() : " << amat.step1() << endl;
	cout << "size() :" << amat.size() << endl;
	cout << "type() :" << amat.type() << ", " <<
		type2str(amat.type())
		<< endl;
	cout << "elem " << amat.elemSize() << ", " << amat.elemSize1()<<endl;
	cout << "total " << amat.total() << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}
void mat_print(cv::Mat *amat, cv::String matname, int d) {
	cout << matname << endl;
	unsigned char * p;
	char buf[30];
	if (amat->cols == 0 || amat->rows == 0) {
		cout << "Empty Matrix, "<<d
			<< endl;
		return;
	}
	p = &(amat->data[0]);
	sprintf_s(buf, "%p", p);
	cout << "Data " << buf<<",d "<<d;
	cout << "   ref :" << amat->u->refcount << endl;
	/*cout << "channels: " << amat->channels() << endl;

	cout << "width :" << amat->size().width << endl;
	cout << "height :" << amat->size().height << endl;
	cout << "step1() : " << amat->step1() << endl;
	cout << "size() :" << amat->size() << endl;
	cout << "type() :" << amat->type() << ", " <<
	type2str(amat->type())
	<< endl;
	cout << "elem " << amat->elemSize() << ", " << amat->elemSize1() << endl;
	cout << "total " << amat->total() << endl;
	;
	*/
	cout << "-----------------------------------------------------------------------" << endl;
}
void mat_print(cv::Mat *amat, cv::String matname ) {
	cout << matname << endl;
	unsigned char * p;
	 char buf[30];
	 if (amat->cols == 0 || amat->rows == 0) {
		 cout << "Empty Matrix"
			 << endl;
		 return;
	 }
	p = &(amat->data[0]);
	sprintf_s(buf, "%p", p);
	cout << "Data " << buf;
	cout << "   ref :" << amat->u->refcount << endl;
	/*cout << "channels: " << amat->channels() << endl;

	cout << "width :" << amat->size().width << endl;
	cout << "height :" << amat->size().height << endl;
	cout << "step1() : " << amat->step1() << endl;
	cout << "size() :" << amat->size() << endl;
	cout << "type() :" << amat->type() << ", " <<
		type2str(amat->type())
		<< endl;
	cout << "elem " << amat->elemSize() << ", " << amat->elemSize1() << endl;
	cout << "total " << amat->total() << endl;
	;
	*/
	cout << "-----------------------------------------------------------------------" << endl;
}
void Point_2p_debug(cv::Point p1, cv::String p1_tag, cv::Point p2, cv::String p2_tag) {
	cout << p1_tag.c_str() << " " << setw(5) << p1.x << "," << setw(5) << p1.y << ":";
		cout<< p2_tag.c_str()<< setw(5) << p2.x << "," << setw(5) << p2.y << endl;
};
void Point_1p_debug(cv::Point p1, cv::String p1_tag) {
		cout << p1_tag.c_str() << " "<<setw(5) << p1.x << "," << setw(5)<<p1.y << ":"<<endl;
};