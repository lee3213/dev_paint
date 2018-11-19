#pragma once
#include "stdafx.h"

#include <opencv2\\opencv.hpp>
#include <opencv2\\core.hpp>
#include <list>
//#include "extern.h"

using namespace std;
using namespace cv;

class Stroke_Node
{
public:
	int depth;
	cv::Point srtPoint;
	cv::Point endPoint;
	cv::Size stroke_size;
	int avgS;
	//int no; 
	Stroke_Node() {};
	Stroke_Node(cv::Point s, cv::Point e, int _depth, double S) {
		srtPoint = s;
		endPoint = e;
		stroke_size.width = e.x - s.x;
		stroke_size.height = e.y - s.y;
		
		depth = _depth;
		avgS = (int)S;
	}
	~Stroke_Node() {};
};
	/*
	bool ptrsorter(Img_node *a, Img_node *b) {
		return (a->depth < b->depth ? true : false);
	}
	bool operator<(const Img_node * other)
	{
		cout << "called< " << depth << " : " << other->depth << endl;
		if (depth < other->depth)
			return true;
		else
			return false;
	};
	bool operator>(const Img_node * other)
	{
		cout << "called> " << depth << " : " << other->depth << endl;
		if (depth > other->depth)
			return true;
		else
			return false;
	};
	bool compareDepth_I(Img_node& A, Img_node& B)
	{
		return (A.depth < B.depth ? true : false);
	}
	bool compareDepth_I(Img_node * A, Img_node* B)
	{
		return (A->depth < B->depth ? true : false);
	}
	*/
//Img_node;
//Stroke_Node a;

	//static Img_node *copyImageTree(Imginfo info, int depth, int S);
//	 int  TakeQuadTree_grid(cv::Mat &SaliencyMap, Stroke_set aStroke[],string tag);
	
	// cv::Mat TakeDensity(cv::Mat &srcImg, Stroke_set *aStroke);
	//static Img_node *newImageTree(cv::Point s, cv::Point e, int depth, double S);
	//static Img_node *copyImageTree(Img_node deno, int depth, int avgS);
	
