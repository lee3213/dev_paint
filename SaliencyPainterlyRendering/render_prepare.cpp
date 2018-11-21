
#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/saliency.hpp>
using namespace std;
using namespace cv;
using namespace saliency;
#include "define.h"
#include "util.h"

#include "debug_image.h"
#include "use_json.h"
#include "set_result_path.h"

// 유일한 응용 프로그램 개체입니다.
#include "util.h"
#include "time_stamp.h"

#include "json_read.h"
#include "saliency_main.h"
#include "g_setup.h"


#include "dir.h"


int  saliency_union(Mat saliency_map_C, Mat saliency_map_G, Mat & _union_map) {


	unsigned char * saliency_map_C_data = saliency_map_C.data;
	unsigned char * saliency_map_G_data = saliency_map_G.data;
	int width = saliency_map_C.cols;
	int height = saliency_map_C.rows;

	unsigned char * union_map_data = _union_map.data;
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

	return width*height;
}

cv::Mat get_sobel_map(Mat & srcImg) {
	//cv::Mat gray_Map;
	cv::Mat org_Gaussian_Map;
	cv::Mat sobel_map;
//	cv::Mat sobel_bi_map;
	//Take GradientImg from source Image
	static int called = 0;

	cv::cvtColor(srcImg, g_src_gray_Map, CV_BGR2GRAY);							//3ch -> 1ch
																			//f_path = cv::format("%s/src_1ch.ppm", g_para_method_path.c_str());
																			//cv::imwrite(f_path, gray_Map);
	debug_image("src_1ch" + to_string(called), g_src_gray_Map);
	int gray_map_width = g_src_gray_Map.size().width;
	int gray_map_height = g_src_gray_Map.size().height;

	//	org_Gaussian_Map = bsShowImage::TakeGaussianBlurImg(gray_Map);
	//cv::Mat bilaterial_Map;
	//cv::bilateralFilter(gray_Map, bilaterial_Map, 10, 10, 10);
	//debug_image("src_bilaterial_10_10_10", bilaterial_Map);
	cv::GaussianBlur(g_src_gray_Map, org_Gaussian_Map, Size(3, 3), 0);
	debug_image("src_take_blur" + to_string(called), org_Gaussian_Map);
	int ddepth = CV_16S;
	double scale = 1.0;
	int delta = 0;
	//org_sobeldient_Map = bsShowImage::TakeGradient(org_Gaussian_Map);

	cv::Mat grad_x, grad_y, abs_sobeld_x, abs_sobeld_y;
	cv::Mat grad_bi_x, grad_bi_y, abs_sobeld_bi_x, abs_sobeld_bi_y;
	//org_Gaussian_Map=
	cv::Sobel(org_Gaussian_Map, grad_x, ddepth, 1, 0, 3, scale, delta);
	cv::convertScaleAbs(grad_x, abs_sobeld_x);
	cv::Sobel(org_Gaussian_Map, grad_y, ddepth, 0, 1, 3, scale, delta);
	cv::convertScaleAbs(grad_y, abs_sobeld_y);

	//cv::Sobel(bilaterial_Map, grad_bi_x, ddepth, 1, 0, 3, scale, delta);
	//cv::convertScaleAbs(grad_bi_x, abs_sobeld_bi_x);
//	cv::Sobel(bilaterial_Map, grad_bi_y, ddepth, 0, 1, 3, scale, delta);
//	cv::convertScaleAbs(grad_bi_y, abs_sobeld_bi_y);

	mat_print(grad_x, "grad_x");
	mat_print(abs_sobeld_y, "abs_sobeld_y");
	
	cv::addWeighted(abs_sobeld_x, 0.5, abs_sobeld_y, 0.5, 0, sobel_map);
	//cv::addWeighted(abs_sobeld_bi_x, 0.5, abs_sobeld_bi_y, 0.5, 0, sobel_bi_map);
	//debug_image("sobel_bilaterial", sobel_bi_map);
	debug_image("sobel_gaussian", sobel_map);
	return sobel_map;
}


int   prepare_Rendering(char * src_name, char * deploy_name)
{
	//time_t now = time(NULL);

	//	cv::Mat org_saliency_sobeld_Map;
	//, org_QuadTreeMap, org_DensityMap, org_rstImg;

	

	Mat sobel_8UC1;
	Mat sobel_8UC3;
	Mat gradient_Union_8UC1;
	Mat gradient_Map_C_8UC3;
	Mat gradient_Map_G_8UC3;
	string fname_saliency_G;
	string fname_saliency_C;
	string f_saliency_img;

	Mat saliency_8UC1;
	Mat saliency_32F;

	int saved_depth = -1;


	int ret = set_global(string(src_name), string(deploy_name));
	if (ret < 0)
		return -7777;

	ret = BrushInitialization(g_brush_vector_set);
	if (ret != 0) {
		cout << "Brush Initialization Error_" << endl;
		cerr << "Brush Initialization Error_" << endl;
		return -1001;
	}
	
	g_srcImg_RO = cv::imread(src_name, CV_LOAD_IMAGE_COLOR);

	if (g_srcImg_RO.cols == 0
		|| g_srcImg_RO.rows == 0)
	{
		cout << "Src " << src_name << " size 0" << endl;
		return -100;
	}

	g_src_image_width = g_srcImg_RO.size().width;
	g_src_image_height = g_srcImg_RO.size().height;
	g_src_image_channels = g_srcImg_RO.channels();
	g_src_image_step1 = (int)g_srcImg_RO.step1();

	g_file_cstat << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;
	cout << g_para_method + "," << g_image_name << ", " << "image size," <<
		to_string(g_src_image_width) + "," + to_string(g_src_image_height) << endl;

	gradient_Union_8UC1.create(g_src_image_height, g_src_image_width, CV_8UC1);
	debug_image("src_image", g_srcImg_RO);

	


	cout << "Saliency Method " << g_saliency_method << endl;
	//	cout << "g_Render_method " << g_Render_method << endl;


	sobel_8UC1 = get_sobel_map(g_srcImg_RO);
	if (sobel_8UC1.cols == 0 ||
		sobel_8UC1.rows == 0)
	{
		cout << "unexpected sobel_gradient_Map = 0" << endl;
		return -20002;
	}

	debug_image("0_sobel_gradient", sobel_8UC1);

#ifdef CANNY
	Mat canny_map = get_canny_map(g_srcImg_RO);
#endif

	cv::cvtColor(sobel_8UC1, sobel_8UC3, CV_GRAY2RGB);


#ifdef BING
	Mat sa;
	//	if (g_saliency_method == string("ObjectnessBING")) {

	Mat saliency_Objectness_C, saliency_Objectness_G;
	string training_path = "/render/ObjectnessTrainedModel";
	//Mat saliency_residual_8UC1;
	Ptr<ObjectnessBING> saliencyAlgorithm_O = ObjectnessBING::create();
	//saliencyAlgorithm = ObjectnessBING::create();
	vector<Vec4i> saliencyMap;
	saliencyAlgorithm_O.dynamicCast<ObjectnessBING>()->setTrainingPath(training_path);
	saliencyAlgorithm_O.dynamicCast<ObjectnessBING>()->setBBResDir("/rst/Results");
	sa = saliencyAlgorithm_O->computeSaliency(g_srcImg_RO, saliencyMap);

	int ndet = int(saliencyMap.size());//int(sa_size);
	int maxd = 7, step = 255 / maxd, jitter = 9; // jitter to seperate single rects
	Mat draw = g_srcImg_RO.clone();
	for (int i = 0; i < std::min(maxd, ndet); i++) {
		Vec4i bb = saliencyMap[i];
		Scalar col = Scalar(((i*step) % 255), 50, 255 - ((i*step) % 255));
		Point off(theRNG().uniform(-jitter, jitter), theRNG().uniform(-jitter, jitter));
		rectangle(draw, Point(bb[0] + off.x, bb[1] + off.y), Point(bb[2] + off.x, bb[3] + off.y), col, 2);
		rectangle(draw, Rect(20, 20 + i * 10, 10, 10), col, -1); // mini temperature scale
		debug_image("BING_" + to_string(i), draw);
	}

	Mat mag1u, mag1u_rgb, mag1u_hsv;
	gradientGray(g_srcImg_RO, mag1u);
	debug_image("BING_g_Gray", mag1u);
	gradientRGB(g_srcImg_RO, mag1u_rgb);
	debug_image("BING_g_RGB", mag1u_rgb);
	gradientHSV(g_srcImg_RO, mag1u_hsv);
	debug_image("BING_g_HSV", mag1u_hsv);
	*/

#endif		


		f_saliency_img = g_root_saliency_path + "/" + g_image_name + "_" + g_saliency_method;
	fname_saliency_G = f_saliency_img + "_G_8UC3.jpg";
	fname_saliency_C = f_saliency_img + "_C_8UC3.jpg";

	if (g_saliency_method != SALIENCY_STR_SOBEL) {

		bool exists;
		int flag = 0;

		exists = fileExists(fname_saliency_C);
		if (exists == true) {
			flag += 1;

			gradient_Map_C_8UC3 = cv::imread(fname_saliency_C, CV_LOAD_IMAGE_COLOR);
			cvtColor(gradient_Map_C_8UC3, gradient_Map_C_8UC1, CV_BGR2GRAY);
			exists = fileExists(fname_saliency_G);
			if (exists == true) {
				flag += 1;
				gradient_Map_G_8UC3 = cv::imread(fname_saliency_G, CV_LOAD_IMAGE_COLOR);
				cvtColor(gradient_Map_G_8UC3, gradient_Map_G_8UC1, CV_BGR2GRAY);
				//gradient_Map_G_8UC3.convertTo(gradient_Map_G_8UC1, CV_8UC1);
			}
		}

		if (flag == 0) {
			if (g_saliency_method == string("Residual")) {

				//Mat saliency_residual_8UC1;
				Ptr<StaticSaliencySpectralResidual> saliencyAlgorithm = StaticSaliencySpectralResidual::create();
				saliencyAlgorithm->computeSaliency(g_srcImg_RO, saliency_8UC1);
				//	mat_print(saliency_8UC1, "Residual");
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliencyAlgorithm->computeSaliency(sobel_8UC3, saliency_8UC1);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}

			}
			else if (g_saliency_method == string("Fine_grained")) {
				//	Mat saliency_finegrained_8UC1;
				Ptr<StaticSaliencyFineGrained> saliencyAlgorithmG = StaticSaliencyFineGrained::create();
				saliencyAlgorithmG->computeSaliency(g_srcImg_RO, saliency_8UC1);
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliencyAlgorithmG->computeSaliency(sobel_8UC3, saliency_8UC1);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}

			}
			else if (g_saliency_method == string("Blackandwhite")) {

				//Mat saliency_blackandwhite_8UC1;
				saliency_8UC1 = saliency_blackandwhite_main(g_srcImg_RO);
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliency_8UC1 = saliency_blackandwhite_main(sobel_8UC3);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}
			}
			else if (g_saliency_method == string("Pregraph")) {

				saliency_32F = preGraph_main(g_srcImg_RO);
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliency_32F = preGraph_main(sobel_8UC3);
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}

			}
			else if (g_saliency_method == string("Itti")) {
				//Mat saliency_itti_32F;

				saliency_32F = saliency_itti_main(g_srcImg_RO.clone(), "C");
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliency_32F = saliency_itti_main(sobel_8UC3.clone(), "G");
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}
			}
			else if (g_saliency_method == string("Perazzi")) {

				saliency_32F = saliency_Perazzi_main(g_srcImg_RO);
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_C_8UC1 = saliency_8UC1.clone();
				//	if (g_Render_method != "Saliency") {
				saliency_32F = saliency_Perazzi_main(sobel_8UC3);
				saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
				gradient_Map_G_8UC1 = saliency_8UC1.clone();
				//	}

			}

			else {
				cout << "No Saliency method assigned" << endl;
				return -5555;
			}
			vector<int> compression_params;
			compression_params.push_back(CV_IMWRITE_PXM_BINARY);
			compression_params.push_back(0);

			cvtColor(gradient_Map_C_8UC1, gradient_Map_C_8UC3, CV_GRAY2BGR);
			cvtColor(gradient_Map_G_8UC1, gradient_Map_G_8UC3, CV_GRAY2BGR);
			//	gradient_Map_C_8UC1.convertTo(gradient_Map_C_8UC3, CV_8UC3);
			//	gradient_Map_G_8UC1.convertTo(gradient_Map_G_8UC3, CV_8UC3);
			//cv::imwrite(saliency_C, gradient_Map_C_8UC3, compression_params);
			//cv::imwrite(saliency_G, gradient_Map_G_8UC3, compression_params);
			cv::imwrite(fname_saliency_C, gradient_Map_C_8UC3);
			cv::imwrite(fname_saliency_G, gradient_Map_G_8UC3);
		}
		else {//flag != 0 
			if (flag != 2)
				return -1;

		}
	}// if sobel

	for (int i = 0; i < RENDER_MAX; i++) {//SOBEL SALIENCY UNION TWO_MERGE TWO_ENHANCE
		_render[i] = new  render_(i, g_srcImg_RO);

		if (g_saliency_method == g_saliency_method_str[i])
		{
			g_saliency_method = i;
		}
	}
		_render[RENDER_SOBEL]->add_gradient_map(Gradient_Sobel, sobel_8UC1);
		
	if (g_saliency_method != SALIENCY_STR_SOBEL) {

		debug_image("0_Saliency_C_" + g_saliency_method, gradient_Map_C_8UC1);
		//if (g_Render_method != "Sobel") {
		debug_image("0_Saliency_G_" + g_saliency_method
			, gradient_Map_G_8UC1);

		int size_union = saliency_union(gradient_Map_C_8UC1, gradient_Map_G_8UC1, gradient_Union_8UC1);
		debug_image("0_Saliency_union_" + g_saliency_method, gradient_Union_8UC1);

		//}


		for (int i = RENDER_SALIENCY; i < RENDER_MAX; i++) {
			_render[i]->add_gradient_map(Gradient_Sobel, sobel_8UC1);
			_render[i]->add_gradient_map(Gradient_Saliency_C, gradient_Map_C_8UC1);

			_render[i]->add_gradient_map(Gradient_Saliency_G, gradient_Map_G_8UC1);
			_render[i]->add_gradient_map(Gradient_Union, gradient_Union_8UC1);
			_render[i]->add_gradient_map(Gradient_Twopass, gradient_Union_8UC1);
			//	}
		}
	}// if != Sobel

	//t1.time_stamping(cout, "saliency_finished", TIME_STAMP_END);
	int how_many_render;
	if (g_saliency_method == SALIENCY_STR_SOBEL) {
		how_many_render = 1;
	}
	else {
		how_many_render = RENDER_MAX;
		_render[RENDER_UNION]->add_render(_render[0], _render[2]);
		_render[RENDER_TWOPASS_ENHANCE]->add_render(_render[0], _render[2]);//render_sobel, render_union
		_render[RENDER_TWOPASS_MERGE]->add_render(_render[0], _render[2]);
	}
	time_t p_s_time[RENDER_MAX], p_e_time[RENDER_MAX];
	tm p_t_s[RENDER_MAX], p_t_e[RENDER_MAX];
	char p_s_buff[100];
	char p_e_buff[100];

	for (int i = 0; i < how_many_render; i++) {
		time(&p_s_time[i]);
		localtime_s(&p_t_s[i], &p_s_time[i]);

		_render[i]->prepare();

		time(&p_e_time[i]);
		localtime_s(&p_t_e[i], &p_e_time[i]);
	}
	for (int i = 0; i < how_many_render; i++) {
		cout << "++++++++++++++++++ prepare " + _render[i]->m_tag +
			" ++++++++++++++++++++++++++++++++++++++++" << endl;
		int tot = 0;
		for (int j = 0; j < _render[i]->render_depth; j++) {
			cout << _render[i]->m_tag << " : "
				<< _render[i]->render_Stroke_set[j].stroke_list.size() << endl;
			tot += (int)_render[i]->render_Stroke_set[j].stroke_list.size();
		}
		cout << _render[i]->render_depth << "  " << tot << endl;
		strftime(p_s_buff, 20, "%Y-%m-%d %H:%M:%S", &p_t_s[i]);
		strftime(p_e_buff, 20, "%Y-%m-%d %H:%M:%S", &p_t_e[i]);
		cout << " Prepare Finished " << _render[i]->m_tag << " " << p_s_buff << " : " << p_e_buff << endl;
		g_file_clog << " Prepare Finished, " << _render[i]->m_tag << " " << p_s_buff << ", " << p_e_buff << endl;
	}

	cout << " ++++++++++++++++++++++++++++++++++++++++" << endl;
	return 0;
}