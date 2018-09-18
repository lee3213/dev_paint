#pragma once
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
class QuadTree
{
	
public:
	typedef struct Imageinformation
	{
		cv::Point srtPoint;
		cv::Point endPoint;
	}Imginfo;
	
	typedef struct ImageTree
{
	ImageTree *TL;
	ImageTree *TR;
	ImageTree *BL;
	ImageTree *BR;
	int chk = false;
	Imginfo info;
	int depth;
	int avgS;
	int no;
	}Img_node;

	static QuadTree::ImageTree *copyImageTree(QuadTree::Imginfo info, int depth, int S);
	static int  TakeQuadTree(cv::Mat &SaliencyMap, std::vector<QuadTree::Img_node*> &aStroke,string tag);
	static cv::Mat TakeDensity(cv::Mat &srcImg, std::vector<QuadTree::Img_node*> &aStroke);

	
};