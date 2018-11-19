#pragma once
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

Mat preGraph_main(Mat &);
Mat saliency_itti_main(Mat srcImg,string tag);
Mat saliency_blackandwhite_main(Mat &srcImg);
Mat saliency_Perazzi_main(Mat &srcImg);