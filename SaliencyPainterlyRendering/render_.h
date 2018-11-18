#pragma once
#include "stdafx.h"
#include <list>
#include "opencv2\\opencv.hpp"
#include "opencv2\\core.hpp"

#include "define.h"
#include "brush.h"

#include "extern.h"
#include "QuadTree.h"
using namespace std;
using namespace cv;
//int draw_grid_2(Mat _Quad_TreeMap,
	//list<Img_node*> *aStroke_set, string tag, int  depth, int draw_depth, int c, string _tag);
class Stroke_set {
public:
	Stroke_Node a;
	list<Stroke_Node *> stroke_list;
	//int stroke_cnt;
	int depth;
	//int size() {
	//		return (int)(stroke_list.size());
	//}
	Stroke_set() {

	};
	~Stroke_set() {

	};
	void set_depth(int _depth) {
		depth = _depth;
	};
	int push_back(Stroke_Node * _p) {

		stroke_list.push_back(_p);


		return 0;//Good
	};
	

};
class render_ {
public:
	Stroke_set *a_mm_aStroke_set;
	Stroke_set mm_aStroke_set[MAX_DEPTH];
	fstream r_cout;
	//	streambuf* r_stream_cout;
	fstream r_clog;
	//	streambuf* r_stream_clog;
	fstream r_cstat;
	//	streambuf* r_stream_cstat;
		//Mat *paint_map;
		//unsigned char * paint_map_data;
	int success_or_fail;
	int render_method;
	//Mat render_::PainterlyRendering();
	cv::Mat gradient_map[MAX_Gradient];
	cv::Mat m_srcImg_;
	unsigned char * m_srcData;
	//cv::Mat Quad_TreeMap;
	cv::Mat result_image;

	Mat src_canvas;
	cv::Mat rst_accu_canvas[MAX_DEPTH];
	cv::Mat ing_canvas[MAX_DEPTH];
	unsigned char * rst_accu_canvas_data[MAX_DEPTH];
	unsigned char * ing_canvas_data[MAX_DEPTH];

	int canvas_size_width;
	int canvas_size_height;
	int canvas_size_bezel_size;
	int  BrushMaxSize;						//1024size에는 130, 3072size에는 400으로 설정했습니다..
	int  BrushMinSize;
	int brush_step;
	int grid_map_sum = 0, grid_count_sum = 0;
	Mat r_grid_map_1c[MAX_DEPTH + 1];
	Mat r_grid_map_1c_accu;
	Mat r_try_map_1c[MAX_DEPTH + 1];
	unsigned char * r_try_map_1c_data[MAX_DEPTH];
	int mm_depth;



	int depth_sobel, depth_saliency;//, depth_attach;
	vector <Brush*> brush_set;
	vector <Brush*> brush_resized_set[MAX_DEPTH];
	int QT_depth;

	int brush_size[MAX_DEPTH];
	int QT_grid_count[MAX_DEPTH];
	int r_s_changed_count[MAX_DEPTH];
	int r_s_grid_painting_try[MAX_DEPTH];
	long int r_s_painting_area[MAX_DEPTH];

	render_ * render_sobel;
	render_ * render_saliency;

	std::string m_tag;
	std::string m__tag;
	std::string m_tag_;
	string m_t;
	int *random_x;
	int *random_y;
	//int get_depth;
	render_(int _render_method, Mat &_srcImg);
	~render_();
	void render_::func_();
	int  PainterlyRendering();



	int stroke_dump(Stroke_set _aStroke_set[], string tag, int  depth);
	//int stroke_dump(Stroke_set _aStroke_set, string tag, int  depth);
	int draw_grid_depth(Mat  _grid_map_1c[], Mat _grid_map_1c_accu,
		Stroke_set  aStroke_set[], string tag, int & grid_map_sum,
		int _QT_grid_count[]//, bool do_grid_cnt//, int draw_depth, int c
	);
	void proof_box(Point &s, int i_width, int i_height, char * p);
	void proof_box(Point &s, int i_width, int i_height);
	//	list<Img_node*> *get_Stroke_set_ptr(int i) {
		//	return &mm_aStroke_set[i];
	//	}
	void add_gradient_map(int i, Mat a_map);
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
	void render_::brush_delete(vector <Brush*> brush_set);
	//void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2);
	void add_render(render_ *_sobel_render, render_ *_saliency_render) {
		render_sobel = _sobel_render;
		render_saliency = _saliency_render;
	};
	//void  render_::p_peek_canvas(unsigned char * p, int p_x, int p_y, int &p_0, int &p_1, int &p_2);
	int draw_grid_2(Mat _Quad_TreeMap,
		Stroke_set aStroke_set[], string ftag, int  depth,// int draw_depth, 
		int c, string _tag);
	int render_::calc_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
		int _brush_size[], string tag);

	int render_::P_Rendering(Mat & _src_ROI, Mat & _before_ROI, cv::Mat & _changed_ROI,
		cv::Mat & ing_ROI, Point _fetch_color_Point, Point centered_SrtPoint,
		Point canvas_centered_SrtPoint, Point canvas_centered_EndPoint, int brush_area_w_size,
		int brush_area_h_size, int astroke_depth, int painting_count, int color_BGR_B, int color_BGR_G, int color_BGR_R);

/*
	int   render_::P_Rendering(//cv::Mat srcImg,
							   //	unsigned char * srcData,
							   //	unsigned char * changed_canvas_data,//rstImage
		Mat & _src_ROI,
		Mat & _before_ROI,//rstImage.clone()
		cv::Mat & _changed_ROI,
		cv::Mat & ing_ROI,
		//vector <Brush*> &_brush_set,
		Point _fetch_color_Point,
		Point centered_SrtPoint,
		Point canvas_centered_SrtPoint,
		Point canvas_centered_EndPoint,
		int brush_area_w_size, int brush_area_h_size,
		//String tag,
		//int astroke_number,
		//int ing,
		int astroke_depth, int painting_count,
		int color_BGR_B, int color_BGR_G, int color_BGR_R//BGR order
														  //unsigned char * _ing_canvas_data

	);
	*/
	double TakeAvgS(cv::Mat &srcImg, Point srtPoint, Point endPoint, int depth, string quad);
	Point render_::get_midPoint(cv::Mat &srcImg, Point s, Point e, double *D, int d, int child_QT_depth);
	int  TakeQuadTree(cv::Mat &SaliencyMap, Stroke_set aStroke[], string tag);

	int DivideImage(cv::Mat &SaliencyMap, Stroke_Node* me_node, Stroke_set aStroke_set[],
		string  quad,
		Mat & gradient_src,
		Mat stageMap, int depth, string tag);

	void brush_resize();
	/*
	Mat brush_at_brush_clone(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->brush.clone();
		}
		return _brush_set.front()->brush.clone();
	}

	Mat brush_at_brush_embose_clone(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->bump.clone();
		}
		return _brush_set.front()->bump.clone();
	}
	Mat brush_at_brush_gray_clone(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->brush_gray;
		}
		cerr << "at G " << no << endl;
		return(*it)->brush_gray;
	}
	Mat brush_at_brush(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->brush;
		}
		return (*it)->brush;
	}

	Mat brush_at_brush_embose(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->bump;
		}
		return (*it)->bump;
	}
	Mat brush_at_brush_gray(vector <Brush*> & _brush_set, int no) {

		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			if ((*it)->brush_no == no)
				return (*it)->brush_gray.clone();
		}
		cerr << "at G " << no << endl;
		return (*it)->brush_gray;
	}
	*/


	inline void p_poke(unsigned char * p, int index, int p_0) {
		p[index] = p_0;
	}
	inline void p_poke(unsigned char * p, int index, int p_0, int p_1, int p_2) {
		p[index] = p_0;
		p[index + 1] = p_1;
		p[index + 2] = p_2;
	}
	inline void p_peek(unsigned char * p, int index, int &p_0, int &p_1, int &p_2) {
		p_0 = p[index];
		p_1 = p[index + 1];
		p_2 = p[index + 2];
	}
	inline void p_peek_1c(unsigned char * p, int index, int &p_0) {
		p_0 = p[index];
	}

	int lbph();


	double double_array_min_at(double a[], int n);
	double double_array_min(double a[], int n);
	double double_array_max(double a[], int n);
	double double_array_max_at(double a[], int n);
	double double_2xarray_min_at(double *a, int w, int h, int n);
	double double_2xarray_min(double *a, int w, int h, int n);
	double double_2xarray_max(double *a, int w, int h, int n);
	double double_2xarray_max_at(double *a, int w, int h, int n);
	int get_selected(double *_D, int n, int divider, int &selected_x, int &selected_y);

};

