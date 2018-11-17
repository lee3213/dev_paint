#include "stdafx.h"
#include "saliency_itti.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"

using namespace cv;
Mat saliency_itti_main(Mat srcImg,string tag_s){
	//imwrite("/rst/saliency_src.ppm", srcImg);
    SaliencyMap * SM  = new SaliencyMap  (srcImg.rows,srcImg.cols);
      // create retinal filter mask
   //   SM->createRetinalFilterMask(frame_size);
	cv::Mat sailImg=SM->SMGetSM	(srcImg,tag_s);
	//imwrite("/rst/saliency.ppm", sailImg);
	delete SM;
	return sailImg.clone();
}