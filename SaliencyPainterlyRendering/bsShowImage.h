#pragma once
#include "stdafx.h"
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
;
using namespace cv;

class bsShowImage
{
public:
	static void show(const char* winname, const cv::Mat& img, int delay = 1);
	static void save(const char* winname, const cv::Mat& img, int delay = 1);
	static char getKey();
	static cv::Mat TakeGaussianBlurImg(cv::Mat srcImg);
	static cv::Mat RGB2GRAY(cv::Mat srcImg);
	static cv::Mat TakeGradient(cv::Mat srcImg);
	static cv::Mat TakeSaliencyMap(cv::Mat saliImg, cv::Mat gradImg);
protected:
	static char key;
};