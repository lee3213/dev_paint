#include "stdafx.h"

#include "QuadTree.h"
#include "extern.h"
#include "util.h"
#include "debug_image.h"
using namespace std;
using namespace cv;


Img_node * QuadTree::copyImageTree(Imginfo info, int depth, int avgS)
{

	Img_node *newT = new Img_node;
	if (newT != NULL) {
		newT->info = info;
	//	newT->TL = NULL;
	//	newT->TR = NULL;
		//original bug by cwlee newT->BL - NULL;

	//	newT->BL = NULL;
	//	newT->BR = NULL;
		newT->chk = false;
		newT->no = g_unique_id;
		//	cout << "new T " << newT->no << endl;
		g_unique_id++;
		newT->depth = depth;
		newT->avgS = avgS;
		return newT;
	}
	else return newT;// NULL;
}

Img_node * QuadTree::newImageTree(Imginfo info,int depth,double S)
{

	Img_node *newT = new Img_node;
	if (newT != NULL) {
		newT->info = info;
		//newT->TL = NULL;
		//newT->TR = NULL;
		//original bug by cwlee newT->BL - NULL;

		//newT->BL = NULL;
		//newT->BR = NULL;
		newT->chk = false;
		newT->no = g_unique_id;
		//	cout << "new T " << newT->no << endl;
		g_unique_id++;
		newT->depth = depth;
		newT->avgS = (int)S;
		return newT;
	}
	else return newT;// NULL;
}

//int cnt;

bool compareDepth(Img_node* A, Img_node* B)
{
	return A->depth < B->depth ? true : (A->depth == B->depth ? true : false);
}

void DrawEdgeLine(cv::Mat &srcImg, Imginfo info)
{
	unsigned char* srcData = (unsigned char*)srcImg.data;

	Point srtPoint, endPoint;
	srtPoint.x = info.srtPoint.x;
	srtPoint.y = info.srtPoint.y;
	endPoint.x = info.endPoint.x;
	endPoint.y = info.endPoint.y;

	for (int y = (int)srtPoint.y; y < (int)endPoint.y; y++)
	{
		for (int x = (int)srtPoint.x; x < (int)endPoint.x; x++)
		{
			size_t index = y * srcImg.step1() + x * srcImg.channels();
			if (x == (int)srtPoint.x || x == (int)endPoint.x - 1 || y == srtPoint.y || y == (int)endPoint.y - 1)
				srcData[index] = 255;
		}
	}
}

double TakeAvgS(cv::Mat &srcImg, Point srtPoint, Point endPoint)
{
	double avgS, S = 0;
	unsigned char *srcData = (unsigned char*)srcImg.data;
	int width = srcImg.size().width, height = srcImg.size().height;
	int s1 =(int) srcImg.step1();
	int sc=srcImg.channels();
	for (int y = (int)srtPoint.y; y <= (int)endPoint.y && y <= height; y++)
	{
		for (int x = (int)srtPoint.x; x <= (int)endPoint.x && x<= width; x++)
		{
			size_t index = y * s1+ x * sc;
			S += srcData[index];
		}
	}
	avgS = S / (width + height);

	return avgS;
}
double double_array_min_at(double a[], int n) {
	double min_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (min_v > a[i]) {
			min_v = a[i];
			loc = i;
		}
	return loc;
}
double double_array_min(double a[], int n) {
	double min_v = a[0];
	for (int i = 1; i < n; i++)
		if (min_v > a[i]) min_v = a[i];
	return min_v;
}
double double_array_max(double a[], int n) {
	double max_v = a[0];
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) max_v = a[i];
	return max_v;
}
double double_array_max_at(double a[], int n) {
	double max_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) {
			max_v = a[i];
			loc = i;
		}
	return loc;
}
double double_2xarray_min_at(double *a, int w,int h,int n) {
	double min_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (min_v < a[i]) {
			min_v = a[i];
			loc = i;
		}
	return loc;
}
double double_2xarray_min(double *a, int w,int h, int n) {
	double min_v = a[0];
	for (int i = 1; i < n; i++)
		if (min_v < a[i]) min_v = a[i];
	return min_v;
}
double double_2xarray_max(double *a, int w, int h, int n) {
	double max_v = a[0];
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) max_v = a[i];
	return max_v;
}
double double_2xarray_max_at(double *a, int w, int h, int n) {
	double max_v = a[0];
	int loc = 0;
	for (int i = 1; i < n; i++)
		if (max_v < a[i]) {
			max_v = a[i];
			loc = i;
		}
	return loc;
}
int get_selected(double *_D, int n,int divider, int &selected_x, int &selected_y) {
	int bias = 1;
	int x, y;
	double v = 0.0;
	selected_x = 0; selected_y = 0;
	for(x =bias;x<divider;x++)
		for (y = bias; y <= divider; y++) {
		//	cout << x << "," << y << " : " << *(_D + (y*n + x)) << endl;
			if (v < *(_D + (y*n + x))) {
				selected_x = x;
				selected_y = y;
			}
		}
	return 1;
};
Point get_midPoint(cv::Mat &srcImg,Point s, Point e, double *D,int d) {
	int divider = g_QT_method_N +1;
	Point midPoint;
	Point candidate[10][10];
	bool size_err=false;
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

		if (length_width < g_min_gridsize) size_err = true;
		if (length_height < g_min_gridsize) size_err = true;
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
						P[h][w].S[0] = TakeAvgS(srcImg, candidate[0][0]/*s*/, candidate[h][w]/*p*/);//LU
						P[h][w].S[1] = TakeAvgS(srcImg, candidate[0][w], candidate[h][divider]);//RU
						P[h][w].S[2] = TakeAvgS(srcImg, candidate[h][w], candidate[divider][divider]);//RD
						P[h][w].S[3] = TakeAvgS(srcImg, candidate[h][0], candidate[divider][w]);//LD

						min_s = double_array_min(P[h][w].S, 4);
						max_s = double_array_max(P[h][w].S, 4);
						_D[h][w] = max_s - min_s;
					}

				}
			
			}
#ifdef _PRINT
			std::cout << setw(7) << "w\\h" << ": ";

			for (int w = 0; w < divider; w++)
				std::cout << setw(12) << w;
			std::cout << endl;
			for (int h = 1; h < divider; h++) {
				std::cout << h << ": ";
				for (int w = 1; w < divider; w++) {
					std::cout << "("<<setw(4) << candidate[h][w].x << ", " << setw(4) << candidate[h][w].y<<")";
		
				}
				std::cout << endl;
			}
			std::cout << endl;
			std::cout << setw(7) << "w\\h" << ": ";
		
				for (int k = 0; k < 4; k++)
					std::cout << setw(13) << k  ;
				std::cout << endl;
			for (int h = 1; h < divider; h++) {
			
				for (int w = 1; w < divider; w++) {
					std::cout << setw(3) << w <<","<< setw(3)<<h<<": ";
					for (int k = 0; k < 4; k++)
						std::cout << setw(11) << P[h][w].S[k] << ", ";

					std::cout << setw(11) << _D[h][w];
					std::cout << endl ;
				}

				std::cout << endl;

			}//h
			std::cout << endl;
#endif
			get_selected(&_D[0][0],10,divider, selected_x, selected_y);
			midPoint = candidate[selected_x][selected_y];
		
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


int DivideImage(cv::Mat &SaliencyMap, Img_node* me_node, list<Img_node*> aStroke_set[],
	 string  quad,Mat & gradient_src,Mat stageMap,int depth,string tag)
{
	Point srtPoint, endPoint, midPoint;
	double D[4];
	static int called = 0;
	int get_depth=depth;
		Imginfo child_Info, my_Info;
		Img_node* BL, *BR, *TL, *TR;
	//	cout << "----" << quad << "  --------------------------------------------------------" << endl;
		if (me_node == NULL)
		{
			cout << "NULL " << endl;
			return -1;
		}
		my_Info = me_node->info;
	//	DrawEdgeLine(Quadtree_map, my_Info);
	//	rectangle(Quadtree_map, Rect(my_Info.srtPoint,my_Info.endPoint),RGB(255, 255, 255));


		//Depth의 값을 오름차순으로 저장
		//list<Img_node*>::iterator Qt_it = lower_bound(aStroke_set->begin(), aStroke_set->end(), me_node, compareDepth);
		aStroke_set[me_node->depth].push_back(me_node);

		srtPoint = my_Info.srtPoint;
		endPoint = my_Info.endPoint;
		rectangle(stageMap, srtPoint, endPoint, RGB(255, 255, 255));
	/*
		g_QT_grid_size[parent_node->depth].width = endPoint.x - srtPoint.x;
		g_QT_grid_size[parent_node->depth].height = endPoint.y - srtPoint.y;
		*/
		//double avgS = TakeAvgS(SaliencyMap, parent_node->info.srtPoint, parent_node->info.endPoint);
	//	me->avgS = (int)parent_S;
		string called_str=std::string(3, '0').append(to_string(called));
		if ( called < 30)
				debug_image(string("grid/stage_"+tag) + called_str+string("_")+to_string(me_node->depth) +
					string("_") + quad ,
					stageMap);
		stageMap = gradient_src;
		called++;

		int child_QT_depth=me_node->depth+1;
		/*
		TL 2 | 1 TR
		-----------
		BL 3 | 4 BR
		*/
		
		if (child_QT_depth < g_depth_limit) {

			midPoint = get_midPoint(SaliencyMap, srtPoint, endPoint, D, child_QT_depth);
			//	cout << "r no : "<<resultinfo->no <<", "<<info.srtPoint.x<<", "<<info.srtPoint.y<< endl;
			if (midPoint.x != 0 && midPoint.y != 0) {
				child_Info.srtPoint = srtPoint;
				child_Info.endPoint = midPoint;
				//2사분면
			
				if ((child_Info.endPoint.y - child_Info.srtPoint.y) >= g_min_gridsize
					&& (child_Info.endPoint.x - child_Info.srtPoint.x) >= g_min_gridsize)
				{

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint);
					if (avgS > g_QT_avgSThreshold ) {
						TL = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);

						get_depth=DivideImage(SaliencyMap,TL, aStroke_set,
							string("TL") + to_string(child_QT_depth),stageMap, gradient_src,depth,tag);
						if (get_depth > depth)
							get_depth = depth;

					}
					

				}
				
				//1사분면
	
				child_Info.srtPoint.x = midPoint.x, child_Info.srtPoint.y = srtPoint.y;
				child_Info.endPoint.x = endPoint.x, child_Info.endPoint.y = midPoint.y;
				if (child_Info.endPoint.y - child_Info.srtPoint.y >= g_min_gridsize
					&& child_Info.endPoint.x - child_Info.srtPoint.x >= g_min_gridsize)
				{

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint);
					if (avgS > g_QT_avgSThreshold) {
						TR = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, TR, aStroke_set,
							string("TR") + to_string(child_QT_depth), stageMap, gradient_src,depth,tag);
						if (get_depth > depth)
							get_depth = depth;
					}
					

				}
				
				//3사분면
				child_Info.srtPoint.x = srtPoint.x, child_Info.srtPoint.y = midPoint.y;
				child_Info.endPoint.x = midPoint.x, child_Info.endPoint.y = endPoint.y;
				if (child_Info.endPoint.y - child_Info.srtPoint.y >= g_min_gridsize
					&& child_Info.endPoint.x - child_Info.srtPoint.x >= g_min_gridsize)
				{

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint);
					if (avgS > g_QT_avgSThreshold) {
						BL = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, BL, aStroke_set,
							string("BL") + to_string(child_QT_depth), stageMap, gradient_src,depth,tag);
						if (get_depth > depth)
							get_depth = depth;
					}
					

				}
			
				//4사분면
				child_Info.srtPoint.x = midPoint.x, child_Info.srtPoint.y = midPoint.y;
				child_Info.endPoint.x = endPoint.x, child_Info.endPoint.y = endPoint.y;
				if (child_Info.endPoint.y - child_Info.srtPoint.y >= g_min_gridsize
					&& child_Info.endPoint.x - child_Info.srtPoint.x >= g_min_gridsize)
				{

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint);
					if (avgS > g_QT_avgSThreshold) {
						BR = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, BR, aStroke_set,
							string("BR") + to_string(child_QT_depth), stageMap, gradient_src,depth,tag);
						if (get_depth > depth)
							get_depth = depth;
					}
				

				}
				
			}//midPoint==0;
		

		}//child 
		
		return get_depth;
	}
//g_QT_method_N

	int  QuadTree :: TakeQuadTree(cv::Mat &SaliencyMap,  list<Img_node*> aStroke_set[],string tag)
{
	//cv::Mat Quad_treeMap;
	Mat Quad_treeMap = SaliencyMap.clone();
	Mat stageMap= SaliencyMap.clone();
	int last_depth = 0;

	int width = SaliencyMap.size().width, height = SaliencyMap.size().height;
	unsigned char* srcData = (unsigned char*)SaliencyMap.data;
	 Img_node* root;
	 Imginfo root_Info;
	 root_Info.srtPoint.x = 0;
	 root_Info.srtPoint.y = 0;
	 root_Info.endPoint.x = width-1 ; 
	 root_Info.endPoint.y = height-1;
	 double avgS = TakeAvgS(SaliencyMap, root_Info.srtPoint, root_Info.endPoint);
	 root = newImageTree(root_Info, 0,avgS);
	
	 last_depth=DivideImage(SaliencyMap, root, aStroke_set, string("root") + to_string(0), Quad_treeMap, stageMap,0,tag);
		 //	cout		<< "after  aStroke size" << aStroke.size() << endl;
	
		
	//cv::String f_path = cv::format("%s/org_quad_tree.ppm", g_para_method_path.c_str());//1ch
	//cv::imwrite(f_path, Quad_treeMap);
	//return Quad_treeMap;
	 return last_depth;
}

cv::Mat QuadTree::TakeDensity(cv::Mat &srcImg, list<Img_node*> *aStroke)
{
	cv::Mat densityMap = srcImg.clone();
	unsigned char* densityData = (unsigned char*)densityMap.data;

	for (list<Img_node*>::iterator it = aStroke->begin(); it != aStroke->end(); it++)
	{
		int density = 255 / (g_depth_limit - 1) * (g_depth_limit - (*it)->depth);
		Point srtPoint, endPoint;
		srtPoint.x = (*it)->info.srtPoint.x, srtPoint.y = (*it)->info.srtPoint.y;
		endPoint.x = (*it)->info.endPoint.x, endPoint.y = (*it)->info.endPoint.y;

		for (int y = (int)srtPoint.y; y < (int)endPoint.y; y++)
		{
			for (int x = (int)srtPoint.x; x < (int)endPoint.x; x++)
			{
				size_t index = y * densityMap.step1() + x * densityMap.channels();
				//densityData[index] = density;
				densityData[index] = (*it)->depth;
			}
		}
	}
	return densityMap;
}