/*************************************************

Copyright: Guangyu Zhong all rights reserved

Author: Guangyu Zhong

Date:2014-09-27

Description: codes for Manifold Ranking Saliency Detection
Reference http://ice.dlut.edu.cn/lu/Project/CVPR13[yangchuan]/cvprsaliency.htm

**************************************************/
#include "stdafx.h"
#include "opencv2\\opencv.hpp"
#include "opencv2\\core.hpp"
#include "..\\extern.h"
#include"saliency_PreGraph.h"
#include "..\\debug_image.h"
using namespace std;
using namespace cv;

Mat preGraph_main(Mat &srcImg)
{
	
//	Mat img = imread("a.ppm");
	PreGraph SpMat;
	Mat superpixels = SpMat.GeneSp(srcImg);
	Mat sal = SpMat.GeneSal(srcImg);
	//cout << sal;
	Mat salMap = SpMat.Sal2Img(superpixels, sal);
	Mat tmpsuperpixels;
	debug_image("saliency/superpixel", salMap);
	//normalize(salMap, tmpsuperpixels, 255.0, 0.0, NORM_MINMAX);
	mat_print(salMap,"sal_map");
	//tmpsuperpixels.convertTo(tmpsuperpixels, CV_8UC3, 1.0);
	//debug_image("0_prefraph.ppm", tmpsuperpixels);
	//waitKey();
	return salMap;
}
