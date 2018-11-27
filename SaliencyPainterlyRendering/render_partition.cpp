#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "define.h"
#include "extern.h"

#include "brush.h"
#include "QuadTree.h"
#include "debug_image.h"

#include "render_.h"
int  render_::TakeQuadTree(cv::Mat &SaliencyMap, Region_set aRegion_set[], string tag)
{
	//cv::Mat Quad_treeMap;
	Mat Quad_treeMap = SaliencyMap.clone();
	Mat stageMap = SaliencyMap.clone();
	int last_depth = 0;

	int width = SaliencyMap.size().width, height = SaliencyMap.size().height;
	unsigned char* srcData = (unsigned char*)SaliencyMap.data;
	partition_Node* root;
	// Imginfo root_Info;
	string quad = "0_whole";

	double avgS = TakeAvgS(SaliencyMap, Point(0, 0), Point(width - 1, height - 1), 0, quad);
	root = new partition_Node(Point(0, 0), Point(width - 1, height - 1), 0, avgS,g_no);
	g_no++;
	/*	 root.srtPoint.x = 0;
	root_Info.srtPoint.y = 0;
	root_Info.endPoint.x = width-1 ;
	root_Info.endPoint.y = height-1;


	root = newImageTree(root_Info, 0,avgS);

	*/
	last_depth = DivideImage(SaliencyMap, root, aRegion_set, string("root"), Quad_treeMap, stageMap, 0, tag);
	//	cout		<< "after  aStroke size" << aStroke.size() << endl;
	
	//cv::String f_path = cv::format("%s/org_quad_tree.ppm", g_para_method_path.c_str());//1ch
	//cv::imwrite(f_path, Quad_treeMap);
	//return Quad_treeMap;
	return last_depth;
}

int render_::DivideImage(cv::Mat &SaliencyMap, partition_Node* me_node, Region_set aRegion_set[],
	string  quad,
	Mat & gradient_src,
	Mat stageMap, int depth, string tag)
{
	Point p_srtPoint, p_endPoint, p_midPoint;
	Point c_srtPoint[5], c_endPoint[5];
	double D[4];
	static int called = 0;
	int get_depth;
	double avgS[5];
	//Point child_Info_s, child_Info_e;//,my_Info;
	//Img_node* BL, *BR, *TL, *TR;
	partition_Node *new_node[5];
	partition_Node *new_dropped_node;
	//	cout << "----" << quad << "  --------------------------------------------------------" << endl;
	if (me_node == NULL)
	{
		cout << "NULL " << endl;
		return -1;
	}
	p_srtPoint = me_node->srtPoint;
	p_endPoint = me_node->endPoint;

	string called_str = std::string(3, '0').append(to_string(called));
	if (called < 30) {
		rectangle(stageMap, p_srtPoint, p_endPoint, RGB(255, 255, 255));
		debug_image(string("grid/stage_" + tag) + called_str + string("_") + to_string(me_node->depth) +
			string("_") + quad,
			stageMap);
		stageMap = gradient_src;
	}
	called++;

	int ret = aRegion_set[me_node->depth].push_back(me_node);
	if (ret < 0)
	{
		cout << "ERROR stroke_Set FULL " << me_node->depth << endl;
		return -1;
	}



	/*
	TL 2 | 1 TR
	-----------
	BL 3 | 4 BR
	*/


	int child_QT_depth = me_node->depth + 1;


	if (child_QT_depth >= g_depth_limit)
		return child_QT_depth;


	p_midPoint = get_midPoint(SaliencyMap, p_srtPoint, p_endPoint, D, child_QT_depth, child_QT_depth);


	//if (p_midPoint.x != 0 && p_midPoint.y != 0) {

	//2»çºÐ¸é

	for (int quadrant = 1; quadrant <= 4; quadrant++) {
		quad = to_string(quadrant);
		switch (quadrant) {
		case 1:
			c_srtPoint[quadrant].x = p_midPoint.x, c_srtPoint[quadrant].y = p_srtPoint.y;
			c_endPoint[quadrant].x = p_endPoint.x, c_endPoint[quadrant].y = p_midPoint.y;
			break;
		case 2:
			c_srtPoint[quadrant] = p_srtPoint;
			c_endPoint[quadrant] = p_midPoint;
			break;
		case 3:
			c_srtPoint[quadrant].x = p_srtPoint.x, c_srtPoint[quadrant].y = p_midPoint.y;
			c_endPoint[quadrant].x = p_midPoint.x, c_endPoint[quadrant].y = p_endPoint.y;
			break;
		case 4:
			c_srtPoint[quadrant].x = p_midPoint.x, c_srtPoint[quadrant].y = p_midPoint.y;
			c_endPoint[quadrant].x = p_endPoint.x, c_endPoint[quadrant].y = p_endPoint.y;
			break;
		}

		if ((c_endPoint[quadrant].y - c_srtPoint[quadrant].y) >= g_min_gridsize
			&& (c_endPoint[quadrant].x - c_srtPoint[quadrant].x) >= g_min_gridsize)
		{

			avgS[quadrant] = TakeAvgS(SaliencyMap, c_srtPoint[quadrant], c_endPoint[quadrant], child_QT_depth, quad);
		//	if (avgS[quadrant] != D[quadrant - 1]) {
			//	cout << "depth mismatch " << child_QT_depth << " " << quad << "["<<D[quadrant - 1] << "] != [" << avgS[quadrant] <<"]"<< endl;
		//	}
			if (avgS[quadrant] > g_QT_avgSThreshold) {
				new_node[quadrant] = new partition_Node(c_srtPoint[quadrant], c_endPoint[quadrant], child_QT_depth, avgS[quadrant],g_no);
				g_no++;

				get_depth = DivideImage(SaliencyMap, new_node[quadrant], aRegion_set,
					quad + to_string(child_QT_depth), stageMap, gradient_src, child_QT_depth, tag);

			}
			else
			{
				new_dropped_node = new partition_Node(c_srtPoint[quadrant], c_endPoint[quadrant], child_QT_depth, avgS[quadrant], dropped_no);
				dropped_no++;
				int ret = render_dropped_set[new_dropped_node->depth].push_back(new_dropped_node);
				if (ret < 0)
				{
					cout << "ERROR dropped_stroke_Set push_back Error " << new_dropped_node << endl;
					return -1;
				}
			}//if avgS



		}//if mion_grid_Size

	}//for quadrant 

	return get_depth;
}
Point render_::get_midPoint(cv::Mat &srcImg, Point s, Point e, double *D, int d, int child_QT_depth) {
	int divider = g_QT_method_N + 1;
	Point midPoint;
	Point candidate[10][10];
	bool size_err = false;
	struct {
		double S[4];
	}P[10][10];
	double min_s, max_s;
	double _D[10][10];
	int selected_x = 1, selected_y = 1;
	//std::cout << "depth " << d << endl;
	//std::cout << "s "<<setw(4)<<s.x << " ," << setw(4)<<s.y << endl;

	//std::cout << "e "<<setw(4) << e.x << " ," << setw(4) << e.y << endl;
	/*
	if (g_QT_method_N == 1) {
	midPoint.x = (e.x - s.x)/2;
	midPoint.y = (e.y - s.y)/2;
	}
	else
	*/
	midPoint.x = 0; midPoint.y = 0;

	if (g_QT_method_N < 4) {

		int length_width = (int)((e.x - s.x) / divider);
		int length_height = (int)((e.y - s.y) / divider);
		candidate[0][0] = s;

		//std::cout << "l " << setw(4) << length_width << " ," << setw(4) << length_height << endl;
		/*
		if (length_width < g_min_gridsize) size_err = true;
		if (length_height < g_min_gridsize) size_err = true;
		*/
		/*
		s,s     s+w,s    s+2w,s     s+3w,s    s+4w=e,s
		s,s+h   s+w.s+h  s+2w,s+h   s+2w,s+h  s+4w,s+h
		s,s+2h  s+w.s+2h s+2w,s+2h  s+2w,s+2h s+4w,s+2h
		s,s+3h  s+w.s+3h s+2w,s+3h  s+2w,s+3h s+4w,s+3h
		s,s+4h  s+w.s+4h s+2w,s+4h  s+2w,s+4h s+4w,s+4h
		*/
		if (!size_err) {
			for (int h = 0; h <= divider; h++)
				for (int w = 0; w <= divider; w++) {
					for (int k = 0; k < 4; k++)
						P[h][w].S[k] = 0.0;
					_D[h][w] = 0.0;
					if (h == 0 && w == 0) candidate[0][0] = s;

					else {
						if (w != divider)
							candidate[h][w].x = s.x + w*length_width;
						else
							candidate[h][w].x = e.x;

						if (h != divider)
							candidate[h][w].y = s.y + h*length_height;
						else
							candidate[h][w].y = e.y;


					}

				}

			for (int h = 1; h < divider; h++) {

				for (int w = 1; w < divider; w++) {
					if (h >= 1 && w >= 1) {//clock wise
						P[h][w].S[0] = TakeAvgS(srcImg, candidate[0][0]/*s*/, candidate[h][w]/*p*/, child_QT_depth, "KK");//LU
						P[h][w].S[1] = TakeAvgS(srcImg, candidate[0][w], candidate[h][divider], child_QT_depth, "KK");//RU
						P[h][w].S[2] = TakeAvgS(srcImg, candidate[h][w], candidate[divider][divider], child_QT_depth, "KK");//RD
						P[h][w].S[3] = TakeAvgS(srcImg, candidate[h][0], candidate[divider][w], child_QT_depth, "KK");//LD

						min_s = double_array_min(P[h][w].S, 4);
						max_s = double_array_max(P[h][w].S, 4);
						_D[h][w] = max_s - min_s;
					}

				}

			}

			get_selected(&_D[0][0], 10, divider, selected_x, selected_y);
			midPoint = candidate[selected_x][selected_y];
			if (midPoint == s) {

				std::cout << setw(7) << "w\\h" << ": ";

				for (int w = 0; w < divider; w++)
					std::cout << setw(12) << w;
				std::cout << endl;
				for (int h = 1; h < divider; h++) {
					std::cout << h << ": ";
					for (int w = 1; w < divider; w++) {
						std::cout << "(" << setw(4) << candidate[h][w].x << ", " << setw(4) << candidate[h][w].y << ")";

					}
					std::cout << endl;
				}
				std::cout << endl;
				std::cout << setw(7) << "w\\h" << ": ";

				for (int k = 0; k < 4; k++)
					std::cout << setw(13) << k;
				std::cout << endl;
				for (int h = 1; h < divider; h++) {

					for (int w = 1; w < divider; w++) {
						std::cout << setw(3) << w << "," << setw(3) << h << ": ";
						for (int k = 0; k < 4; k++)
							std::cout << setw(11) << P[h][w].S[k] << ", ";

						std::cout << setw(11) << _D[h][w];
						std::cout << endl;
					}

					std::cout << endl;

				}//h
				std::cout << endl;

			}
		}//size_err
	} //epth < 4
	  //D=_D[?]
	else {
		//out << "====================" << endl;

		//out << "====================" << endl;
	}
	//std::cout << "mid " << midPoint.x << ", " << midPoint.y << endl;
	//cout << "selected " << selected_x << ", " << selected_y << endl;

	return midPoint;
}

double render_::TakeAvgS(cv::Mat &srcImg, Point srtPoint, Point endPoint, int depth, string quad)
{
	double avgS, S = 0;
	unsigned char *srcData = (unsigned char*)srcImg.data;
	int width = srcImg.size().width, height = srcImg.size().height;
	int s1 = (int)srcImg.step1();
	int sc = srcImg.channels();
	int s_w = endPoint.x - srtPoint.x;
	int s_h = endPoint.y - srtPoint.y;
	for (int y = (int)srtPoint.y; y <= (int)endPoint.y && y <= height; y++)
	{
		for (int x = (int)srtPoint.x; x <= (int)endPoint.x && x <= width; x++)
		{
			size_t index = y * s1 + x * sc;
			S += srcData[index];
		}
	}
	avgS = S / (g_qt_s_scale*(s_w + s_h));
#ifdef TRACE_AVGS
	if (depth >= g_trace_depth) {
		if (quad != "KK") {
			cout << "depth " << depth << " :" << quad << ": (x,y) " << setw(5) << srtPoint.x << " " << srtPoint.y <<
				" :e( " << endPoint.x << " " << endPoint.y;

			cout << " s_ : " << setw(5) << s_w << setw(5) << s_h << " 2*w*h= " << g_qt_s_scale*(s_w + s_h)
				<< " S " << setw(15) << S << setw(15) << " avgS : " << avgS << endl;
		}
	}
#endif
	return avgS;
}
double  render_::double_array_min_at(double a[], int n) {
	double min_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (min_v > a[i]) {
			min_v = a[i];
			loc = i;
		}
	return loc;
}
double  render_::double_array_min(double a[], int n) {
	double min_v = a[0];
	for (int i = 1; i < n; i++)
		if (min_v > a[i]) min_v = a[i];
	return min_v;
}
double  render_::double_array_max(double a[], int n) {
	double max_v = a[0];
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) max_v = a[i];
	return max_v;
}
double  render_::double_array_max_at(double a[], int n) {
	double max_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) {
			max_v = a[i];
			loc = i;
		}
	return loc;
}
double  render_::double_2xarray_min_at(double *a, int w, int h, int n) {
	double min_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (min_v < a[i]) {
			min_v = a[i];
			loc = i;
		}
	return loc;
}
double  render_::double_2xarray_min(double *a, int w, int h, int n) {
	double min_v = a[0];
	for (int i = 1; i < n; i++)
		if (min_v < a[i]) min_v = a[i];
	return min_v;
}
double  render_::double_2xarray_max(double *a, int w, int h, int n) {
	double max_v = a[0];
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) max_v = a[i];
	return max_v;
}
double  render_::double_2xarray_max_at(double *a, int w, int h, int n) {
	double max_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) {
			max_v = a[i];
			loc = i;
		}
	return loc;
}
int render_::get_selected(double *_D, int n, int divider, int &selected_x, int &selected_y) {
	int bias = 1;
	int x, y;
	double v = 0.0;
	selected_x = 1; selected_y = 1;
	for (x = bias; x<divider; x++)
		for (y = bias; y <= divider; y++) {
			//	cout << x << "," << y << " : " << *(_D + (y*n + x)) << endl;
			if (v < *(_D + (y*n + x))) {
				selected_x = x;
				selected_y = y;
			}
		}
	return 1;
};

void render_::proof_box(Point &s, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << s.y << endl;

}

void render_::proof_box(Point &s, int i_width, int i_height, char * p) {
	//Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << p << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << p << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << p << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << p << s.y << endl;

}
