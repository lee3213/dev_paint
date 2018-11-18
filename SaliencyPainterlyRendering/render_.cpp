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
	debug_image_abs(g_root_image_path, g_para_method + string("_") + image_name+"_Q" , Quad_TreeMap);

	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_path + string("/"), g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);

	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_path + string("/"), g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);

		debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name, result_image);
	debug_image_abs(g_para_method_image_path, g_para_method + string("_") + image_name + string("_Q"), Quad_TreeMap);



	debug_image(g_para_method + string("_") + g_image_name, result_image);
	debug_image(g_para_method + string("_") + g_image_name, Quad_TreeMap);
}
void render_::brush_delete(vector <Brush*> brush_set) {
	int nth = 0;
	for (std::vector <Brush*>::iterator i = brush_set.begin(), endI = brush_set.end(); i != endI; ++i, nth++)
	{

		(*i)->brush.release();
		(*i)->brush_thumbnail.release();
		(*i)->bump.release();
		(*i)->index_brush.release();
		(*i)->brush_gray.release();
	}

	brush_set.clear();

}
int render_ ::stroke_dump(Stroke_set _aStroke_set[], string tag__, int  depth) {
	r_cout << tag__ << " " << depth << endl;
	for (int i = 0; i < depth; i++) {
		
			r_cout << tag__ << " " << i << " : " << _aStroke_set[i].stroke_list.size() << endl;
		
	}
/*	int __saved_depth = -1;
	int  _grid[MAX_DEPTH];

	r_cout << tag << endl;
	for (int i = 0; i <MAX_DEPTH; i++) {
		_grid[i] = 0;
	}

	for (int i = 0; i < depth; i++) {
		for (list<Img_node*>::iterator Stroke_it = _aStroke_set[i]->begin(); Stroke_it != _aStroke_set[i]->end(); Stroke_it++)
		{

			_grid[(*Stroke_it)->depth]++;
			__saved_depth = (*Stroke_it)->depth;
		}//end of QuadTree dump
	}
		for (int i = 0; i < depth; i++) {
			r_cout << setw(4) << i << ", " << _grid[i] << endl;
	}
	*/
	return depth;
}

int render_::draw_grid_2(Mat _Quad_TreeMap,
	Stroke_set aStroke_set[], string ftag, int  depth, 
	//int draw_depth, 
	int c,string _tag) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];
	
	static int called_cnt = 0;
	if (aStroke_set->stroke_list.size() == 0) {
		r_cout << "aStroke_Set size() = 0 " + _tag << +": " << ftag << " : "// << to_string(draw_depth) 
			<< endl;
		return depth;
	}

	for (int i = 0; i <MAX_DEPTH; i++) {
		overlay_grid_map[i] = _Quad_TreeMap.clone();
	}
	for (int i = 0; i < depth; i++) {

		for (list<Stroke_Node *>::iterator Stroke_it = mm_aStroke_set[i].stroke_list.begin(); Stroke_it != mm_aStroke_set[i].stroke_list.end(); Stroke_it++)
		{
		//	if (draw_depth != -1 && __saved_depth != draw_depth) {
		//		continue;
		//	}
			__saved_depth = (*Stroke_it)->depth;

			//if (draw_depth == -1 || __saved_depth == draw_depth)
			rectangle_canvas(overlay_grid_map[__saved_depth], Rect((*Stroke_it)->srtPoint,
				(*Stroke_it)->endPoint), Scalar(c, c, c));

		}//end of QuadTree dump
	}// for depth
	for (int i = 0; i <= __saved_depth; i++) {
	//	if (draw_depth == -1 || i == draw_depth) {
			debug_image("ing/"+to_string(called_cnt)+"_o_" + ftag, i, overlay_grid_map[i]);
			called_cnt++;
	//	}
	}
	 depth = __saved_depth + 1;
	r_cout << tag << " " << __FUNCTION__ << "finalized Depth : " << depth << " : " << grid_map_sum << endl;

	called_cnt++;
	return depth;
}
int render_::draw_grid_depth(Mat  _grid_map_1c[], Mat _grid_map_1c_accu,
	Stroke_set aStroke_set[], string tag, int & grid_map_sum,
	 int _QT_grid_count[]//, bool do_grid_cnt//, int draw_depth, int c
	) {
	int __saved_depth = -1;
	Mat overlay_grid_map[MAX_DEPTH];
	static int called_cnt = 0;
	list<Stroke_Node*>::iterator outbox = aStroke_set[0].stroke_list.begin();
	//Point p;
//	p.x=(*outbox)->info.endPoint.x - 1;
	//p.y=(*outbox)->info.endPoint.y - 1;
	for (int i = 0; i < MAX_DEPTH; i++) {
		overlay_grid_map[i] = _grid_map_1c[MAX_DEPTH].clone();
		cv::rectangle(_grid_map_1c[i], cv::Rect((*outbox)->srtPoint,
			(*outbox)->endPoint)
			, RGB(0, 0,0));
		//	debug_image("ing/_ok_grid_" + tag +
			//	"_" + to_string(called_cnt) + "_", i, overlay_grid_map[i]);
		_QT_grid_count[i] = 0;
	}
	
	for (int i = 0; i < MAX_DEPTH; i++) {
		int ssize=(int) aStroke_set[i].stroke_list.size();
		//r_cout << "z : stroke " << i<<" : "<<__saved_depth <<" , "<<aStroke_set[i].size() << endl;
		if (ssize == 0) 
			break;
		
		for (list<Stroke_Node*>::iterator Stroke_it = aStroke_set[i].stroke_list.begin(); 
			Stroke_it != aStroke_set[i].stroke_list.end(); Stroke_it++)
		{
			__saved_depth = (*Stroke_it)->depth;

			cv::rectangle(_grid_map_1c_accu, Rect((*Stroke_it)->srtPoint,
				(*Stroke_it)->endPoint), RGB(255, 255, 255));
			cv::rectangle(_grid_map_1c[__saved_depth], Rect((*Stroke_it)->srtPoint,
				(*Stroke_it)->endPoint), RGB(0,0, 0));
			cv::rectangle(overlay_grid_map[__saved_depth], Rect((*Stroke_it)->srtPoint,
				(*Stroke_it)->endPoint), RGB(255, 255, 255));

				_QT_grid_count[__saved_depth]++;
		}//end of for list
	}//end of for i
	for (int i = 0; i <= __saved_depth; i++) {
		
			debug_image("ing/_og" +m_t, i, overlay_grid_map[i]);
			debug_image("_g" + m_t, i, _grid_map_1c[i]);
			debug_image("ing/_g" + m_t, i, _grid_map_1c[i]);

			r_cout << tag<<" "<<setw(6) << i << " : " << setw(10)<<_QT_grid_count[i] << endl;
				
			grid_map_sum += _QT_grid_count[i];
	
	}

	debug_image("QT" + m_t + "_" + to_string(__saved_depth) + "_f", _grid_map_1c_accu);

	//r_cout << "z depth " + tag << setw(5) << __saved_depth << " : QT_" << _QT_grid_count[__saved_depth] << endl;
	
	int depth = __saved_depth + 1;
	r_cout << tag <<" "<<__FUNCTION__<<"finalized Depth : " << depth << " : " << grid_map_sum<< endl;
	
	called_cnt++;
	return depth;
}
void render_::func_(){
//void func_(int _depth, int _changed_count[], int _QT_grid_count[], int _Sgrid_painting_try[], int _m_brush_size[], long int _painting_area[]) {
	int Qt_sum = 0;
	int try_sum = 0;
	int c_sum = 0;
	int t_sum = 0;
	//_painting_area[astroke_depth] += st_w_size*st_h_size;
	r_cout << m_tag_+"    g_Scale = " << g_paint_area_scale << endl;
	r_cout << endl <<
		setw(6) << "[]" <<
		setw(12)<<"tag "<<
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
		r_cout << setw(15) << i << "," <<m_tag<<
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
			r_cout << setw(15) << to_string(i)+","+m_tag<< "," <<
				setw(6) << QT_grid_count[i] << ", " <<
				setw(6) << r_s_grid_painting_try[i] << ", " <<

				setw(8) << "**/0"//(int)(grid_painting_try[i] / QT_grid_count[i])
				<<
				setw(8) << r_s_changed_count[i] << ",  " <<
				std::fixed <<
				std::setw(9) << std::setprecision(2) <<"******/0"<<
				//(int)((float)changed_count[i] / (float)(grid_painting_try[i])* 100.) <<
				setw(6) << brush_size[i] <<
				setw(6) << brush_size[i] * brush_size[i] <<
				setw(10) << brush_size[i] * brush_size[i] * r_s_grid_painting_try[i]
				<< setw(17) << r_s_painting_area[i]
				<< setw(15) << "**/0"//nt)((painting_area[i] / grid_painting_try[i])*100.)

				<< setw(12) <<"**/0"// (int)(brush_size[i] * brush_size[i] * grid_painting_try[i] / painting_area[i])
				<< endl;
		}
		Qt_sum += QT_grid_count[i];
		try_sum += r_s_grid_painting_try[i];
		//	t_sum += try_count[i];
		c_sum += r_s_changed_count[i];

		g_file_cstat << g_para_method + "," << g_image_name << ", " << "depth:pr_count changed: " + to_string(i) + "," <<

			r_s_grid_painting_try[i] << "," << to_string(r_s_changed_count[i]) << endl;
	}

	r_cout << setw(12) << Qt_sum << setw(18) << try_sum << setw(8) << c_sum << endl;

}
int render_::calc_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
	int _brush_size[], string tag)
{
	int brush_step;
	int k_depth;
	if (render_method == RENDER_TWOPASS_ATTACH ) {
		if (render_sobel->mm_depth > render_saliency->mm_depth) {
			k_depth = render_sobel->mm_depth; 
		}
		else 	k_depth = render_saliency->mm_depth; 
		if ( k_depth == 1)
			brush_step = (int)((_BrushMaxSize - _BrushMinSize));
		else
		 brush_step = (int)((_BrushMaxSize - _BrushMinSize) / (k_depth-1));
		
		 for (int i = 0; i < k_depth; i++)
		 {
			 if (i == g_first_layer) {
				 _brush_size[g_first_layer] = (int)(_BrushMaxSize*g_BrushMax_scale);
			 }
			 else {
				 _brush_size[i] = (int)(_BrushMaxSize - (i)* brush_step);

				 if (_brush_size[i] < _BrushMinSize)
					 _brush_size[i] = _BrushMinSize;
			 }
		//	r_cout << tag << "   Brush Size : " << i << ", " << setw(4) << _brush_size[i] << endl;
			g_file_cstat << tag<<"   "<<g_para_method + "," << g_image_name << ", " << "brush_size" + to_string(i) + "," <<
				_brush_size[i] << endl;
		}

		 int _B = _brush_size[k_depth-1];
		 int _brush_step;
		
			 if ((mm_depth - k_depth) == 0)
				 _brush_step = (_B - g_BrushAttachSize);
			 else
				 _brush_step = (_B - g_BrushAttachSize) / (mm_depth - k_depth);
	
		for (int i = 0; i < (mm_depth-k_depth); i++)
		{
			_brush_size[i+k_depth] = (int)(_B - (i+1)* _brush_step);
		}
	//	r_cout << "_brush_step " << _brush_step << endl;
	}
	else {// except for attach
		if (_depth == 1)
			brush_step = (int)(_BrushMinSize) ;
		else 
			brush_step = (int)((_BrushMaxSize - _BrushMinSize) / (_depth-1));
	
	 for (int i = 0; i < _depth; i++)
		{
		 if (i == g_first_layer) {
			 _brush_size[g_first_layer] = (int)(_BrushMaxSize*g_BrushMax_scale);
		 }
		 else {
			 _brush_size[i] = (int)(_BrushMaxSize - (i)* brush_step);

			 if (_brush_size[i] < _BrushMinSize)
				 _brush_size[i] = _BrushMinSize;
		 }
		
			g_file_cstat << g_para_method + "," << g_image_name << ", " << "brush_size" + to_string(i) + "," <<
				_brush_size[i] << endl;
		}
	}
	r_cout << tag + " image depth  " << _depth << ", size: " << g_src_image_width << ",  " << g_src_image_height << endl;
	r_cout << "  _BrushMaxSize " << _BrushMaxSize  ;
	r_cout << "  _BrushMinSize " << _BrushMinSize ;
	r_cout << "  _BrushAttachSize " << g_BrushAttachSize;
	r_cout << "  _depth  " << _depth;
	r_cout << "  brush_step " << brush_step << endl;
	for (int i = 0; i < _depth; i++) {
		r_cout << tag <<" : Br size : " <<setw(5) << i << setw(3) << _brush_size[i] << endl;
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
		image_save(m_t+g_image_name, result_image, r_grid_map_1c_accu);

		
			ret = draw_grid_2(result_image.clone(), mm_aStroke_set, "prst_" + m_t, mm_depth, //-1, 
				255, m_t);

		func_();
	}
	//m_depth, changed_count, QT_grid_count, grid_painting_try, brush_size, painting_area);
}
void  render_::add_gradient_map(int render_method, Mat a_map) {
	gradient_map[render_method] = a_map;
}
render_::render_(int _render_method, cv::Mat &_srcImg) {
	string r_cout_fname = g_root_path_win + string("\\cout\\") + g_para
		+ "\\cout_" + g_para_method + "_" + g_image_name + _tag[_render_method] + string(".txt");
	cout << " file name " << _tag[_render_method] <<" "<< r_cout_fname<< endl;
	r_cout.open(r_cout_fname.c_str(), ios::out);
	
	render_method = _render_method;
	m_tag = tag[_render_method];
	m_tag_ = tag_[_render_method];
	m__tag = _tag[_render_method];
	m_t = _t[render_method];
	BrushMinSize = g_BrushMinSize;
	m_srcImg_ = _srcImg.clone();
	if (m_srcImg_.channels() == 1)
	{
		mat_print(m_srcImg_, "m_Src" + tag[_render_method]);
	}
	m_srcData = _srcImg.data;
	QT_depth = g_depth_limit;
	;
	for (int i = 0; i <= MAX_DEPTH; i++) {
		QT_grid_count[i] = 0;
		r_s_grid_painting_try[i] = 0;
		brush_size[i]=0;
		r_s_changed_count[i]=0;
		r_s_painting_area[i]=0;
		r_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_grid_map_1c[i].setTo(255);
		r_try_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		r_try_map_1c[i].setTo(255);
	
		mm_aStroke_set[i].set_depth(i);


	}
	r_grid_map_1c_accu.create(g_src_image_height, g_src_image_width, CV_8UC1);
};
render_::~render_() {
	m_srcImg_.release();
	//Quad_TreeMap.release();
	result_image.release();
	for (int i = 0; i < MAX_DEPTH; i++) {
		r_grid_map_1c[i].release();
		/*
	for (list<Img_node*>::iterator Stroke_it = mm_aStroke_set[i].stroke_list.begin(); 
		Stroke_it != mm_aStroke_set[i].stroke_list.end(); Stroke_it++) {
		delete (*Stroke_it);
	}*/
	}
	
	brush_delete(brush_set);
	for(int i=0;i<MAX_DEPTH;i++)
		brush_delete(brush_resized_set[i]);

};

int render_::prepare() {

	int ret;
	
	int k_depth;

	if (g_src_image_width < g_src_image_height)
		BrushMaxSize = g_src_image_width / 10;
	else
		BrushMaxSize = g_src_image_height / 10;

	canvas_size_bezel_size = BrushMaxSize; 

	//Rendering
	canvas_size_width = g_src_image_width + canvas_size_bezel_size * 2;
	canvas_size_height = g_src_image_height + canvas_size_bezel_size * 2;
	//paint_map = new Mat(canvas_size_height, canvas_size_width, CV_8UC3);
	//	paint_map_data = paint_map->data;
	//	paint_map->setTo(255);
	r_cout << "************* prepare start >" << m_tag << " <********************************************" << endl;
	r_grid_map_1c_accu = gradient_map[render_method].clone();
	r_grid_map_1c[MAX_DEPTH] = gradient_map[render_method].clone();
	//m_aStroke_set = new list<Img_node*>();
	//for (int i = 0; i < MAX_DEPTH; i++)
	//	r_cout << "aStroke_set[" << setw(2) << to_string(i) + "]->size().initialize = " + m_tag << ", " << mm_aStroke_set[i].size() << endl;

	if (render_method != RENDER_TWOPASS_ATTACH && render_method != RENDER_TWOPASS_MERGE) {
		//if (render_method != RENDER_SOBEL) {
		//	ret = QuadTree::TakeQuadTree_grid(gradient_map[render_method], mm_aStroke_set, m_tag);
		//}
		//else
		ret = TakeQuadTree(gradient_map[render_method], mm_aStroke_set, m_tag);
	}
	else {
		int saved_depth = -1;
		Stroke_Node *me_node;
	//	Imginfo info;
		int  S;
		Stroke_set Stroke_set_sobel[MAX_DEPTH];
		Stroke_set Stroke_set_saliency[MAX_DEPTH];
		
	
		for (int l = 0; l < render_sobel->mm_depth; l++) {
			Stroke_set_sobel[l] = render_sobel->mm_aStroke_set[l];

		}
		for (int l = 0; l < render_saliency->mm_depth; l++) {
			Stroke_set_saliency[l] = render_saliency->mm_aStroke_set[l];

		}

			stroke_dump(Stroke_set_sobel, m_tag + " : sobel depth, count = ", render_sobel->mm_depth);

	//	for (int l = 0; l < render_saliency->mm_depth; l++)
		stroke_dump(Stroke_set_saliency, m_tag + " : saliency depth, count  = ", render_saliency->mm_depth);

		int kk = 0;

		for (int i = 0; i < render_sobel->mm_depth; i++) {
			//	r_cout << "astroke.size(_sobel)= " + m_tag << ", " << mm_aStroke_set[i].size() << endl;
			for (list<Stroke_Node*>::iterator Stroke_it = Stroke_set_sobel[i].stroke_list.begin(); Stroke_it != Stroke_set_sobel[i].stroke_list.end(); Stroke_it++) {
			//	info = (*Stroke_it)->info;
				k_depth = (*Stroke_it)->depth;
				S = (*Stroke_it)->avgS;
				me_node = new 
					Stroke_Node((*Stroke_it)->srtPoint,(*Stroke_it)->endPoint, k_depth, S);
				mm_aStroke_set[i].push_back(me_node);
				kk++;
			}
		}

		r_cout << " sobel added " << kk << " strokes : to depth  " << k_depth << endl;
		depth_sobel = k_depth;
		//mm_aStroke_set->sort();
		stroke_dump(mm_aStroke_set, m_tag, mm_depth);
		kk = 0;
		k_depth = 0;
		int s_depth = render_sobel->mm_depth-1;
		for (int i = s_depth; i < render_saliency->mm_depth; i++) {
			//r_cout << "astroke.size()= " + m_tag << ", " << mm_aStroke_set[i].size() << endl;
			int mod = 0;
			for (list<Stroke_Node*>::iterator Stroke_it = Stroke_set_saliency[i].stroke_list.begin(); Stroke_it != Stroke_set_saliency[i].stroke_list.end(); Stroke_it++) {
				mod++;
				//if ((mod % 2) == 0) continue;
				if (render_method == RENDER_TWOPASS_MERGE)
					k_depth = (*Stroke_it)->depth;
				else { //RENDER_TWOPASS_ATTACH
							k_depth = (*Stroke_it)->depth+1 ;
				}

			//	info = (*Stroke_it)->info;
				S = (*Stroke_it)->avgS;
				me_node = new Stroke_Node((*Stroke_it)->srtPoint,(*Stroke_it)->endPoint, k_depth, S);
				mm_aStroke_set[k_depth].push_back(me_node);
				kk++;
			}
		}
		r_cout << " saliency added " << kk << ": " << k_depth << endl;

		//depth_saliency = render_saliency->mm_depth;
		//mm_depth=depth_attach = k_depth+1;
		mm_depth = k_depth + 1;
		for (int i = 0; i < mm_depth; i++) {
			r_cout << m_tag + "  _aStrokeset_size() : " <<i << ": " << mm_aStroke_set[i].stroke_list.size() << endl;
		}
	}

	//m_aStroke_set->sort(Img_node::greater<Img_node*>());


//	sort(m_aStroke_set->begin(), m_aStroke_set->end());
	//m_aStroke_set->sort();
	mm_depth = draw_grid_depth(r_grid_map_1c, r_grid_map_1c_accu, mm_aStroke_set, m_t,
		grid_map_sum, QT_grid_count//, true, -1, 0
	);
	k_depth = -1;
	int astroke_depth;
	int k = 0;
	for(int i=0;i<mm_depth;i++){
		if (mm_aStroke_set[i].stroke_list.size() == 0) {
			r_cout << m_tag + "Fail Sort check _aStrokeset_size() : " << k_depth << ": " << 
				mm_aStroke_set[i].stroke_list.size() << endl;
		}
	for (list<Stroke_Node*>::iterator Stroke_it = mm_aStroke_set[i].stroke_list.begin(); 
		Stroke_it != mm_aStroke_set[i].stroke_list.end(); Stroke_it++, k++)
	{
		astroke_depth = (*Stroke_it)->depth;
		if (k_depth != -1) {
			if (astroke_depth < k_depth ||
				astroke_depth > k_depth + 1)
			{
				r_cout << "sort fail  " << k << "th : " << k_depth << " -> " << astroke_depth << endl;
			}
		}
		k_depth = astroke_depth;
	}
	r_cout << m_tag + " Sort check _aStrokeset_size() : " << k_depth<<": "<<mm_aStroke_set[i].stroke_list.size() << endl;
	}

	/*for (int i = 0; i <= MAX_DEPTH; i++) {
	

		debug_image("ing/_or_grid_" + m_tag +
			"_" +  "_", i, r_grid_map_1c[i]);

	}
	*/
	
	QT_depth = mm_depth;
	
	//r_cout << "x depth " + m_tag + " = " << mm_depth  << endl;
	
	for (int i = 0; i < mm_depth ; i++)
	{
		r_cout << setw(15) << m_tag + ", " + to_string(i) <<", "<< QT_grid_count[i] << endl;
	}
	calc_brush_size(BrushMaxSize, BrushMinSize, mm_depth,
	brush_size, m_tag);

	ret = BrushInitialization(brush_set, mm_depth, brush_size);
	if (ret != 0) {
		r_cout << "Brush Initialization Error_" + m_tag << endl;
		cerr << "Brush Initialization Error_" + m_tag << endl;
		return -1001;
	}
	brush_resize();

	////for (int i = 0; i < mm_depth; i++) {
	//	r_cout << "Brush "<<i << ", " << brush_size[i] << endl;
	//}
//	for (int i = 0; i < mm_depth; i++) {
//		r_cout << "QT stroke count(" + m_tag + ") " << QT_depth << " " << mm_depth << " : " << mm_aStroke_set[i].size() << endl;
	//	r_cout.flush();
	//	if (mm_aStroke_set[i].size() == 0) {
	//		r_cout << "unexpected Stroke Tree" + m_tag + " = 0" << endl;
	//		return -20000;
	//	}
	//}
	//ret = draw_grid_2(m_srcImg_.clone(), m_aStroke_set, "src_0_" + m_tag, m_depth, -1, 0,m_tag);
	r_cout << "------------------------------------------------------------------" << endl;
	r_cout.flush();
	return 0;
}

int  render_::TakeQuadTree(cv::Mat &SaliencyMap, Stroke_set aStroke_set[], string tag)
{
	//cv::Mat Quad_treeMap;
	Mat Quad_treeMap = SaliencyMap.clone();
	Mat stageMap = SaliencyMap.clone();
	int last_depth = 0;

	int width = SaliencyMap.size().width, height = SaliencyMap.size().height;
	unsigned char* srcData = (unsigned char*)SaliencyMap.data;
	Stroke_Node* root;
	// Imginfo root_Info;
	string quad = "0_whole";

	double avgS = TakeAvgS(SaliencyMap, Point(0, 0), Point(width - 1, height - 1), 0, quad);
	root = new Stroke_Node(Point(0, 0), Point(width - 1, height - 1), 0, avgS);
	/*	 root.srtPoint.x = 0;
	root_Info.srtPoint.y = 0;
	root_Info.endPoint.x = width-1 ;
	root_Info.endPoint.y = height-1;


	root = newImageTree(root_Info, 0,avgS);

	*/
	last_depth = DivideImage(SaliencyMap, root, aStroke_set, string("root"), Quad_treeMap, stageMap, 0, tag);
	//	cout		<< "after  aStroke size" << aStroke.size() << endl;


	//cv::String f_path = cv::format("%s/org_quad_tree.ppm", g_para_method_path.c_str());//1ch
	//cv::imwrite(f_path, Quad_treeMap);
	//return Quad_treeMap;
	return last_depth;
}

int render_::DivideImage(cv::Mat &SaliencyMap, Stroke_Node* me_node, Stroke_set aStroke_set[],
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
	Stroke_Node *new_node[5];

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

	int ret = aStroke_set[me_node->depth].push_back(me_node);
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


	if (child_QT_depth <= g_depth_limit)
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
			if (avgS[quadrant] != D[quadrant - 1]) {
				cout << "depth mismatch " << child_QT_depth << " " << quad << D[quadrant - 1] << "!= " << avgS[quadrant] << endl;
			}
			if (avgS[quadrant] > g_QT_avgSThreshold) {
				new_node[quadrant] = new Stroke_Node(c_srtPoint[quadrant], c_endPoint[quadrant], child_QT_depth, avgS[quadrant]);

				get_depth = DivideImage(SaliencyMap, new_node[quadrant], aStroke_set,
					quad + to_string(child_QT_depth), stageMap, gradient_src, child_QT_depth, tag);

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
	if (depth >= g_trace_depth) {
		if (quad != "KK") {
			cout << "depth " << depth << " :" << quad << ": (x,y) " << setw(5) << srtPoint.x << " " << srtPoint.y <<
				" :e( " << endPoint.x << " " << endPoint.y;

			cout << " s_ : " << setw(5) << s_w << setw(5) << s_h << " 2*w*h= " << g_qt_s_scale*(s_w + s_h)
				<< " S " << setw(15) << S << setw(15) << " avgS : " << avgS << endl;
		}
	}
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