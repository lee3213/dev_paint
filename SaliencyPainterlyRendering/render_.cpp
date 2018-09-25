#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "render_.h"

#include "brush.h"
#include "QuadTree.h"
#include "debug_image.h"
#include "define.h"
#include "extern.h"
using namespace std;
using namespace cv;
void image_save(string image_name, Mat result_image, Mat Quad_TreeMap) {
	debug_image_abs(g_root_image_path, g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_root_image_path, g_para_method + string("_") + image_name + string("_QT"), Quad_TreeMap);

	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name + string("_QT"), Quad_TreeMap);

	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name + string("_QT"), Quad_TreeMap);


	debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name + string("_QT"), Quad_TreeMap);



	debug_image(g_para_method + string("_") + g_image_name, result_image);
	debug_image(g_para_method + string("_") + g_image_name, Quad_TreeMap);
}
void render_::brush_delete(list<Brush> &brush_set) {
	int nth = 0;
	for (std::list<Brush>::iterator i = brush_set.begin(), endI = brush_set.end(); i != endI; ++i, nth++)
	{

		(*i).brush.release();
		(*i).brush_thumbnail.release();
		(*i).bump.release();
		(*i).index_brush.release();
		(*i).brush_gray.release();
	}

	brush_set.clear();

}
int stroke_dump(list<Img_node*> *_aStroke_set[], string tag, int  depth) {
	int __saved_depth = -1;
	int  _grid[MAX_DEPTH];

	cout << tag << endl;
	for (int i = 0; i <MAX_DEPTH; i++) {
		_grid[i] = 0;
	}

	for (int i = 0; i < depth; i++) {
		for (list<Img_node*>::iterator St_it = _aStroke_set[i]->begin(); St_it != _aStroke_set[i]->end(); St_it++)
		{

			_grid[(*St_it)->depth]++;
			__saved_depth = (*St_it)->depth;
		}//end of QuadTree dump
	}
		for (int i = 0; i < depth; i++) {
			cout << setw(4) << i << ", " << _grid[i] << endl;
	}
	return depth;
}
int stroke_dump(list<Img_node*> *_aStroke_set, string tag,  int  depth) {
	int __saved_depth = -1;
	int  _grid[MAX_DEPTH];

	cout << tag << endl;
	for (int i = 0; i <MAX_DEPTH; i++) {
		_grid[i] = 0;
	}

	for (int i = 0; i < depth; i++) {
		for (list<Img_node*>::iterator St_it = _aStroke_set[i].begin(); St_it != _aStroke_set[i].end(); St_it++)
		{

			_grid[(*St_it)->depth]++;
			__saved_depth = (*St_it)->depth;
		}//end of QuadTree dump
	}
	for (int i = 0; i < depth; i++) {
		cout << setw(4) << i << ", " << _grid[__saved_depth] << endl;
	}
	return depth;
}
int draw_grid_2(Mat _Quad_TreeMap,
	list<Img_node*> aStroke_set[], string ftag, int  depth, int draw_depth, int c,string _tag) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];
	
	static int called_cnt = 0;
	if (aStroke_set->size() == 0) {
		cout << "aStroke_Set size() = 0 " + _tag << +": " << ftag << " : " << to_string(draw_depth) << endl;
		return depth;
	}

	for (int i = 0; i <MAX_DEPTH; i++) {
		overlay_grid_map[i] = _Quad_TreeMap.clone();
	}
	for (int i = 0; i < depth; i++) {
		for (list<Img_node*>::iterator St_it = aStroke_set[i].begin(); St_it != aStroke_set[i].end(); St_it++)
		{
			if (draw_depth != -1 && __saved_depth != draw_depth) {
				continue;
			}
			__saved_depth = (*St_it)->depth;

			if (draw_depth == -1 || __saved_depth == draw_depth)
				cv::rectangle(overlay_grid_map[__saved_depth], Rect((*St_it)->info.srtPoint,
				(*St_it)->info.endPoint), RGB(c, c, c));

		}//end of QuadTree dump
	}// for depth
	for (int i = 0; i <= __saved_depth; i++) {
		if (draw_depth == -1 || i == draw_depth) {
			debug_image("ing/"+to_string(called_cnt)+"_o_" + ftag, i, overlay_grid_map[i]);
			called_cnt++;
		}
	}
	return depth;
}
int draw_grid_depth(Mat  _grid_map_1c[], Mat _grid_map_1c_accu,
	list<Img_node*> aStroke_set[], string tag, int & grid_map_sum,
	 int _QT_grid_count[], bool do_grid_cnt, int draw_depth, int c) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];
	static int called_cnt = 0;

	for (int i = 0; i < MAX_DEPTH; i++) {
		overlay_grid_map[i] = _grid_map_1c[MAX_DEPTH].clone();
	}

	for (int i = 0; i < MAX_DEPTH; i++) {
	//	cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aStroke_set[i].size() << endl;
		if (aStroke_set[i].size() == 0) 
			break;
		for (list<Img_node*>::iterator St_it = aStroke_set[i].begin(); St_it != aStroke_set[i].end(); St_it++)
		{
			__saved_depth = (*St_it)->depth;

			cv::rectangle(_grid_map_1c_accu, Rect((*St_it)->info.srtPoint,
				(*St_it)->info.endPoint), RGB(255, 255, 255));
			cv::rectangle(_grid_map_1c[__saved_depth], Rect((*St_it)->info.srtPoint,
				(*St_it)->info.endPoint), RGB(c, c, c));

			if (do_grid_cnt) {
				_QT_grid_count[__saved_depth]++;
			}

			if (draw_depth == -1 || __saved_depth == draw_depth)
				cv::rectangle(overlay_grid_map[__saved_depth], Rect((*St_it)->info.srtPoint,
				(*St_it)->info.endPoint), RGB(255, 255, 255));

		}//end of for list
	}//end of for i
	for (int i = 0; i <= __saved_depth; i++) {
		if (draw_depth == -1 || i == draw_depth) {
			debug_image("ing/_o_grid_" + tag, i, overlay_grid_map[i]);
			debug_image("_grid_" + tag, i, _grid_map_1c[i]);
			called_cnt++;
		}
		cout << " y depth " + tag << setw(5) << i << " : " << __saved_depth<<", "<<_QT_grid_count[i] << endl;
		if (do_grid_cnt) {
			grid_map_sum += _QT_grid_count[i];
		}
	}

	debug_image("QT_" + tag + "_" + to_string(__saved_depth) + "_f", _grid_map_1c_accu);

	cout << "z depth " + tag << setw(5) << __saved_depth << " : QT_" << _QT_grid_count[__saved_depth] << endl;
	
	int depth = __saved_depth + 1;
	cout << "finalized Depth : " << depth << " : " << _QT_grid_count[__saved_depth] << endl;
	cout << tag + " grid sum " << grid_map_sum << endl;

	return depth;
}
void render_::func_(){
//void func_(int _depth, int _changed_count[], int _QT_grid_count[], int _Sgrid_painting_try[], int _m_brush_size[], long int _painting_area[]) {
	int Qt_sum = 0;
	int try_sum = 0;
	int c_sum = 0;
	int t_sum = 0;
	//_painting_area[astroke_depth] += st_w_size*st_h_size;
	cout << m_tag_+"    g_Scale = " << g_paint_area_scale << endl;
	cout << endl <<
		setw(6) << "[]" <<
		setw(7) << "QT_cnt" <<
		setw(6) << "try" <<
		setw(11) << "try/QT" <<
		setw(8) << "changed" <<
		setw(10) << "ch ratio" <<
		setw(12) << "brush_size" <<
		setw(12) << "b area*try" <<
		setw(15) << "painting_Area" <<
		setw(14) << "p_area/try" <<
		setw(16) << "p_area/p_Area" <<
		endl;
	for (int i = 0; i < mm_depth; i++) {
		if (r_s_grid_painting_try[i] != 0 && r_s_painting_area[i] !=0  && QT_grid_count[i] !=0){
		cout << setw(15) << i << "," <<
			setw(6) << QT_grid_count[i] << ", " <<
			setw(6) << r_s_grid_painting_try[i] << ", " <<

			setw(8) << (int)(r_s_grid_painting_try[i] / QT_grid_count[i]) <<
			setw(8) << r_s_changed_count[i] << ",  " <<
			std::fixed <<
			std::setw(7) << std::setprecision(2) <<
			(int)((float)r_s_changed_count[i] / (float)(r_s_grid_painting_try[i])* 100.) <<
			setw(6) << brush_size[i] <<
			setw(6) << brush_size[i] * brush_size[i] <<
			setw(10) << brush_size[i] * brush_size[i] * r_s_grid_painting_try[i]
			<< setw(17) << r_s_painting_area[i]
			<< setw(15) << (int)((r_s_painting_area[i] / r_s_grid_painting_try[i])*100.)

			<< setw(12) << (int)(brush_size[i] * brush_size[i] * r_s_grid_painting_try[i] / r_s_painting_area[i])
			<< endl;
		
		}
		else {
			cout << setw(15) << to_string(i)+","+m_tag<< "," <<
				setw(6) << QT_grid_count[i] << ", " <<
				setw(6) << r_s_grid_painting_try[i] << ", " <<

				setw(8) << "****"//(int)(grid_painting_try[i] / QT_grid_count[i])
				<<
				setw(8) << r_s_changed_count[i] << ",  " <<
				std::fixed <<
				std::setw(7) << std::setprecision(2) <<"********"<<
				//(int)((float)changed_count[i] / (float)(grid_painting_try[i])* 100.) <<
				setw(6) << brush_size[i] <<
				setw(6) << brush_size[i] * brush_size[i] <<
				setw(10) << brush_size[i] * brush_size[i] * r_s_grid_painting_try[i]
				<< setw(17) << r_s_painting_area[i]
				<< setw(15) << "****"//nt)((painting_area[i] / grid_painting_try[i])*100.)

				<< setw(12) <<"****"// (int)(brush_size[i] * brush_size[i] * grid_painting_try[i] / painting_area[i])
				<< endl;
		}
		Qt_sum += QT_grid_count[i];
		try_sum += r_s_grid_painting_try[i];
		//	t_sum += try_count[i];
		c_sum += r_s_changed_count[i];

		g_file_cstat << g_para_method + "," << g_image_name << ", " << "depth:pr_count changed: " + to_string(i) + "," <<

			r_s_grid_painting_try[i] << "," << to_string(r_s_changed_count[i]) << endl;
	}

	cout << setw(12) << Qt_sum << setw(18) << try_sum << setw(8) << c_sum << endl;

}
int calc_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
	int _brush_size[], string tag)
{
	int brush_step = (int)((_BrushMaxSize - _BrushMinSize) / _depth);

	cout << tag + " image depth  " << _depth << ", size: " << g_src_image_width << ",  " << g_src_image_height << endl;
	cout << "_BrushMaxSize " << _BrushMaxSize << "      ";
	cout << "_BrushMinSize " << _BrushMinSize << "       ";
	cout << "_brush_step " << brush_step << endl;

	for (int i = 0; i < _depth; i++)
	{
		_brush_size[i] = (int)(_BrushMaxSize - (i)* brush_step);

		if (_brush_size[i] < _BrushMinSize)
			_brush_size[i] = _BrushMinSize;
		if (i == 0)_brush_size[0] = (int)(_BrushMaxSize*1.5);
		cout << tag << "   Brush Size : " << i << ", " << setw(4) << _brush_size[i] << endl;
		g_file_cstat << g_para_method + "," << g_image_name << ", " << "brush_size" + to_string(i) + "," <<
			_brush_size[i] << endl;
	}

	return _depth;
}

void  render_::rectangle_canvas(cv::Mat mat, cv::Rect  rect, Scalar s) {

	rect.x = rect.x + canvas_size_bezel_size;
	rect.y = rect.y + canvas_size_bezel_size;
	cv::rectangle(mat, rect, s);
}

void  render_::p_peek_canvas(unsigned char * p, int p_x, int p_y, int &p_0, int &p_1, int &p_2) {
	int index = ((p_x + canvas_size_bezel_size) + (p_y + canvas_size_bezel_size) * canvas_size_width) * 3;

	p_0 = p[index];
	p_1 = p[index + 1];
	p_2 = p[index + 2];
}
void  render_::post_process() {
	int ret;
	if (success_or_fail == 0) {
		image_save(g_image_name + m_tag_, result_image, r_grid_map_1c_accu);

		
			ret = draw_grid_2(result_image.clone(), mm_aStroke_set, "prst_" + m_tag, mm_depth, -1, 255, m_tag);

		func_();
	}
	//m_depth, changed_count, QT_grid_count, grid_painting_try, brush_size, painting_area);
}
void  render_::add_gradient_map(int i, Mat a_map) {
	gradient_map[i] = a_map;
}
render_::render_(int _render_method,std::string tag, std::string tag_, std::string _tag, cv::Mat _srcImg) {
	render_method = _render_method;
	m_tag = tag;
	m_tag_ = tag_;
	m__tag = _tag;
	BrushMinSize = g_BrushMinSize;
	m_srcImg_ = _srcImg.clone();
	if (m_srcImg_.channels() == 1)
	{
		mat_print(m_srcImg_, "m_Src" + tag);
	}
	m_srcData = _srcImg.data;
	QT_depth = g_depth_limit;
	;
	for (int i = 0; i < MAX_DEPTH; i++) {
		QT_grid_count[i] = 0;
		r_s_grid_painting_try[i] = 0;
		brush_size[i]=0;
		r_s_changed_count[i]=0;
		r_s_painting_area[i]=0;
		r_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_grid_map_1c[i].setTo(255);
		r_try_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_try_map_1c[i].setTo(255);

	}
	r_grid_map_1c_accu.create(g_src_image_height, g_src_image_width, CV_8UC1);
};
render_::~render_() {
	m_srcImg_.release();
	//Quad_TreeMap.release();
	result_image.release();
	for (int i = 0; i < MAX_DEPTH; i++) {
		r_grid_map_1c[i].release();

	for (list<Img_node*>::iterator St_it = mm_aStroke_set[i].begin(); St_it != mm_aStroke_set[i].end(); St_it++) {
		free(*St_it);
	}
	}
	brush_delete(brush_set);

};

int render_::prepare() {

	int ret;
	int depth_sobel, depth_saliency;
	int k_depth;
	if (g_src_image_width < g_src_image_height)
		BrushMaxSize = g_src_image_width / 10;
	else
		BrushMaxSize = g_src_image_height / 10;

	canvas_size_bezel_size = BrushMaxSize;

	//Rendering
	canvas_size_width = g_src_image_width + canvas_size_bezel_size * 2;
	canvas_size_height = g_src_image_height + canvas_size_bezel_size * 2;


	cout << "************* prepare start >" << m_tag << " <********************************************" << endl;
	r_grid_map_1c_accu = gradient_map[render_method];
	//m_aStroke_set = new list<Img_node*>();
	//for (int i = 0; i < MAX_DEPTH; i++)
	//	cout << "aStroke_set[" << setw(2) << to_string(i) + "]->size().initialize = " + m_tag << ", " << mm_aStroke_set[i].size() << endl;

	if (render_method != RENDER_TWOPASS_ATTACH && render_method != RENDER_TWOPASS_MERGE) {

		ret = QuadTree::TakeQuadTree(gradient_map[render_method], mm_aStroke_set, m_tag);
	}
	else {
		int saved_depth = -1;
		Img_node *me_node;
		Imginfo info;
		int  S;
		list<Img_node*> *stroke_set_sobel[MAX_DEPTH];
		list<Img_node*> *stroke_set_saliency[MAX_DEPTH];

		for (int l = 0; l < render_sobel->mm_depth; l++) {
			stroke_set_sobel[l] = render_sobel->get_stroke_set_ptr(l);
			cout << "sobel.size()= " + m_tag << " : " <<l<<", "<< stroke_set_sobel[l]->size() << endl;
			
		}
	//	for (int l = 0; l < render_sobel->mm_depth; l++)
			stroke_dump(stroke_set_sobel, m_tag + " sobel= ", render_sobel->mm_depth);

		for (int l = 0; l < render_saliency->mm_depth; l++) {
			stroke_set_saliency[l] = render_saliency->get_stroke_set_ptr(l);
			cout << "saliency.size()= " + m_tag << ", " << stroke_set_saliency[l]->size() << endl;
		}
	//	for (int l = 0; l < render_saliency->mm_depth; l++)
		stroke_dump(stroke_set_saliency, m_tag + "saliency", render_saliency->mm_depth);

		int kk = 0;

		for (int i = 0; i < render_sobel->mm_depth; i++) {
			//	cout << "astroke.size(_sobel)= " + m_tag << ", " << mm_aStroke_set[i].size() << endl;
			for (list<Img_node*>::iterator St_it = stroke_set_sobel[i]->begin(); St_it != stroke_set_sobel[i]->end(); St_it++) {
				info = (*St_it)->info;
				k_depth = (*St_it)->depth;
				S = (*St_it)->avgS;
				me_node = QuadTree::newImageTree(info, k_depth, S);
				mm_aStroke_set[i].push_back(me_node);
				kk++;
			}
		}
		cout << " added " << kk << ": " << k_depth << endl;
		depth_sobel = k_depth;
		//mm_aStroke_set->sort();
		stroke_dump(mm_aStroke_set, m_tag, mm_depth);
		kk = 0;

		for (int i = 0; i < render_saliency->mm_depth; i++) {
			//cout << "astroke.size()= " + m_tag << ", " << mm_aStroke_set[i].size() << endl;

			for (list<Img_node*>::iterator St_it = stroke_set_saliency[i]->begin(); St_it != stroke_set_saliency[i]->end(); St_it++) {
				if (render_sobel->mm_depth > 5 && (*St_it)->depth < 4)
					continue;
				else if ((*St_it)->depth < 3)
					continue;

				info = (*St_it)->info;
				if (render_method == RENDER_TWOPASS_MERGE)
					k_depth = (*St_it)->depth;
				else {
					if (render_sobel->mm_depth > 5)
						k_depth = render_saliency->mm_depth + (*St_it)->depth - 4;
					else
						k_depth = render_saliency->mm_depth + (*St_it)->depth - 3;
				}
				S = (*St_it)->avgS;
				me_node = QuadTree::newImageTree(info, k_depth, S);
				mm_aStroke_set[k_depth].push_back(me_node);
				kk++;
			}
		}
		cout << " added " << kk << ": " << k_depth << endl;

		depth_saliency = k_depth;
	}
	//m_aStroke_set->sort(Img_node::greater<Img_node*>());


//	sort(m_aStroke_set->begin(), m_aStroke_set->end());
	//m_aStroke_set->sort();

	k_depth = -1;
	int astroke_depth;
	int k = 0;
	for(int i=0;i<mm_depth;i++){
	for (list<Img_node*>::iterator St_it = mm_aStroke_set[i].begin(); St_it != mm_aStroke_set[i].end(); St_it++, k++)
	{
		astroke_depth = (*St_it)->depth;
		if (k_depth != -1) {
			if (astroke_depth < k_depth ||
				astroke_depth > k_depth + 1)
			{
				cout << "sort fail  " << k << "th : " << k_depth << " -> " << astroke_depth << endl;
			}
		}
		k_depth = astroke_depth;
	}
	cout << m_tag + "Sort check _aStrokeset_size() : " << k_depth<<": "<<mm_aStroke_set[i].size() << endl;
	}
	/*if (depth_sobel > depth_saliency) {
		m_depth = QT_depth = depth_sobel;
	}
	else 	m_depth = QT_depth = depth_saliency;
	*/
	
	mm_depth=draw_grid_depth(r_grid_map_1c, r_grid_map_1c_accu,mm_aStroke_set, m_tag,
		grid_map_sum, QT_grid_count, true, -1, 0);
	QT_depth = mm_depth;
	
	cout << "x depth " + m_tag + " = " << mm_depth  << endl;
	for (int i = 0; i < mm_depth ; i++)
	{
		cout << setw(15) << m_tag + ", " + to_string(i) <<", "<< QT_grid_count[i] << endl;
	}
	calc_brush_size(BrushMaxSize, BrushMinSize, mm_depth,
	brush_size, m_tag);

	ret = BrushInitialization(brush_set, mm_depth, brush_size);
	if (ret != 0) {
		cout << "Brush Initialization Error_" + m_tag << endl;
		cerr << "Brush Initialization Error_" + m_tag << endl;
		return -1001;
	}
	////for (int i = 0; i < mm_depth; i++) {
	//	cout << "Brush "<<i << ", " << brush_size[i] << endl;
	//}
//	for (int i = 0; i < mm_depth; i++) {
//		cout << "QT stroke count(" + m_tag + ") " << QT_depth << " " << mm_depth << " : " << mm_aStroke_set[i].size() << endl;
	//	cout.flush();
	//	if (mm_aStroke_set[i].size() == 0) {
	//		cout << "unexpected Stroke Tree" + m_tag + " = 0" << endl;
	//		return -20000;
	//	}
	//}
	//ret = draw_grid_2(m_srcImg_.clone(), m_aStroke_set, "src_0_" + m_tag, m_depth, -1, 0,m_tag);
	cout << "------------------------------------------------------------------" << endl;
	cout.flush();
	return 0;
}