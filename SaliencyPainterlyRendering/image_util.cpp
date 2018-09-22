#include "stdafx.h"
#include "image_util.h"
#include "debug_image.h"
void overlap(Mat &a, Mat &b,int channel)
{
	//assume same size mat
	unsigned char * p_a, *p_b;
	int rows, cols;
	rows = a.size().height;
	cols = a.size().width;
	p_a = a.data;
	p_b = b.data;
	int inx;
	bool tf;
	for (int y = 0; y<rows; y++)
		for (int x = 0; x < cols; x++)
		{
			inx = (x + y*cols) * channel;
			tf = false;
			for (int i = 0; i < channel; i++) {
				if (p_b[inx + i] != 0) {
					tf = true;
					break;
				}
			}
			if (tf)  {
				for (int i = 0; i < channel; i++) {
					p_a[inx+channel] = 255;
				}
				}
		
		}
}
void overlap_white(unsigned char * p_a, unsigned char * p_b,int rows, int cols)
{
	//assume same size mat
	//unsigned char * p_a, *p_b;
	//int rows, cols;
	//rows = a.size().height;
	//cols = a.size().width;
	//p_a = a.data;
	//p_b = b.data;
	int inx;
	for (int y = 0; y<rows; y++)
		for (int x = 0; x < cols; x++)
		{
			inx = (x + y*cols) * 3;
			if (p_b[inx] != 255 || p_b[inx + 1] != 255 || p_b[inx + 2] != 255) {
				p_a[inx] = 255;
				p_a[inx + 1] = 255;
				p_a[inx + 2] = 255;
			}
		}

}
void overlap_blue(Mat &a, Mat &b)
{
	//assume same size mat
	debug_image("overlap_blue_a", a);
	debug_image("overlap_blue_b", b);


	unsigned char * p_a, *p_b;
	int rows, cols;
	rows = a.size().height;
	cols = a.size().width;
	p_a = a.data;
	p_b = b.data;
	int inx;
	for (int y = 0; y<rows; y++)
		for (int x = 0; x < cols; x++)
		{
			inx = (x + y*cols) * 3;
			if (p_b[inx] != 255 ) {
				p_a[inx] = 255;
				p_a[inx + 1] = 255;
				p_a[inx + 2] = 255;
			}
		}
}
cv::Mat ColorReversal(cv::Mat &srcImg)//cv::Mat invSrc =  cv::Scalar::all(255) - src;
{
	unsigned char* srcData = (unsigned char*)srcImg.data;

	int height = srcImg.size().height, width = srcImg.size().width;
	size_t widthStep = srcImg.step[0];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int index = y * width + x;
			unsigned char  temp = 255 - srcData[index];
			srcData[index] = temp;
		}
	}
	return srcImg;
}

void  PaintBackGround(unsigned char * rstData, int width, int height,int b0, int b1, int b2)
{
	
	//int width = rstImg.size().width;
//	int height = rstImg.size().height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			size_t index = y * width*3 + x * 3;
			rstData[index + 2] = b2;
			rstData[index + 1] = b1;
			rstData[index + 0] = b0;
		}
	}

}

void  PaintBackGround(cv::Mat &a_mat, int b0, int b1, int b2)
{
	//cv::Mat temp = a_mat;
	unsigned char* a_mat_data = (unsigned char*)a_mat.data;
	int width = a_mat.size().width;
	int height = a_mat.size().height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			size_t index = y * a_mat.step1() + x * a_mat.channels();
			a_mat_data[index + 2] = b2;
			a_mat_data[index + 1] = b1;
			a_mat_data[index + 0] = b0;
		}
	}

}

cv::Rect rounding_box(cv::Mat a_mat,int bsize_x,int bsize_y, int V_th){
unsigned char *p = a_mat.data;

Point crop_s, crop_e;
bool find_flag = false;
//int V_th = 235;
int x, y, v;

for (y = 0; y < bsize_y; y++)
{

	for (x = 0; x < bsize_x; x++) {
		int k;
		k = x + y * bsize_x;
		v = (int)(*(p + k));
		if (v <  V_th) {
	//		cout << "T y : " << x << "," << y << " : " << v << endl;
			find_flag = true;
			break;
		}

	}
	if (find_flag == true) break;
}

if (find_flag == true)
{
	crop_s.y = y;
}
else {
	cout << "Never top s.y " << crop_s.y<< endl;
}

//cout <<  crop_s.x << ", " << crop_s.y << ", " << crop_e.x << ", " << crop_e.y << endl;

find_flag = false;
for (x = 0; x < bsize_x; x++)
{

	for (y = 0; y < bsize_y; y++) {
		int k;
		k = x + y * bsize_x;
		v = (int)(*(p + k));
		if (v <  V_th) {
		//	cout << "L x : " << x << "," << y << " : " << v << endl;

			find_flag = true;
			break;
		}

	}
	if (find_flag == true) break;
}

if (find_flag == true)
{
	crop_s.x = x;
}
else {
	cout << "Never top s.x"<< crop_s.x << endl;
}

//cout <<  crop_s.x << ", " << crop_s.y << ", " << crop_e.x << ", " << crop_e.y << endl;


find_flag = false;

for (y = bsize_y-1; y >= 0; y--)
{
	for (x = bsize_x-1; x >= 0; x--) {
		int k;
		k = x + y * bsize_x;
		k = (int)(*(p + k));

		//				k = (int)(temp_brush.at<unsigned char >(y, x));
		if (k < V_th) {
		//	cout << "B x : " << x << "," << y << " : " << v << endl;

			find_flag = true;
			break;
		}
	}
	if (find_flag == true) break;
}

if (find_flag == true)
{
	crop_e.y = y;

}
else {
	cout << "Never top e y" <<crop_e.y<< endl;
}
//cout <<  crop_s.x << ", " << crop_s.y << ", " << crop_e.x << ", " << crop_e.y << endl;

find_flag = false;

for (x = bsize_x-1; x >= 0; x--)
{
	for (y = bsize_y-1; y >= 0; y--) {
		int k;
		k = x + y * bsize_x;
		k = (int)(*(p + k));

		//				k = (int)(temp_brush.at<unsigned char >(y, x));
		if (k < V_th) {
		//	cout << "B y : " << x << "," << y << " : " << v << endl;

			find_flag = true;
			break;
		}
	}
	if (find_flag == true) break;
}

if (find_flag == true)
{
	crop_e.x = x;
}
else {
	cout << "Never top e.x " <<crop_e.x<< endl;
}
//cout << crop_s.x << ", " << crop_s.y << ", " << crop_e.x << ", " << crop_e.y << endl;
Rect r = Rect(crop_s, crop_e);
return r;
}