
#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;
void cvQuiver(Mat &Image, int x, int y, int u, int v, CvScalar Color,
	int Size, int Thickness) {
	cv::Point pt1, pt2;
	double Theta;
	double PI = 3.1416;

	if (u == 0)
		Theta = PI / 2;
	else
		Theta = atan2(double(v), (double)(u));

	pt1.x = x;
	pt1.y = y;

	pt2.x = x + u;
	pt2.y = y + v;

	cv::line(Image, pt1, pt2, Color, Thickness, 8);  //Draw Line

	/*
	Size = (int)(Size*0.707);


	if (Theta == PI / 2 && pt1.y > pt2.y)
	{
		pt1.x = (int)(Size*cos(Theta) - Size*sin(Theta) + pt2.x);
		pt1.y = (int)(Size*sin(Theta) + Size*cos(Theta) + pt2.y);
		cv::line(Image, pt1, pt2, Color, Thickness, 8);  //Draw Line

		pt1.x = (int)(Size*cos(Theta) + Size*sin(Theta) + pt2.x);
		pt1.y = (int)(Size*sin(Theta) - Size*cos(Theta) + pt2.y);
		cv::line(Image, pt1, pt2, Color, Thickness, 8);  //Draw Line
	}
	else {
		pt1.x = (int)(-Size*cos(Theta) - Size*sin(Theta) + pt2.x);
		pt1.y = (int)(-Size*sin(Theta) + Size*cos(Theta) + pt2.y);
		cv::line(Image, pt1, pt2, Color, Thickness, 8);  //Draw Line

		pt1.x = (int)(-Size*cos(Theta) + Size*sin(Theta) + pt2.x);
		pt1.y = (int)(-Size*sin(Theta) - Size*cos(Theta) + pt2.y);
		cv::line(Image, pt1, pt2, Color, Thickness, 8);  //Draw Line
	}
	*/
}