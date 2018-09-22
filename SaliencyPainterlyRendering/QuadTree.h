#pragma once
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"

typedef struct Imageinformation
{
	cv::Point srtPoint;
	cv::Point endPoint;
}Imginfo;

class Img_node
{
public:
	int depth;
	//Img_node *TL;
//	Img_node *TR;
	//Img_node *BL;
	//Img_node *BR;
	int chk = false;
	Imginfo info;

	int avgS;
	int no;

	bool operator<(const Img_node & other)
	{
		if (depth < other.depth)
			return true;
		else
			return false;
	};

};//Img_node;

class QuadTree
{
	
public:



	static Img_node *copyImageTree(Imginfo info, int depth, int S);
	static int  TakeQuadTree(cv::Mat &SaliencyMap, std::list<Img_node*> *aStroke,string tag);
	static cv::Mat TakeDensity(cv::Mat &srcImg, std::list<Img_node*> *aStroke);

	bool compareDepth_I(Img_node* A, Img_node* B);
};