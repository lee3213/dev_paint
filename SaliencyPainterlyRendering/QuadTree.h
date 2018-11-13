#pragma once
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include <list>
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
	bool ptrsorter(Img_node *a, Img_node *b) {
		return (a->depth < b->depth ?true:false);
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
};//Img_node;

class QuadTree
{
	
public:



	//static Img_node *copyImageTree(Imginfo info, int depth, int S);
	static int  TakeQuadTree_grid(cv::Mat &SaliencyMap, std::list<Img_node*> aStroke[],string tag);
	static int  TakeQuadTree(cv::Mat &SaliencyMap, std::list<Img_node*> aStroke[], string tag);
	static cv::Mat TakeDensity(cv::Mat &srcImg, std::list<Img_node*> *aStroke);
	static Img_node *newImageTree(Imginfo info, int depth, double S);
	static Img_node *copyImageTree(Imginfo info, int depth, int avgS);
	
};