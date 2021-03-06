#pragma once
#include "stdafx.h"
#include "brush.h"
#include "QuadTree.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "define.h"
#include "extern.h"
using namespace std;
using namespace cv;
int draw_grid_2(Mat _Quad_TreeMap,
	list<Img_node*> *aStroke_set, string tag, int & depth, int draw_depth, int c, string _tag);

class render_ {
public:


	int render_method;
	//Mat render_::PainterlyRendering();
	cv::Mat gradient_map[MAX_Gradient];
	cv::Mat m_srcImg_;
	unsigned char * m_srcData;
	//cv::Mat Quad_TreeMap;
	cv::Mat result_image;
	int  BrushMaxSize;						//1024size에는 130, 3072size에는 400으로 설정했습니다..
	int  BrushMinSize;
	int brush_step;

	//cv::Mat gradient_Map_union;
	//Mat gra_Map;
	int canvas_size_width;
	int canvas_size_height;
	int canvas_size_bezel_size;

	int grid_map_sum = 0, grid_count_sum = 0;
	Mat grid_map_1c[MAX_DEPTH + 1];
	Mat try_map_1c[MAX_DEPTH + 1];
	int m_depth;

	list<Img_node*> *m_aStroke_set;
	//list<Img_node*> *m_aStroke_set_merge;

	//list<QuadTree::Img_node*>aStroke_set_Sgrad;

	list<Brush> brush_set;
	int QT_depth;

	int brush_size[MAX_DEPTH];
	int QT_grid_count[MAX_DEPTH];
	int changed_count[MAX_DEPTH];
	int grid_painting_try[MAX_DEPTH];
	long int painting_area[MAX_DEPTH];

	render_ * render_sobel;
	render_ * render_saliency;
		
	std::string m_tag;
	std::string m__tag;
	std::string m_tag_;

	//int get_depth;
	render_(int _render_method,std::string tag, std::string tag_, std::string _tag, Mat _srcImg);
	~render_();
	void render_::func_();
	cv::Mat  PainterlyRendering();

	list<Img_node*> *get_stroke_set() {
		return m_aStroke_set;
	}
	void add_gradient_map(int i,Mat a_map);
	int render_::prepare();
	void post_process();
	void  render_::rectangle_canvas(cv::Mat mat, cv::Rect  rect, Scalar s);
	 //void p_poke_canvas(unsigned char * p, int index, int y, int r, int g, int b);
	 void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2) {
		 int index = ((p_x + canvas_size_bezel_size) + (p_y + canvas_size_bezel_size) * canvas_size_width) * 3;
		 p[index] = p_0;
		 p[index + 1] = p_1;
		 p[index + 2] = p_2;
	 }
	 void p_peek_canvas(unsigned char * p, int index, int y, int &r, int &g, int &b);
	//void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2);
	void render_::brush_delete(list<Brush> &brush_set);
	//void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2);
	void add_render(render_ *_sobel_render, render_ *_saliency_render) {
		render_sobel = _sobel_render;
		render_saliency = _saliency_render;
	};
	//void  render_::p_peek_canvas(unsigned char * p, int p_x, int p_y, int &p_0, int &p_1, int &p_2);

	

	int   render_::P_Rendering(//cv::Mat srcImg,
							   //	unsigned char * srcData,
		unsigned char * changedData,//rstImage
		Mat & beforeImg,//rstImage.clone()

		cv::Mat &_testImg_resized,
		//list<Brush> &_brush_set,
		Point _fetch_color_Point,
		Point centered_SrtPoint,
		int paint_grid_w_size, int paint_grid_h_size,
		//String tag,
		int astroke_number, int ing, int astroke_depth, int painting_count,
		int color_BGR_B, int color_BGR_G, int color_BGR_R,//BGR order
		unsigned char * ing_canvas_data

	);
};

