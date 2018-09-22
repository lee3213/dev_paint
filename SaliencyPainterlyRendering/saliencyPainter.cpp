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

#include "render_.h"

Mat Sgrid_grid_map_1c[MAX_DEPTH];
Mat gradient_Map_C;
Mat gradient_Map_G;
string tag[] = { "0_sobel","1_saliency","2_union","3_two_attach","4_two_merge" };
string tag_[] = { "0_sobel_", "1_saliency_", "2_union_", "3_twoattach_","4_two_merge_" };
string _tag[] = { "_0_sobel", "_1_saliency", "_2_union", "_3_twopass","_4_twopass" };





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








void run_render(render_ * _render ) {
	_render->result_image = _render->PainterlyRendering();
	//.m_srcImg,
	//	_render[_render_method].aStroke_set, _render[_render_method].tag,
	//	_render[_render_method].brush_size, _render[_render_method].depth,
	//	_render[_render_method].grid_map_1c, _render[_render_method].grid_painting_try,
	//	_render[_render_method].brush_set, _render[_render_method].QT_grid_count,
	//	_render[_render_method].changed_count,
//		_render[_render_method].painting_area
//	);

	image_save(g_image_name + _render->m_tag_, _render->result_image, _render->grid_map_1c[MAX_DEPTH]);
//	image_save(g_image_name + _render->m_tag_, _render->result_image, _render->result_image_DEPTH]);
	//cout << "sobel_ " << sobel_depth << " : " << aStroke_set_sobel.size() << endl;
}

int   RenderingImage(char * src_name, char * deploy_name)
{

	//	cv::Mat org_saliency_sobeld_Map;
		//, org_QuadTreeMap, org_DensityMap, org_rstImg;
	render_ *_render[RENDER_MAX];
	Mat x_srcImg;

	Mat sobel_8UC1;
	Mat sobel_8UC3;
	Mat gradient_Union;

/*	Mat saliency_finegrained_C;
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
*/
	int saved_depth = -1;


	int ret = set_global(string(src_name), string(deploy_name));
	if (ret < 0)
		return -7777;

	x_srcImg = cv::imread(src_name, CV_LOAD_IMAGE_COLOR);

	if (x_srcImg.cols == 0
		|| x_srcImg.rows == 0)
	{
		cout << "Src " << src_name << " size 0" << endl;
		return -100;
	}
	

	
	//Point St_srtPoint, St_endPoint, fetch_color_Point, centered_SrtPoint, centered_EndPoint;
	g_src_image_width = x_srcImg.size().width;
	g_src_image_height = x_srcImg.size().height;
	g_src_image_channels = x_srcImg.channels();
	g_src_image_step1 = (int)x_srcImg.step1();

	g_file_cstat << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;
	cout << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;


	debug_image("src_image", x_srcImg);
	for (int i = 0; i < RENDER_MAX; i++) {
		_render[i] = new  render_(i,tag[i], tag_[i], _tag[i], x_srcImg);
	}

	//Brush initialization


	//g_saliency_method;//without, itti, pregraph, blackandwhite,resudual,fine_sobelined


	cout << "Saliency Method " << g_saliency_method << endl;
	cout << "g_Render_method " << g_Render_method << endl;

	sobel_8UC1 = get_sobel_map(x_srcImg);
	if (sobel_8UC1.cols == 0 ||
		sobel_8UC1.rows == 0)
	{
		cout << "unexpected sobel_gradient_Map = 0" << endl;
		return -20002;
	}

	debug_image("0_sobel_gradient", sobel_8UC1);

	if (g_Render_method != "Sobel") {
		cv::cvtColor(sobel_8UC1, sobel_8UC3, CV_GRAY2RGB);
		Mat saliency_8UC1;
		Mat Saliency_8UC3;
		Mat saliency_32F;
	
		if (g_saliency_method == string("Residual")) {

			//Mat saliency_residual_8UC1;
			Ptr<StaticSaliencySpectralResidual> saliencyAlgorithm = StaticSaliencySpectralResidual::create();
			saliencyAlgorithm->computeSaliency(x_srcImg, saliency_8UC1);
			gradient_Map_C =  saliency_8UC1;
			if (g_Render_method != "Saliency") {
				saliencyAlgorithm->computeSaliency(sobel_8UC3, saliency_8UC1);
				gradient_Map_G = saliency_8UC1;
			}

		}
		else if (g_saliency_method == string("Fine_grained")) {
			//	Mat saliency_finegrained_8UC1;
			Ptr<StaticSaliencyFineGrained> saliencyAlgorithmG = StaticSaliencyFineGrained::create();
			saliencyAlgorithmG->computeSaliency(x_srcImg, saliency_8UC1);
			gradient_Map_C =  saliency_8UC1;
			if (g_Render_method != "Saliency") {
				saliencyAlgorithmG->computeSaliency(sobel_8UC3, saliency_8UC1);
				gradient_Map_G = saliency_8UC1;
			}

		}
		else if (g_saliency_method == string("Blackandwhite")) {

			//Mat saliency_blackandwhite_8UC1;
			saliency_8UC1 = saliency_blackandwhite_main(x_srcImg);
			gradient_Map_C =  saliency_8UC1;
			if (g_Render_method != "Saliency") {
				saliency_8UC1 = saliency_blackandwhite_main(sobel_8UC3);
				gradient_Map_G = saliency_8UC1;
			}
			}
		else if (g_saliency_method == string("Pregraph")) {

			saliency_32F = preGraph_main(x_srcImg);
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_C =  saliency_8UC1;
			if (g_Render_method != "Saliency") {
				saliency_32F = preGraph_main(sobel_8UC3);
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_G = saliency_8UC1;
			}

		}
		else if (g_saliency_method == string("Itti")
			) {
			//Mat saliency_itti_32F;

			saliency_32F = saliency_itti_main(x_srcImg, "C");
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_C =  saliency_8UC1;
			if (g_Render_method != "Saliency") {
				saliency_32F = saliency_itti_main(sobel_8UC3, "G");
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_G = saliency_8UC1;
			}
		}

		else {
			cout << "No Saliency method assigned" << endl;
			return -1;
		}

		debug_image("0_Saliency_C_" + g_saliency_method, gradient_Map_C);
		if (g_Render_method != "Sobel") {
			debug_image("0_Saliency_G_sobel", gradient_Map_G);
			gradient_Union = saliency_union(gradient_Map_C, gradient_Map_G);
			debug_image("0_Saliency_union_gra(sobel)_gra(" + g_saliency_method + ")", gradient_Union);
		}
		_render[RENDER_SOBEL]->add_gradient_map(Gradient_Sobel, sobel_8UC1);

		for(int i=RENDER_SALIENCY;i<RENDER_MAX;i++){
			_render[i]->add_gradient_map(Gradient_Sobel, sobel_8UC1);
			_render[i]->add_gradient_map(Gradient_Saliency_C,gradient_Map_C);
			if (g_Render_method != "Sobel") {
				_render[i]->add_gradient_map(Gradient_Saliency_G, gradient_Map_G);
				_render[i]->add_gradient_map(Gradient_Union, gradient_Union);
				_render[i]->add_gradient_map(Gradient_Twopass, gradient_Union);
			}
		}

	}// if != Sobel

	_render[RENDER_TWOPASS_ATTACH]->add_render(_render[0], _render[1]);
	_render[RENDER_TWOPASS_MERGE]->add_render(_render[0], _render[1]);
	for (int i = 0; i < RENDER_MAX; i++) {
		_render[i]->prepare();
	}

	

		std::thread *p_render[RENDER_MAX];

	p_render[0] = new thread(run_render,_render[RENDER_SOBEL]);

	if (g_Render_method != "Sobel" )
		{
		p_render[RENDER_SALIENCY] = new thread(run_render,_render[RENDER_SALIENCY]);
		p_render[RENDER_UNION] = new thread(run_render,_render[RENDER_UNION]);
	}

	cout << "p_sobel    : " << p_render[RENDER_SOBEL]->get_id() << endl;

	if (g_Render_method != "Sobel") {
		cout << "p_saliency : " << p_render[RENDER_SALIENCY]->get_id() << endl;
	cout << "p_union    : " << p_render[RENDER_UNION]->get_id() << endl;
	}

	
	std::cout << "Number of threads = "+ g_Render_method
		<< std::thread::hardware_concurrency() << std::endl;
	

	p_render[RENDER_SOBEL]->join();
	

	if (g_Render_method != "Sobel" ){
		p_render[RENDER_SALIENCY]->join();
		p_render[RENDER_UNION]->join();
	}
	
	if (g_Render_method == "Twopass") {
		
		p_render[RENDER_TWOPASS_ATTACH] = new thread(run_render, _render[RENDER_TWOPASS_ATTACH]);
		cout << "p_twopass  ATTACH: " << p_render[RENDER_TWOPASS_ATTACH]->get_id() << endl;
		p_render[RENDER_TWOPASS_MERGE] = new thread(run_render, _render[RENDER_TWOPASS_MERGE]);
		cout << "p_twopass MERGE : " << p_render[RENDER_TWOPASS_MERGE]->get_id() << endl;
		std::cout << "Number of threads Twopass = "
			<< std::thread::hardware_concurrency() << std::endl;
		p_render[RENDER_TWOPASS_ATTACH]->join();
		p_render[RENDER_TWOPASS_MERGE]->join();
	}
	
	_render[RENDER_SOBEL]->post_process();
	
	if (g_Render_method != "Sobel"){
			_render[RENDER_SALIENCY]->post_process();
			_render[RENDER_UNION]->post_process();
		}
	if (g_Render_method == "Twopass" ){
			_render[RENDER_TWOPASS_MERGE]->post_process();
			_render[RENDER_TWOPASS_ATTACH]->post_process();
		}
		

	return 0;
}

