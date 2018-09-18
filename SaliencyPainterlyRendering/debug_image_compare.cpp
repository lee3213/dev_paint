#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;

#include "debug_image_compare.h"
#include "debug_image.h"
debug_image_single::debug_image_single(Size im_size, int howmany, int mat_type)
{

	a_image_size = im_size;
	c_called_cnt = 0;
	if (howmany > 100) c_howmany = 100;
	else
		c_howmany = howmany;
	c_image_size.x = a_image_size.x;
	c_image_size.y = (a_image_size.y*c_howmany);
	Mat_aRow_d.create(c_image_size.y, c_image_size.x, mat_type);

	Mat_aRow_d.setTo( 255);

	for (int i = 0; i < howmany; i++) {
		line(Mat_aRow_d, Point(0, i*a_image_size.y - 1), Point(a_image_size.x - 1, (i)*a_image_size.y - 1),
			Scalar(0, 0, 255));
	}
}



void  debug_image_single::image_reset(int r) {

	Mat_aRow_d.setTo( r);
};
void  debug_image_single::set_file_format(std::string p)
{
	fname = p;
};
Mat *debug_image_single::get_mat()
{
	return &Mat_aRow_d;
};
void debug_image_single::image_add(cv::Mat & leftimage) {

	if (c_called_cnt % 100 > c_howmany) {
		write_image_file(c_called_cnt);
	}

	if (!(c_called_cnt % 100) && (c_called_cnt != 0)) {

		write_image_file(c_called_cnt);
	}


	int y = c_called_cnt % 100;
	Rect r = cv::Rect(0, y*a_image_size.y, a_image_size.x, a_image_size.y);
	leftimage.copyTo(Mat_aRow_d(r));

	c_called_cnt++;
}
void debug_image_single::image_add(cv::Mat & leftimage, int bsize) {
	bsize -= 1;
	if (c_called_cnt % 100 > c_howmany) {
		write_image_file(c_called_cnt);
	}

	if (!(c_called_cnt % 100) && (c_called_cnt != 0)) {

		write_image_file(c_called_cnt);
	}


	int y = c_called_cnt % 100;
	Rect r = cv::Rect(0, y*a_image_size.y, bsize, bsize);
	leftimage.copyTo(Mat_aRow_d(r));

	c_called_cnt++;
}


void debug_image_single::image_final() {
	//	cout << "fname format " << fname_format.c_str() << endl;
	//	fname_format.append("%05d.ppm");
	//		fname = format(fname_format.c_str() ,  c_called_cnt);
	//	cout << "fname " << fname.c_str() << endl;
	debug_image(fname, c_called_cnt, Mat_aRow_d);
	//	imwrite(fname, Mat_aRow_d);
}
void debug_image_single::image_final_split(int a_size) {

	int i;
	int k;
	//	fname_format.append("%05d_%03d.ppm");
	for (i = 0; i <= c_howmany / a_size; i++) {

		//fname = format(fname_format.c_str(), c_called_cnt,i);
		cout << "fname " << fname.c_str() << endl;
		if ((c_howmany - i * a_size) < a_size)
			k = (c_howmany - i * a_size);
		else k = a_size;
		Rect r = Rect(Point(0, i * a_size * a_image_size.y), Point(a_image_size.x, (i * a_size + k) * a_image_size.y));
		//	cout << "r " << r.size().height << ", " << r.size().width << endl;
		Mat roi = Mat_aRow_d(r);
		//	cout << "roi " << roi.size().height<<", "<<roi.size().width<<" k= "<<k << endl;
		//imshow("roi", roi);
		debug_image(fname, roi);
	}
}
void debug_image_single::draw_rectangle(Point s, Point e)
{
	//s.x = b.x;
	s.y = (c_called_cnt%c_howmany)*a_image_size.y + s.y;
	//e.x = e.x - b.x;
	e.y = (c_called_cnt%c_howmany)*a_image_size.y + e.y;
	rectangle(Mat_aRow_d, Rect(s, e), RGB(255, 0, 0));
	//cout << s.x << "," << s.y << " : " << e.x << ", " << e.y << endl;
}
void debug_image_single::next_roi() {
	c_called_cnt++;
}



void debug_image_single::write_image_file(int calledcnt) {

	//fname = format(fname_format.c_str(), calledcnt);
	//	imwrite(fname, Mat_aRow_d);
	debug_image(fname, Mat_aRow_d);
	Mat_aRow_d.setTo(255);
}