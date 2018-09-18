// SaliencyPainterlyRendering.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "thread"
//#include "SaliencyPainterlyRendering.h"
#include "util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "debug_image.h"
#include "use_json.h"
#include "set_result_path.h"
#include "RenderingImage.h"
// 유일한 응용 프로그램 개체입니다.
#include "util.h"

#include "opencv2/saliency.hpp"
#include "saliency_blackandwhite.h"

using namespace std;
using namespace cv;

using namespace saliency;
#include "RenderingImage.h"

#include "json_read.h"
#include "saliency_main.h"
#include "extern.h"
#include "g_setup.h"
#include "draw_grid2.h"
int Brush_adjust(vector<Brush> &brush_set);

void my_delete(QuadTree::Img_node *p)
{
 free( p);
}

void g_brush_delete(list<Brush> &brush_set) {
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

cv::Mat get_sobel_map(Mat & srcImg) {
	cv::Mat gray_Map;
	cv::Mat org_Gaussian_Map;
	cv::Mat sobel_map;
	//Take GradientImg from source Image
	cv::cvtColor(srcImg, gray_Map, CV_BGR2GRAY);							//3ch -> 1ch
																			//f_path = cv::format("%s/src_1ch.ppm", g_para_method_path.c_str());
																			//cv::imwrite(f_path, gray_Map);
	debug_image("src_1ch", gray_Map);
	int gray_map_width = gray_Map.size().width;
	int gray_map_height = gray_Map.size().height;

	//	org_Gaussian_Map = bsShowImage::TakeGaussianBlurImg(gray_Map);

	cv::GaussianBlur(gray_Map, org_Gaussian_Map, Size(3, 3), 0);
	debug_image("src_take_blur", org_Gaussian_Map);
	int ddepth = CV_16S;
	//org_sobeldient_Map = bsShowImage::TakeGradient(org_Gaussian_Map);

	cv::Mat grad_x, grad_y, abs_sobeld_x, abs_sobeld_y;

	cv::Sobel(org_Gaussian_Map, grad_x, ddepth, 1, 0);
	cv::convertScaleAbs(grad_x, abs_sobeld_x);
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	cv::Sobel(org_Gaussian_Map, grad_y, ddepth, 0, 1);
	cv::convertScaleAbs(grad_y, abs_sobeld_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_sobeld_x, 0.5, abs_sobeld_y, 0.5, 0, sobel_map);
	return sobel_map;
}


Mat saliency_union(Mat saliency_map_C, Mat saliency_map_G) {
	Mat union_map;
	
	unsigned char * saliency_map_C_data = saliency_map_C.data;
	unsigned char * saliency_map_G_data = saliency_map_G.data;
	int width = saliency_map_C.cols;
	int height = saliency_map_C.rows;
	union_map.create(height, width, CV_8UC1);
	unsigned char * union_map_data = union_map.data;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int ind = y*width + x;
			if (*(saliency_map_C_data + ind) < *(saliency_map_G_data + ind))
			{
				*(union_map_data + ind) = *(saliency_map_G_data + ind);
			}
			else 
			{
				*(union_map_data + ind) = *(saliency_map_C_data + ind);
			}
			
		}//x

	}//y
	
	return union_map;
}

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

bool compareDepth_I(QuadTree::Img_node* A, QuadTree::Img_node* B)
{
	return A->depth < B->depth ? true : (A->depth == B->depth ? true : false);
}

int // return depth
draw_grid_depth(Mat _Quad_TreeMap,
	vector<QuadTree::Img_node*> aStroke_set, string tag, int & depth,int & grid_map_sum, 
	Mat  _grid_map_1c[],int _QT_grid_count[], bool do_overlay_grid,int draw_depth,int c) {
	int __saved_depth=-1;
	Mat overlay_grid_map[MAX_DEPTH];


		for (int i = 0; i < MAX_DEPTH; i++) {
			overlay_grid_map[i] = _Quad_TreeMap.clone();
		}
	
	
	for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set.begin(); St_it != aStroke_set.end(); St_it++)
	{

		if (__saved_depth != (*St_it)->depth && __saved_depth != -1) {
		
				if ( draw_depth ==-1 || __saved_depth==draw_depth)
					debug_image("ing/_o_grid_" + tag, __saved_depth, overlay_grid_map[__saved_depth]);

				debug_image("_grid_" + tag, __saved_depth, _grid_map_1c[__saved_depth]);

			cout << "depth " + tag << setw(5) << __saved_depth << " : " << _QT_grid_count[__saved_depth] << endl;
			if (do_overlay_grid) {
				grid_map_sum += _QT_grid_count[__saved_depth];
			}
		}
		if ( __saved_depth < (*St_it)->depth)
			__saved_depth = (*St_it)->depth;

		cv::rectangle(_Quad_TreeMap, Rect((*St_it)->info.srtPoint,
			(*St_it)->info.endPoint), RGB(255,255, 255));
		cv::rectangle(_grid_map_1c[__saved_depth], Rect((*St_it)->info.srtPoint,
			(*St_it)->info.endPoint), RGB(c, c, c));

		if (do_overlay_grid) {
			_QT_grid_count[__saved_depth]++;
		}
	
			if (draw_depth == -1 || __saved_depth == draw_depth)
				cv::rectangle(overlay_grid_map[__saved_depth], Rect((*St_it)->info.srtPoint,
				(*St_it)->info.endPoint), RGB(255, 255, 255));
	
	}//end of QuadTree dump

	if (do_overlay_grid)
		grid_map_sum += _QT_grid_count[__saved_depth];
	
	debug_image("_grid_" + tag+"_"+to_string(__saved_depth)+"_f",  _grid_map_1c[__saved_depth]);
	
		if (draw_depth == -1 || __saved_depth == draw_depth)
		debug_image("ing/_o_grid_" + tag + "_" + to_string(__saved_depth) + "_f", overlay_grid_map[__saved_depth]);
		debug_image("QT_"+tag+ "_" + to_string(__saved_depth) + "_f", _Quad_TreeMap);

	cout << "depth " + tag << setw(5) << __saved_depth << " : " << _QT_grid_count[__saved_depth] << endl;
	if (__saved_depth > depth) {
		cout << "depth changed  : " << __saved_depth << endl;
	}
	depth = __saved_depth + 1;
	cout << "finalized Depth : " << depth  << " : " << _QT_grid_count[__saved_depth] << endl;
	cout << tag+" grid sum " << grid_map_sum << endl;
	
	return depth;
}

int calc_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
	int _brush_size[],string tag)
{
	int brush_step = (int)((_BrushMaxSize - _BrushMinSize) / _depth);
		
	cout << tag+ " image depth  " << _depth << ", size: " << g_src_image_width << ",  " << g_src_image_height << endl;
	cout << "_BrushMaxSize " << _BrushMaxSize <<"      ";
	cout << "_BrushMinSize " << _BrushMinSize <<"       ";
	cout << "_brush_step " << brush_step << endl;

	for (int i = 0; i < _depth; i++)
	{
		_brush_size[i] = (int)(g_BrushMaxSize - (i)* brush_step);

		if (_brush_size[i] < _BrushMinSize)
			_brush_size[i] = _BrushMinSize;
		if ( i==0 )_brush_size[0] = (int)(g_BrushMaxSize*1.5);
		cout << tag<<"   Brush Size : " << i << ", " << setw(4) << _brush_size[i]  << endl;
		g_file_cstat << g_para_method + "," << g_image_name << ", " << "brush_size" + to_string(i) + "," <<
			_brush_size[i]  << endl;
	}

	return _depth;
}
cv::Mat m_srcImg;
Mat sobel_grid_map_1c[MAX_DEPTH];

Mat Sgrid_grid_map_1c[MAX_DEPTH];
Mat Scol_grid_map_1c[MAX_DEPTH];
Mat union_grid_map_1c[MAX_DEPTH];
Mat twopass_grid_map_1c[MAX_DEPTH];

vector<QuadTree::Img_node*> aStroke_set_sobel;
//vector<QuadTree::Img_node*>aStroke_set_Sgrad;
vector<QuadTree::Img_node*> aStroke_set_Scol;
vector<QuadTree::Img_node*> aStroke_set_union;
vector<QuadTree::Img_node*> aStroke_set_twopass;
list<Brush> sobel_brush_set;
list<Brush> Scol_brush_set;
//list<Brush> Sgrad_brush_set;
list<Brush> union_brush_set;
list<Brush> twopass_brush_set;

int sobel_brush_size[MAX_DEPTH];
int saliency_brush_size[MAX_DEPTH];
int union_brush_size[MAX_DEPTH];
int twopass_brush_size[MAX_DEPTH];

int sobel_depth;
int saliency_depth;
int union_depth;
int twopass_depth;

Mat sobel_result_image;
Mat saliency_result_image;
Mat union_result_image;
Mat twopass_result_image;

Mat Quad_TreeMap_sobel;
Mat Quad_TreeMap_saliency;
Mat Quad_TreeMap_union;
Mat Quad_TreeMap_twopass
;
int sobel_QT_grid_count[MAX_DEPTH];
int saliency_QT_grid_count[MAX_DEPTH];
int union_QT_grid_count[MAX_DEPTH];
int twopass_QT_grid_count[MAX_DEPTH];

int sobel_changed_count[MAX_DEPTH];
int saliency_changed_count[MAX_DEPTH];
int union_changed_count[MAX_DEPTH];
int twopass_changed_count[MAX_DEPTH];

int sobel_Sgrid_painting_try[MAX_DEPTH];
int saliency_Sgrid_painting_try[MAX_DEPTH];
int union_Sgrid_painting_try[MAX_DEPTH];
int twopass_Sgrid_painting_try[MAX_DEPTH];

long int sobel_painting_area[MAX_DEPTH];
long int saliency_painting_area[MAX_DEPTH];
long int union_painting_area[MAX_DEPTH];
long int twopass_painting_area[MAX_DEPTH];



//cv::Mat gradient_Map_union;
Mat union_Map_C;
Mat union_Map_G;
Mat gra_Map_sobel;
Mat gra_Map_twopass;
Mat gra_Map_union;
Mat gra_Map_saliency;

string tag_sobel="0_sobel";
string tag_union = "1_union";
string tag_saliency = "2_saliency";
string tag_twopass = "3_twopass_";
string tag_sobel_ = "0_sobel_";
string tag_union_ = "1_union_";
string tag_saliency_ = "2_saliency_";
string tag_twopass_ = "3_twopass_";
string tag__sobel = "_0_sobel";
string tag__union = "_1_union";
string tag__saliency = "_2_saliency";
string tag__twopass = "_3_twopass";
void func_(int _depth, int _changed_count[], int _QT_grid_count[], int _Sgrid_painting_try[], int _m_brush_size[], long int _painting_area[]) {
	int Qt_sum = 0;
	int try_sum = 0;
	int c_sum = 0;
	int t_sum = 0;
	//_painting_area[astroke_depth] += st_w_size*st_h_size;
	cout << "g_Scale = " << g_paint_grid_scale << endl;
	cout <<endl<<
		setw(6)<< "[]"<<
		setw(7)<<"QT_cnt" << 
		setw(6) << "try" << 
		setw(11) << "try/QT" << 
		setw(8)	<<"changed"<<
		setw(10)<<"ch ratio"<<
		setw(12)<<"brush_size"<<
		setw(12)<<"b area*try"<<
		setw(15)<<"painting_Area"<<
		setw(14)<<"p_area/try" <<
		setw(16)<<"p_area/p_Area"<<
		endl;
	for (int i = 0; i < _depth; i++) {
		cout << setw(5) << i << "," << 
			setw(6) << _QT_grid_count[i] << ", "<<
		 setw(6)  << _Sgrid_painting_try[i] << ", "<<

		 setw(8) << (int)(_Sgrid_painting_try[i] / _QT_grid_count[i])<<
			setw(8) << _changed_count[i] << ",  " <<
			std::fixed << 
			std::setw(7) << std::setprecision(2)<<
			(int)((float)_changed_count[i] / (float)(_Sgrid_painting_try[i] )* 100.) <<
			setw(6) <<			_m_brush_size[i] <<
			setw(6) << _m_brush_size[i] * _m_brush_size[i] <<
			setw(10)<<			_m_brush_size[i] * _m_brush_size[i] * _Sgrid_painting_try[i]
			<< setw(17) << _painting_area[i]
			<<setw(15)<< (int)((_painting_area[i]/_Sgrid_painting_try[i])*100.)
			
			<< setw(12) << (int)(_m_brush_size[i] * _m_brush_size[i] * _Sgrid_painting_try[i] / _painting_area[i])
			<< endl;
		Qt_sum += _QT_grid_count[i];
		try_sum += _Sgrid_painting_try[i];
		//	t_sum += try_count[i];
		c_sum += _changed_count[i];

		g_file_cstat << g_para_method + "," << g_image_name << ", " << "depth:pr_count changed: " + to_string(i) + "," <<

			_Sgrid_painting_try[i] << "," << to_string(_changed_count[i]) << endl;
	}

	cout << setw(12) << Qt_sum << setw(18) << try_sum << setw(8) << c_sum << endl;

}

void run_sobel() {
	sobel_result_image = PainterlyRendering(m_srcImg, aStroke_set_sobel, tag_sobel,
		sobel_brush_size, sobel_depth, sobel_grid_map_1c, sobel_Sgrid_painting_try, sobel_brush_set, sobel_QT_grid_count,
		sobel_changed_count,sobel_painting_area
	);

	image_save(g_image_name + tag__sobel, sobel_result_image, Quad_TreeMap_sobel);
	//cout << "sobel_ " << sobel_depth << " : " << aStroke_set_sobel.size() << endl;
}
void run_saliency() {
	saliency_result_image = PainterlyRendering(m_srcImg, aStroke_set_Scol, tag_saliency,
		saliency_brush_size, saliency_depth, Scol_grid_map_1c, saliency_Sgrid_painting_try,
		Scol_brush_set, saliency_QT_grid_count,saliency_changed_count
		,saliency_painting_area
	);
	image_save(g_image_name + tag__saliency, saliency_result_image, Quad_TreeMap_saliency);
}
void run_union() {
	union_result_image = PainterlyRendering(m_srcImg, aStroke_set_union, tag_union,

		union_brush_size, union_depth, union_grid_map_1c, union_Sgrid_painting_try,
		union_brush_set, union_QT_grid_count,union_changed_count
		,union_painting_area
		);


	image_save(g_image_name + tag__union, union_result_image, Quad_TreeMap_union);
}

void run_twopass() {
	twopass_result_image = PainterlyRendering(m_srcImg, aStroke_set_twopass, tag_twopass,

		twopass_brush_size, twopass_depth, twopass_grid_map_1c, twopass_Sgrid_painting_try,
		twopass_brush_set, twopass_QT_grid_count, twopass_changed_count
		, twopass_painting_area
	);


	image_save(g_image_name + tag__twopass, twopass_result_image, Quad_TreeMap_twopass);
}
int   RenderingImage(char * src_name, char * deploy_name)
{

	//	cv::Mat org_saliency_sobeld_Map;
		//, org_QuadTreeMap, org_DensityMap, org_rstImg;

	Mat sobel_8UC1;
	Mat sobel_8UC3;

	Mat saliency_finegrained_C;
	Mat saliency_pregraph_C;
	Mat saliency_blackandwhite_C;
	Mat saliency_residual_C;
	Mat saliency_itti_C;
	Mat saliency_finegrained_G;
	Mat saliency_pregraph_G;
	Mat saliency_blackandwhite_G;
	Mat saliency_residual_G;
	Mat saliency_itti_G;

	int get_depth_saliency;
	int get_depth_twopass;
	int get_depth_sobel;
	int get_depth_union;

	int saved_depth = -1;
	int sobel_grid_map_sum = 0, sobel_grid_count_sum = 0;
	int saliency_grid_map_sum = 0, saliency_grid_count_sum = 0;
	int union_grid_map_sum = 0, union_grid_count_sum = 0;
	int twopass_grid_map_sum = 0, twopass_grid_count_sum = 0;


	//Mat saliency_grid_map[MAX_DEPTH];
	//Mat union_grid_map[MAX_DEPTH];
	//Mat sobel_grid_map[MAX_DEPTH];

	//Mat saliency_w_src_grid_map[MAX_DEPTH];
//	Mat union_w_src_grid_map[MAX_DEPTH];
//	Mat sobel_w_src_grid_map[MAX_DEPTH];

//	Mat saliency_w_rst_grid_map[MAX_DEPTH];
	//Mat union_w_rst_grid_map[MAX_DEPTH];
	//Mat sobel_w_rst_grid_map[MAX_DEPTH];




	 //vector<QuadTree::Img_node*> aStroke_set_union;
	//cv::Mat rstImg;
	//cv::String f_path;
	int ret = set_global(string(src_name), string(deploy_name));
	if (ret < 0)
		return -7777;

	cout << "after global" << endl;
	sobel_depth = g_depth_limit;
	saliency_depth = g_depth_limit;
	union_depth = g_depth_limit;
	twopass_depth = g_depth_limit;
	for (int i = 0; i < MAX_DEPTH; i++) {
		sobel_QT_grid_count[i] = 0;
		saliency_QT_grid_count[i] = 0;
		union_QT_grid_count[i] = 0;
		twopass_QT_grid_count[i] = 0;

		sobel_Sgrid_painting_try[i] = 0;
		saliency_Sgrid_painting_try[i] = 0;
		union_Sgrid_painting_try[i] = 0;
		twopass_Sgrid_painting_try[i] = 0;
	}
	m_srcImg = cv::imread(src_name, CV_LOAD_IMAGE_COLOR);

	if (m_srcImg.cols == 0
		|| m_srcImg.rows == 0)
	{
		cout << "Src " << src_name << " size 0" << endl;
		cerr << "Src " << src_name << " size 0" << endl;

		return -100;
	}
	debug_image("src_image", m_srcImg);
	//Point St_srtPoint, St_endPoint, fetch_color_Point, centered_SrtPoint, centered_EndPoint;
	g_src_image_width = m_srcImg.size().width;
	g_src_image_height = m_srcImg.size().height;
	g_src_image_channels = m_srcImg.channels();
	g_src_image_step1 = (int)m_srcImg.step1();


	//Brush initialization
	sobel_brush_set.clear();
	Scol_brush_set.clear();
	union_brush_set.clear();
	twopass_brush_set.clear();

	aStroke_set_sobel.clear();
	aStroke_set_Scol.clear();
	aStroke_set_union.clear();
	aStroke_set_twopass.clear();

	//g_saliency_method;//without, itti, pregraph, blackandwhite,resudual,fine_sobelined


	cout << "Saliency Method " << g_saliency_method << endl;
	cout << "g_twopass_method " << g_Render_method << endl;

	gra_Map_sobel = sobel_8UC1 = get_sobel_map(m_srcImg);

	//debug_image("sobel_8UC1", sobel_8UC1);

	cv::cvtColor(sobel_8UC1, sobel_8UC3, CV_GRAY2RGB);

	if (g_saliency_method == string("Residual")
		|| g_saliency_method == string("residual")
		) {

		Mat saliency_residual_8UC1;
		Ptr<StaticSaliencySpectralResidual> saliencyAlgorithm = StaticSaliencySpectralResidual::create();
		saliencyAlgorithm->computeSaliency(m_srcImg, saliency_residual_8UC1);
		union_Map_C = saliency_residual_C = saliency_residual_8UC1;

		saliencyAlgorithm->computeSaliency(sobel_8UC3, saliency_residual_8UC1);
		union_Map_G = saliency_residual_G = saliency_residual_8UC1;

	}
	else if (g_saliency_method == string("Fine_grained")
		|| g_saliency_method == string("fine_grained")) {
		Mat saliency_finegrained_8UC1;
		Ptr<StaticSaliencyFineGrained> saliencyAlgorithmG = StaticSaliencyFineGrained::create();
		saliencyAlgorithmG->computeSaliency(m_srcImg, saliency_finegrained_8UC1);
		union_Map_C = saliency_finegrained_C = saliency_finegrained_8UC1;

		saliencyAlgorithmG->computeSaliency(sobel_8UC3, saliency_finegrained_8UC1);
		union_Map_G = saliency_finegrained_G = saliency_finegrained_8UC1;

	}
	else if (g_saliency_method == string("Blackandwhite")
		|| g_saliency_method == string("blackandwhite")) {
		Mat saliency_blackandwhite_8UC1;
		saliency_blackandwhite_8UC1 = saliency_blackandwhite_main(m_srcImg);
		union_Map_C = saliency_blackandwhite_8UC1;

		saliency_blackandwhite_8UC1 = saliency_blackandwhite_main(sobel_8UC3);
		union_Map_G = saliency_blackandwhite_8UC1;
	}
	else if (g_saliency_method == string("Pregraph")
		|| g_saliency_method == string("pregraph")) {
		Mat saliency_pregraph_8UC1;
		Mat saliency_pregraph_32F;
		saliency_pregraph_32F = preGraph_main(m_srcImg);
		saliency_pregraph_32F.convertTo(saliency_pregraph_8UC1, CV_8UC1, 255.);
		union_Map_C = saliency_pregraph_8UC1;

		saliency_pregraph_32F = preGraph_main(sobel_8UC3);
		saliency_pregraph_32F.convertTo(saliency_pregraph_8UC1, CV_8UC1, 255.);
		union_Map_G = saliency_pregraph_8UC1;

	}
	else if (g_saliency_method == string("Itti")
		|| g_saliency_method == string("itti")) {
		Mat saliency_itti_32F;

		saliency_itti_32F = saliency_itti_main(m_srcImg, "C");
		saliency_itti_32F.convertTo(union_Map_C, CV_8UC1, 255.);

		saliency_itti_32F = saliency_itti_main(sobel_8UC3, "G");
		saliency_itti_32F.convertTo(union_Map_G, CV_8UC1, 255.);


	}
	else if (g_saliency_method == string("Sobel")
		|| g_saliency_method == string("sobel")) {

		union_Map_C = sobel_8UC1;
		union_Map_G = sobel_8UC1;

	}

	else {
		cout << "No method assiegnd" << endl;
		return -1;
	}
	debug_image("0_sobel_gradient", sobel_8UC1);
	debug_image("0_Saliency_C_" + g_saliency_method, union_Map_C);
	debug_image("0_Saliency_G_sobel", union_Map_G);


	gra_Map_saliency = union_Map_C;
	gra_Map_twopass = gra_Map_union = saliency_union(union_Map_C, union_Map_G);

	debug_image("0_Saliency_union_gra(sobel)_gra(" + g_saliency_method + ")", gra_Map_union);
	//end of take gradient pass
	// QuadTree pass




	Quad_TreeMap_sobel = gra_Map_sobel;
	if (g_Render_method != "Sobel" &&
		g_Render_method != "sobel"
		) {
		Quad_TreeMap_saliency = gra_Map_saliency;
	}
	Quad_TreeMap_union = gra_Map_union.clone();
	Quad_TreeMap_twopass = gra_Map_union.clone();

	if (gra_Map_sobel.cols == 0 ||
		gra_Map_sobel.rows == 0)
	{
		cout << "unexpected sobel_gradient_Map = 0" << endl;
		return -20002;
	}



	if (g_src_image_width < g_src_image_height)
		g_BrushMaxSize = g_src_image_width / 10;
	else
		g_BrushMaxSize = g_src_image_height / 10;

	g_canvas_size_bezel_size = g_BrushMaxSize;

	//Rendering
	g_canvas_size_width = g_src_image_width + g_canvas_size_bezel_size * 2;
	g_canvas_size_height = g_src_image_height + g_canvas_size_bezel_size * 2;

	for (int i = 0; i < MAX_DEPTH; i++) {
		Scol_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		union_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		sobel_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		twopass_grid_map_1c[i].create(g_src_image_height, g_src_image_width, CV_8UC1);
		Scol_grid_map_1c[i].setTo(255);
		sobel_grid_map_1c[i].setTo(255);
		union_grid_map_1c[i].setTo(255);
		twopass_grid_map_1c[i].setTo(255);
	}
	/* cloned in draw_grid

	for (int i = 0; i < MAX_DEPTH; i++) {
		saliency_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		union_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		sobel_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);


		saliency_w_src_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		union_w_src_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		sobel_w_src_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);

		saliency_w_rst_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		union_w_rst_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);
		sobel_w_rst_grid_map[i].create(g_src_image_height, g_src_image_width, CV_8UC3);



	}
	*/





	g_file_cstat << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;
	cout << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;

	//preepare GRID using Quad_Tree
	get_depth_sobel = QuadTree::TakeQuadTree(gra_Map_sobel, aStroke_set_sobel, tag_sobel);
	ret = draw_grid_depth(Quad_TreeMap_sobel, aStroke_set_sobel, tag_sobel, sobel_depth,
		sobel_grid_map_sum, sobel_grid_map_1c, sobel_QT_grid_count, true, -1, 0);

	cout << "depth sobel = " << ret << " : " << sobel_depth << endl;
	calc_brush_size(g_BrushMaxSize, g_BrushMinSize, sobel_depth,
		sobel_brush_size, tag_sobel);
	ret = BrushInitialization(sobel_brush_set, sobel_depth, sobel_brush_size);
	if (ret != 0) {
		cout << "Brush Initialization Error" << endl;
		cerr << "Brush Initialization Error" << endl;
		return -1001;
	}



	cout << "QT stroke count(sobel) " << get_depth_sobel << " " << aStroke_set_sobel.size() << endl;
	cout.flush();
	if (aStroke_set_sobel.size() == 0) {
		cout << "unexpected Stroke Tree Sobel = 0" << endl;
		return -20000;
	}
	ret = draw_grid_2(m_srcImg.clone(), aStroke_set_sobel, "src_0sobel", sobel_depth, -1,0);


	get_depth_union = QuadTree::TakeQuadTree(gra_Map_union, aStroke_set_union, tag_union);
	ret = draw_grid_depth(Quad_TreeMap_union, aStroke_set_union, tag_union, union_depth,
		union_grid_map_sum, union_grid_map_1c, union_QT_grid_count, true, -1,0);

	cout << "depth union = " << ret << " : " << union_depth << endl;
	calc_brush_size(g_BrushMaxSize, g_BrushMinSize, union_depth,
		union_brush_size, tag_sobel);
	ret = BrushInitialization(union_brush_set, union_depth, union_brush_size);
	if (ret != 0) {
		cout << "Brush Initialization Error" << endl;
		cerr << "Brush Initialization Error" << endl;
		return -1002;
	}

	cout << "QT stroke count(union) " << get_depth_union << " " << aStroke_set_union.size() << endl;
	cout.flush();
	if (aStroke_set_union.size() == 0) {
		cout << "unexpected Stroke Tree Union = 0" << endl;
		return -20001;
	}

	ret = draw_grid_2(m_srcImg.clone(), aStroke_set_union, "src_1union", union_depth, -1, 0);

	if (g_Render_method == "Twopass" || g_Render_method == "twopass"
		|| g_Render_method == "Saliency"// { //Sobel, Union
		|| g_Render_method == "saliency") {
		get_depth_saliency = QuadTree::TakeQuadTree(gra_Map_saliency, aStroke_set_Scol, string(tag_saliency));
		ret = draw_grid_depth(Quad_TreeMap_saliency, aStroke_set_Scol, tag_saliency, saliency_depth,
			saliency_grid_map_sum, Scol_grid_map_1c, saliency_QT_grid_count, true, -1, 0);
		cout << "depth saliency = " << ret << " : " << saliency_depth << endl;
		calc_brush_size(g_BrushMaxSize, g_BrushMinSize, saliency_depth,
			saliency_brush_size, tag_saliency);
		ret = BrushInitialization(Scol_brush_set, -1, saliency_brush_size);
		if (ret != 0) {
			cout << "saliency Brush Initialization Error" << endl;
			cerr << "saliency Brush Initialization Error" << endl;
			return -1003;
		}

		cout << "saliency " << saliency_depth << " : " << aStroke_set_Scol.size() << endl;
		ret = draw_grid_2(m_srcImg.clone(), aStroke_set_Scol, "src_2saliency", saliency_depth, -1,0);

	}



	if (g_Render_method == "Twopass"
		|| g_Render_method == "twopass") {
		saved_depth = -1;
		QuadTree::Img_node *me_node;
		QuadTree::Imginfo info;
		int depth, S;

		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_sobel.begin(); St_it != aStroke_set_sobel.end(); St_it++) {
			//if ((*St_it)->depth < 3) continue;;
			info = (*St_it)->info;
			depth = (*St_it)->depth;
			S = (*St_it)->avgS;
			me_node = QuadTree::copyImageTree(info, depth, S);
			//	push_back(aStroke_set_union.begin(), aStroke_set_union.end(), me_node, compareDepth_I);
			vector<QuadTree::Img_node*>::iterator Qt_it = lower_bound(aStroke_set_twopass.begin(), aStroke_set_twopass.end(), me_node, compareDepth_I);
			aStroke_set_twopass.insert(Qt_it, me_node);

			//	aStroke_set_union.push_back(me_node);
		}

		cout << "Twopass_sobel_aStrokeset_size() : " << aStroke_set_twopass.size() << endl;
		//int grad_depth = depth;
		int saved_dd = -1;
		saved_depth = -1;
		int depth_count = 0;
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_Scol.begin(); St_it != aStroke_set_Scol.end(); St_it++) {
			if ((*St_it)->depth < (saliency_depth - 2)) continue;;
			info = (*St_it)->info;
			if (saved_depth != (*St_it)->depth) {
				if (g_merge_method == DEF_ATTACH) {
					saved_dd += 1;
					saved_depth = (*St_it)->depth;
					depth = sobel_depth + saved_dd;
				}
				else depth = (*St_it)->depth;
				depth_count = 0;
			}
			depth_count++;
			S = (*St_it)->avgS;
			if (!(depth_count % g_merge_skip_count)) {
				me_node = QuadTree::copyImageTree(info, depth, S);
				vector<QuadTree::Img_node*>::iterator Qt_it = lower_bound(aStroke_set_twopass.begin(), aStroke_set_twopass.end(), me_node, compareDepth_I);
				aStroke_set_twopass.insert(Qt_it, me_node);
			}

		}
		cout << "Twopass_sobel_aStrokeset_size() : " << aStroke_set_twopass.size() << endl;

		int qdepth = -1;
		int cnt = 0;
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_twopass.begin(); St_it != aStroke_set_twopass.end(); St_it++) {
			if ((*St_it)->depth < qdepth) {
				cout << "depth(" << setw(5) << cnt << ") : " << qdepth << "   " <<
					(*St_it)->depth << endl;
			}
			qdepth = (*St_it)->depth;
			cnt++;
		}
		cout << "sobel_stroke size = " << aStroke_set_sobel.size() << " saliency : " << aStroke_set_Scol.size() <<
			" : union : " << aStroke_set_union.size() << " : twopass : " << aStroke_set_twopass.size() << endl;

		get_depth_twopass = draw_grid_depth(Quad_TreeMap_twopass, aStroke_set_twopass, tag_twopass, twopass_depth,
			twopass_grid_map_sum, twopass_grid_map_1c, twopass_QT_grid_count, true, -1, 0);

		cout << "depth twopass = " << get_depth_twopass << " : " << twopass_depth << endl;
		calc_brush_size(g_BrushMaxSize, g_BrushMinSize, twopass_depth,
			twopass_brush_size, tag_twopass);

		ret = BrushInitialization(twopass_brush_set, -1, twopass_brush_size);
		if (ret != 0) {
			cout << "twopass Brush Initialization Error" << endl;
			cerr << "twopass Brush Initialization Error" << endl;
			return -1004;
		}

		cout << "Twopass_sobel_aStrokeset_size() : " << aStroke_set_twopass.size() << endl;
		ret = draw_grid_2(m_srcImg.clone(), aStroke_set_twopass, "src_3twopass", twopass_depth, -1, 0);
	}





	std::thread *p_sobel = NULL;
	std::thread *p_union = NULL;
	std::thread *p_saliency = NULL;
	std::thread *p_twopass = NULL;
	p_sobel = new thread(run_sobel);
	if (g_Render_method != "Sobel" &&
		 g_Render_method != "sobel") {
		p_saliency = new thread(run_saliency);
		p_union = new thread(run_union);
	}
	if (g_Render_method == "Twopass"
		|| g_Render_method == "twopass")
		p_twopass = new thread(run_twopass);

	cout << "p_sobel    : " << p_sobel->get_id() << endl;

	if (g_Render_method != "Sobel" &&
		 g_Render_method != "sobel"
		) {
		cout << "p_saliency : " << p_saliency->get_id() << endl;
		cout << "p_union    : " << p_union->get_id() << endl;
	}
	if (g_Render_method == "Twopass" ||
		g_Render_method == "twopass"
		) {
		cout << "p_twopass  : " << p_twopass->get_id() << endl;
	}

	std::cout << "Number of threads = "
		<< std::thread::hardware_concurrency() << std::endl;
	
	p_sobel->join();

	if (g_Render_method != "Sobel" &&
		g_Render_method != "sobel"
		){
		p_saliency->join();
		p_union->join();
	}
		if (g_Render_method == "Twopass" ||
			g_Render_method == "twopass"

			)
			p_twopass->join();

		ret = draw_grid_2(sobel_result_image.clone(), aStroke_set_sobel, "rst_0sobel", sobel_depth, -1, 255);
		func_(sobel_depth, sobel_changed_count, sobel_QT_grid_count, sobel_Sgrid_painting_try, sobel_brush_size, sobel_painting_area);
		if (g_Render_method != "Sobel" &&
			g_Render_method != "sobel"){
			ret = draw_grid_2(saliency_result_image.clone(), aStroke_set_Scol, "rst_2saliency", saliency_depth, -1, 255);
			func_(saliency_depth, saliency_changed_count, saliency_QT_grid_count, saliency_Sgrid_painting_try, saliency_brush_size, saliency_painting_area);

			ret = draw_grid_2(union_result_image.clone(), aStroke_set_union, "rst_1union", union_depth, -1, 255);
			func_(union_depth, union_changed_count, union_QT_grid_count, union_Sgrid_painting_try, union_brush_size, union_painting_area);
		}
		if (g_Render_method == "Twopass" ||
			g_Render_method == "twopass") {
			ret = draw_grid_2(twopass_result_image.clone(), aStroke_set_twopass, "rst_3twopass", twopass_depth, -1, 255);
			func_(twopass_depth, twopass_changed_count, twopass_QT_grid_count, twopass_Sgrid_painting_try, twopass_brush_size, twopass_painting_area);
		}
		
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_sobel.begin(); St_it != aStroke_set_sobel.end(); St_it++) {
			free(*St_it);
		}
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_Scol.begin(); St_it != aStroke_set_Scol.end(); St_it++) {
			free(*St_it);
		}
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_union.begin(); St_it != aStroke_set_union.end(); St_it++) {
			free(*St_it);
		}
		for (vector<QuadTree::Img_node*>::iterator St_it = aStroke_set_twopass.begin(); St_it != aStroke_set_twopass.end(); St_it++) {
			free(*St_it);
		}
		g_brush_delete(Scol_brush_set);
		g_brush_delete(sobel_brush_set);
		g_brush_delete(union_brush_set);
		g_brush_delete(twopass_brush_set);

	aStroke_set_sobel.clear();
	aStroke_set_Scol.clear();
	aStroke_set_union.clear();
	aStroke_set_twopass.clear();
	return 0;
}

