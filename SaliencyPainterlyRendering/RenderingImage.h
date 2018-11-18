#pragma once
#include <random>
#include "opencv2\\opencv.hpp"
#include "opencv2\\core.hpp"
#include "bsShowImage.h"
#include "brush.h"
#include "util.h"
#include "RenderingImage.h"
#include "QuadTree.h"
#include "Judgement.h"
#include "brush.h"
#include "extern.h"
#include "debug_define.h"
#include "util.h"

#include "render_.h"
using namespace std;
using namespace cv;



int RenderingImage(char * srcImgname, char * saliImgname);
//void RenderingImage(cv::Mat &srcImg, cv::Mat &saliImg);


//cv::Mat &srcImg, list<QuadTree::Img_node*> aStroke, string tag,

////	int saliency_brush_size[], int saliency_depth, Mat saliency_grid_map[], int saliency_Sgrid_painting_try[],
//	vector <Brush*>& saliency_brush_set, int saliency_QT_grid_count[],int changed_count[],
//	long int _paintng_area[]

//);
