
#include "stdafx.h"
#include <list>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "define.h"
#include "brush.h"
#include "QuadTree.h"
#include "debug_image.h"
#include "brush_pgm.h"
#include "render_.h"

render_::render_(int _render_method, cv::Mat &_srcImg) {
	string r_cout_fname = g_root_path_win + string("\\cout\\") + g_para
		+ "\\cout_" + g_para_method + "_" + g_image_name + _tag[_render_method] + string(".txt");
	cout << " file name " << _tag[_render_method] << " " << r_cout_fname << endl;
	r_cout.open(r_cout_fname.c_str(), ios::out);

	render_method = _render_method;
	m_tag = tag[_render_method];
	m_tag_ = tag_[_render_method];
	m__tag = _tag[_render_method];
	m_t = _t[render_method];
	m_t_ = _t_[render_method];
	x_BrushMinSize = g_BrushMinSize;
	x_srcImg_ = _srcImg.clone();
	x_src_image_size.width = (g_src_image_width);
	x_src_image_size.height = g_src_image_height;
	x_retry_map_1c.create(x_src_image_size.height, x_src_image_size.width, CV_8UC1);
	x_retry_map_1c.setTo(255);

	 x_retry_map_1c_data = x_retry_map_1c.data;
	if (x_srcImg_.channels() == 1)
	{
		mat_print(x_srcImg_, "x_Src" + tag[_render_method]);
	}
	x_src_ptr = x_srcImg_.data;
	QT_depth = g_depth_limit;

	for (int i = 0; i <= MAX_DEPTH; i++) {
		x_strk_times[i] = 0;
		QT_grid_count[i] = 0;
		grid_try_sum[i] = 0;
		render_brush_size[i] = 0;
		r_s_changed_count[i] = 0;

		r_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_grid_map_1c[i].setTo(255);
		r_try_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_try_map_1c[i].setTo(255);

		render_Stroke_set[i].set_depth(i);
	}


	r_grid_map_1c_accu.create(g_src_image_height, g_src_image_width, CV_8UC1);
};
render_::~render_() {
	//	x_srcImg_.release();
	//Quad_TreeMap.release();
	//result_image.release();
	//	for (int i = 0; i < MAX_DEPTH; i++) {
	//	r_grid_map_1c[i].release();
	/*
	for (list<Img_node*>::iterator partition_it = render_Stroke_set[i].stroke_list.begin();
	partition_it != render_Stroke_set[i].stroke_list.end(); partition_it++) {
	delete (*partition_it);
	}*/
	//}

	//	brush_delete(brush_set);
	//for(int i=0;i<MAX_DEPTH;i++)
	//	brush_delete(brush_resized_set[i]);

};

void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 3;
	p[index] = p_0;
	p[index + 1] = p_1;
	p[index + 2] = p_2;
}
 void  render_::p_poke_canvas_1c(unsigned char * p, int p_x, int p_y, int p_0) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 1;
	p[index] = p_0;

}
void  render_::p_peek_canvas(unsigned char * p, int p_x, int p_y, int &p_0, int &p_1, int &p_2) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 3;

	p_0 = p[index];
	p_1 = p[index + 1];
	p_2 = p[index + 2];
}
void  render_::p_peek_canvas_1c(unsigned char * p, int p_x, int p_y, int &p_0) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width);

	p_0 = p[index];

}