#include "stdafx.h"

#include "QuadTree.h"
#include "extern.h"
#include "util.h"
#include "debug_image.h"
using namespace std;
using namespace cv;

//double TakeAvgS(cv::Mat &srcImg, Point srtPoint, Point endPoint, int depth,string quad);
/*
Img_node * QuadTree::copyImageTree(Img_node info, int depth, int avgS)
{

	Img_node *newT = new Img_node(info.srtPoint,info.endPoint);
	if (newT != NULL) {
	
	//	newT->chk = false;
	//	newT->no = g_unique_id;
		//	cout << "new T " << newT->no << endl;
		g_unique_id++;
		newT->depth = depth;
		newT->avgS = avgS;
		return newT;
	}
	else return newT;// NULL;
}

*/

//int cnt;
/*
bool compareDepth(Img_node* A, Img_node* B)
{
	return (A->depth < B->depth ? true : false);
}
*/
/*
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
*/



				/* OLD CODE
				//1사분면
	
				child_Info.srtPoint.x = midPoint.x, child_Info.srtPoint.y = srtPoint.y;
				child_Info.endPoint.x = endPoint.x, child_Info.endPoint.y = midPoint.y;
				if (child_Info.endPoint.y - child_Info.srtPoint.y >= g_min_gridsize
					&& child_Info.endPoint.x - child_Info.srtPoint.x >= g_min_gridsize)
				{

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint, child_QT_depth,quad);
					if (child_QT_depth >= g_trace_depth) {
						cout << depth << " TR " << child_QT_depth << " avgS : " << avgS << " x,y( " <<
							(child_Info.endPoint.x - child_Info.srtPoint.x) << " " <<
							(child_Info.endPoint.y - child_Info.srtPoint.y) << " " <<
							")  g_size= " << g_min_gridsize << " g_S_th " << g_QT_avgSThreshold <<
							endl;
					}
					if (avgS > g_QT_avgSThreshold) {
						TR = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, TR, aRegion_set,
							string("TR") + to_string(child_QT_depth), stageMap, gradient_src, child_QT_depth,tag);
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

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint, child_QT_depth,quad);
					if (child_QT_depth >= g_trace_depth) {
						cout << depth << "  BL " << child_QT_depth << " avgS : " << avgS << " x,y( " <<
							(child_Info.endPoint.x - child_Info.srtPoint.x) << " " <<
							(child_Info.endPoint.y - child_Info.srtPoint.y) << " " <<
							")  g_size= " << g_min_gridsize << " g_S_th " << g_QT_avgSThreshold <<
							endl;
					}
					if (avgS > g_QT_avgSThreshold) {
						BL = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, BL, aRegion_set,
							string("BL") + to_string(child_QT_depth), stageMap, gradient_src, child_QT_depth,tag);
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

					double avgS = TakeAvgS(SaliencyMap, child_Info.srtPoint, child_Info.endPoint, child_QT_depth,quad);
					if (child_QT_depth >= g_trace_depth) {
						cout << depth << " BR " << child_QT_depth << " avgS : " << avgS << " x,y( " <<
							(child_Info.endPoint.x - child_Info.srtPoint.x) << " " <<
							(child_Info.endPoint.y - child_Info.srtPoint.y) << " " <<
							")  g_size= " << g_min_gridsize << " g_S_th " << g_QT_avgSThreshold <<
							endl;
					}
					if (avgS > g_QT_avgSThreshold) {
						BR = QuadTree::newImageTree(child_Info, child_QT_depth, avgS);
						DivideImage(SaliencyMap, BR, aRegion_set,
						string("BR") + to_string(child_QT_depth), stageMap, gradient_src, child_QT_depth,tag);
						if (get_depth > depth)
							get_depth = depth;
					}
				

				}
				*/
			//}//midPoint==0;
		

		//}//child 
		

//g_QT_method_N


/*
	int  QuadTree::TakeQuadTree_grid(cv::Mat &SaliencyMap, list<Img_node*> aRegion_set[], string tag)
	{
		//cv::Mat Quad_treeMap;
		Mat Quad_treeMap = SaliencyMap.clone();
		Mat stageMap = SaliencyMap.clone();
		int last_depth = 0;
		string quad = "0_grid";
		int width = SaliencyMap.size().width, height = SaliencyMap.size().height;
		unsigned char* srcData = (unsigned char*)SaliencyMap.data;
		Img_node* root;
		//Imginfo block_x;
	//	root_Info.srtPoint.x = 0;
		//root_Info.srtPoint.y = 0;
		//root_Info.endPoint.x = width - 1;
		//root_Info.endPoint.y = height - 1;
		int step_x = 30;
		int step_y = 30;
		int block_count_x = (width / step_x);
		int block_count_y = (height / step_y);

		for(int y=0;y<=block_count_y;y++)
			for (int x = 0; x <=block_count_x; x++) {
				block_x.srtPoint.x = x*step_x;
				block_x.srtPoint.y = y*step_y;
				block_x.endPoint.x = (x+1)*step_x;
				block_x.endPoint.y = (y+1)*step_y;
				if (block_x.endPoint.x > width)
					block_x.endPoint.x = width;
				if (block_x.endPoint.y > height)
					block_x.endPoint.y = height;
				double avgS = TakeAvgS(SaliencyMap, block_x.srtPoint, block_x.endPoint, 0,quad);
				if (avgS > 250) {
					root = newImageTree(block_x, 0, avgS);
					aRegion_set[root->depth].push_back(root);

				}
			}
		
		
		//double avgS = TakeAvgS(SaliencyMap, root_Info.srtPoint, root_Info.endPoint, 0);
	//	root = newImageTree(root_Info, 0, avgS);

		//last_depth = DivideImage(SaliencyMap, root, aRegion_set, string("root") + to_string(0), 
			//Quad_treeMap, stageMap, 0, tag);
		//	cout		<< "after  aStroke size" << aStroke.size() << endl;


		//cv::String f_path = cv::format("%s/org_quad_tree.ppm", g_para_method_path.c_str());//1ch
		//cv::imwrite(f_path, Quad_treeMap);
		//return Quad_treeMap;
		return last_depth;
	}
	*/
	/*
cv::Mat TakeDensity(cv::Mat &srcImg, list<Img_node*> *aStroke)
{
	cv::Mat densityMap = srcImg.clone();
	unsigned char* densityData = (unsigned char*)densityMap.data;

	for (list<Img_node*>::iterator it = aStroke->begin(); it != aStroke->end(); it++)
	{
		int density = 255 / (g_depth_limit - 1) * (g_depth_limit - (*it)->depth);
		Point srtPoint, endPoint;
		srtPoint.x = (*it)->srtPoint.x, srtPoint.y = (*it)->srtPoint.y;
		endPoint.x = (*it)->endPoint.x, endPoint.y = (*it)->endPoint.y;

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
*/