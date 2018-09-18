#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "extern.h"
#include "QuadTree.h"
#include "debug_image.h"
using namespace cv;
int // return depth
draw_grid_2(Mat _Quad_TreeMap,
	vector<QuadTree::Img_node*> aStroke_set, string tag, int & depth, int draw_depth,int c) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];


	for (int i = 0; i <MAX_DEPTH; i++) {
		overlay_grid_map[i] = _Quad_TreeMap.clone();
	}


	for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set.begin(); St_it != aStroke_set.end(); St_it++)
	{
		if (draw_depth != -1 && __saved_depth != draw_depth) {
			continue;
		}
		if (__saved_depth != (*St_it)->depth && __saved_depth != -1) {

			if (draw_depth == -1 || __saved_depth == draw_depth)
				debug_image("ing/_o_" + tag +"_" + to_string(__saved_depth), overlay_grid_map[__saved_depth]);
		}
		if (__saved_depth < (*St_it)->depth)
			__saved_depth = (*St_it)->depth;

		if (draw_depth == -1 || __saved_depth == draw_depth)
			cv::rectangle(overlay_grid_map[__saved_depth], Rect((*St_it)->info.srtPoint,
			(*St_it)->info.endPoint), RGB(c, c, c));

	}//end of QuadTree dump

	if (draw_depth == -1 || __saved_depth == draw_depth)
		debug_image("ing/_o_" + tag + "_" + to_string(__saved_depth) + "_f", overlay_grid_map[__saved_depth]);


	return depth;
}