// SaliencyPainterlyRendering.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include "stdafx.h"
#include "thread"
//#include "SaliencyPainterlyRendering.h"
#include "util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define RUN_THREAD 1
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
render_ *_render[RENDER_MAX];
#include "RenderingImage.h"

#include "json_read.h"
#include "saliency_main.h"
#include "extern.h"
#include "g_setup.h"
#include "draw_grid2.h"

#include "render_.h"
#include "time.h"
#include "cvQuiver.h"


cv::Mat get_canny_map(Mat & srcImg) {
	cv::Mat gray_Map;
	cv::Mat org_Gaussian_Map;
	cv::Mat canny_map;
	//Take GradientImg from source Image
	static int called = 0;
	cv::cvtColor(srcImg, gray_Map, CV_BGR2GRAY);							//3ch -> 1ch
																			//f_path = cv::format("%s/src_1ch.ppm", g_para_method_path.c_str());
																			//cv::imwrite(f_path, gray_Map);
//	debug_image("src_1ch" + to_string(called), gray_Map);
	int gray_map_width = gray_Map.size().width;
	int gray_map_height = gray_Map.size().height;

	//	org_Gaussian_Map = bsShowImage::TakeGaussianBlurImg(gray_Map);

	cv::GaussianBlur(gray_Map, org_Gaussian_Map, Size(3, 3), 0);
//	debug_image("src_take_blur" + to_string(called), org_Gaussian_Map);
	int ddepth = CV_16S;
	//org_sobeldient_Map = bsShowImage::TakeGradient(org_Gaussian_Map);

	cv::Mat grad_x, grad_y, abs_sobeld_x, abs_sobeld_y;

	cv::Canny(org_Gaussian_Map, canny_map, 50, 100, 3);
	debug_image("canny_1.jpg", canny_map);
	mat_print(canny_map, "canny_map");
	//cv::convertScaleAbs(grad_x, abs_sobeld_x);
	//debug_image("canny_2.jpg", abs_sobeld_x);
	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );

	/// Total Gradient (approximate)
	//cv::addWeighted(abs_sobeld_x, 0.5, abs_sobeld_y, 0.5, 0, canny_map);
	debug_image("canny_3.jpg", canny_map);
	called++;
	return canny_map;
}

void func_gradient_orientation(Mat org_8UC1_Map, Mat sobeld_16S_x, Mat sobeld_16S_y,std::string label) {
	int gray_map_width = org_8UC1_Map.size().width;
	int		gray_map_height = org_8UC1_Map.size().height;
	int flowResolution = 1;
	//Mat g_img(gray_map_height, gray_map_width, CV_8UC3);
	Mat g_img[101];
	int mag_histo[101];
	int mag_histogram[101];
	//g_img_solid = org_Gaussian_Map.clone();
	int mag = 100;
	for (int i = 0; i < (mag + 1); i++) {
		mag_histo[i] = 0;
		mag_histogram[i] = 0;
		cv::cvtColor(org_8UC1_Map.clone(), g_img[i], CV_GRAY2BGR);
	}
	//cvtColor(org_Gaussian_Map.clone(), g_img_solid, CV_GRAY2BGR);
	mat_print(g_img, "g_img");
	int mag_max = 0;
	Mat gradient_field = Mat::zeros(gray_map_height, gray_map_width, CV_32SC2);
	for (int i = 0; i < gray_map_height; i++) {
		for (int j = 0; j < gray_map_width; j++) {
			int xval = sobeld_16S_x.at<short>(i, j);
			int yval = sobeld_16S_y.at<short>(i, j);
			gradient_field.at<Point>(i, j) = Point(xval, xval);
			int xy = abs((int)xval) + abs((int)xval);
			if (mag_max < xy) mag_max = xy;
			}
	}
	cout << label << "   Mag MAX= " << mag_max << endl;
	int histo_step = (mag_max / mag)-1;

	for (int i = 0; i < gray_map_height; i++) {
		for (int j = 0; j < gray_map_width; j++) {
			int xval = sobeld_16S_x.at<short>(i, j);
			int yval = sobeld_16S_y.at<short>(i, j);
		
			int xy = abs((int)xval) + abs((int)xval);
			if (xy == 0)
				mag_histo[0]++;
			else {
				int his_loc = (xy / histo_step) + 1;
				if (his_loc < 100)
					mag_histo[his_loc]++;
				else
					mag_histo[100]++;
			}
		}
	}
	int histo_sum = 0;
	int histo_thresh = int ((gray_map_height*gray_map_width- mag_histo[0])*0.1);
	int back;

	for (back = 100; back > 0; back--) {
	//	cout << back << ", " << mag_histo[back] <<
	//		" back*step = " << back*histo_step
	//		<<endl;
		histo_sum += mag_histo[back];
		if (histo_sum > histo_thresh)
			break;
	}
	int mag2 = (back)*histo_step;
	int histo_step2 =  (mag2/100)+1;

	cout << "gray_map_height*gray_map_width = " << gray_map_height*gray_map_width <<
		"gray_map_height*gray_map_width - mag_histo[0] "<<
		gray_map_height*gray_map_width - mag_histo[0]<<
		" histo_thresh= " << histo_thresh << " back = " << back
		<< " histo_step= " << histo_step << endl
		<<" mag_histo[back]= "<<mag_histo[back]
		<<" histo_Step2 = "<<histo_step2
		<<" mag2 ="<<mag2
		<< endl;
	unsigned char * ptr;
	ptr = (unsigned char *)g_img[0].data;
	Mat direction_map;
	direction_map.create(gray_map_height, gray_map_width, CV_8UC3);
	direction_map.setTo(0);
	//debug_image("direction_map_o", direction_map);
	int flow_Resolution_dir = 8;

	unsigned char * ptr_direction = (unsigned char *)direction_map.data;
	Point p2, p2_n;
	Point2f v;
	int length;
	Point C, n;
	for (int i = 0; i < gray_map_height; i += flowResolution) {
		for (int j = 0; j < gray_map_width; j += flowResolution) {
			Point p(j, i);
		//	Point p2(gradient_field.at<Point>(p) + p);

		//	Point dir;
			int dx = (gradient_field.at<Point>(p).x);
			int dy = (gradient_field.at<Point>(p).y);
			int mag_dx = abs(gradient_field.at<Point>(p).x);
			int mag_dy = abs(gradient_field.at<Point>(p).y);
			int xy = mag_dx + mag_dy;

			//direction vector
			//normalize
			float v_mag = (float)sqrt(dx*dx + dy*dy);
			v.x = dx / v_mag; v.y = dy / v_mag;
			if (xy <= histo_step2*(mag - 1)) {
				length = xy / histo_step2 + 2;
			}
			else length = 6;

			//arrow_size = 3;
			if (xy == 0)
			{
				*(ptr + (i*gray_map_width + j) * 3) = 255;
				*(ptr + (i*gray_map_width + j) * 3 + 1) = 0;
				*(ptr + (i*gray_map_width + j) * 3 + 2) = 0;
				//mag_histogram[0]++;
				if (! (i %	flow_Resolution_dir) || !(j %	flow_Resolution_dir)) {
					*(ptr_direction + (i*gray_map_width + j) * 3) = 255;
					*(ptr_direction + (i*gray_map_width + j) * 3 + 1) = 0;
					*(ptr_direction + (i*gray_map_width + j) * 3 + 2) = 0;
				}
			}
			else {
				
				C.x = (int)(v.x * length); C.y = (int)( v.y * length);
				p2 = p + C;
			
				//rotate
				float temp = v.x; v.x = -v.y; v.y = temp;
			
				p2_n.x = p.x + (int)(v.x *6); p2_n.y = p.y + (int)(v.y * 6);

				if (xy >= histo_step2*(mag - 1)) {
					arrowedLine(g_img[mag], p, p2, Scalar(0, 0, 255), 1, 8, 0, 0.1);//thickness=1 linetype=8 shift=0 tiplength=0.1
					arrowedLine(g_img[mag], p, p2_n, Scalar(0, 255, 255), 1, 8, 0, 0.1);
					if (!(i %	flow_Resolution_dir) && !(j %	flow_Resolution_dir)) {
						arrowedLine(direction_map, p, p2, Scalar(0, 0, 255), 1, 8, 0, 0.1);
						arrowedLine(direction_map, p, p2_n, Scalar(0, 255, 255), 1, 8, 0, 0.1);
					}

				}
				else {
					int step = (xy / histo_step2) + 1;
					arrowedLine(g_img[step], p, p2, Scalar(0, 0, 255), 1, 8, 0, 0.1);
					arrowedLine(g_img[step], p, p2_n, Scalar(0, 255, 255), 1, 8, 0, 0.1);
					if (!(i %	flow_Resolution_dir) && !(j %	flow_Resolution_dir)) {
						arrowedLine(direction_map, p, p2, Scalar(0, 0, 255), 1, 8, 0, 0.1);
						arrowedLine(direction_map, p, p2_n, Scalar(0, 255, 255), 1, 8, 0, 0.1);
					}
				}

			}
		}
	}
/*
	for (int i = 0; i < 101; i++) {
		cout << i << " , " << mag_histogram[i] << 
			", "<< i*histo_step2<<" , "<< (i + 1)*histo_step2<< " ";
		
		cv::String str=cv::format("grad_ori\\%s_g_field_%03d_%05d_%05d",label.c_str(),i, i*histo_step2,(i+1)*histo_step2);
	
		cout << str.c_str() << endl;
		std::string s = str.c_str();
		debug_image(s, g_img[i]);
	}
	*/

	debug_image("_direction_map_"+label, direction_map);
}
cv::Mat get_sobel_map(Mat & srcImg) {
	cv::Mat gray_Map;
	cv::Mat org_Gaussian_Map;
	cv::Mat sobel_map;
	cv::Mat sobel_bi_map;
	//Take GradientImg from source Image
	static int called = 0;
	cv::cvtColor(srcImg, gray_Map, CV_BGR2GRAY);							//3ch -> 1ch
																			//f_path = cv::format("%s/src_1ch.ppm", g_para_method_path.c_str());
																			//cv::imwrite(f_path, gray_Map);
	debug_image("src_1ch"+to_string(called), gray_Map);
	int gray_map_width = gray_Map.size().width;
	int gray_map_height = gray_Map.size().height;

	//	org_Gaussian_Map = bsShowImage::TakeGaussianBlurImg(gray_Map);
	cv::Mat bilaterial_Map;
	cv::bilateralFilter(gray_Map, bilaterial_Map, 10, 10, 10);
	debug_image("src_bilaterial_10_10_10", bilaterial_Map);
	cv::GaussianBlur(gray_Map, org_Gaussian_Map, Size(3, 3), 0);
	debug_image("src_take_blur" + to_string(called), org_Gaussian_Map);
	int ddepth = CV_16S;
	double scale = 1.0;
	int delta = 0;
	//org_sobeldient_Map = bsShowImage::TakeGradient(org_Gaussian_Map);

	cv::Mat grad_x, grad_y, abs_sobeld_x, abs_sobeld_y;
	cv::Mat grad_bi_x, grad_bi_y, abs_sobeld_bi_x, abs_sobeld_bi_y;
	//org_Gaussian_Map=
	cv::Sobel(org_Gaussian_Map, grad_x, ddepth,1,0,3, scale, delta);
	cv::convertScaleAbs(grad_x, abs_sobeld_x);
	cv::Sobel(org_Gaussian_Map, grad_y, ddepth,0,1,3,scale,delta);
	cv::convertScaleAbs(grad_y, abs_sobeld_y);
	cv::Sobel(bilaterial_Map, grad_bi_x, ddepth, 1, 0, 3, scale, delta);
	cv::convertScaleAbs(grad_bi_x, abs_sobeld_bi_x);
	cv::Sobel(bilaterial_Map, grad_bi_y, ddepth, 0, 1, 3, scale, delta);
	cv::convertScaleAbs(grad_bi_y, abs_sobeld_bi_y);

	mat_print(grad_x,"grad_x");
	mat_print(abs_sobeld_y, "abs_sobeld_y");/*
	func_gradient_orientation(org_Gaussian_Map, grad_x,grad_y,"gausian");
	func_gradient_orientation(bilaterial_Map, grad_bi_x, grad_bi_y,"bilaterial");
	*/
	/*
	Mat gradient_vector(gray_map_height, gray_map_width, CV_8UC1);
	gradient_vector.setTo(0);
	char buf[100]; 
	int k = 0;
	for(int y=0;y<gray_map_height;y++)
		for (int x = 0; x < gray_map_width; x++) {
		//	if (!(x % 10) && !(y % 10)) {
			if ( !(k%37)){
				int u =(int)( grad_x.at<uchar>(y, x));
				int v = (int)(grad_y.at<uchar>(y, x));
				if (u != 0 && v != 0) {
					cvQuiver(gradient_vector, x, y, u, v
						,//int u, int v,
						cvScalar(255, 255, 255),
						1,//int Size,
						1//int Thickness
					);
					//sprintf_s(buf, "gradient_vector_%3d_%3d.jpg", 1, 1);
					//debug_image(buf, gradient_vector);

				}
			
			}
			k++;
		}
	debug_image("gradient_vector.jpg", gradient_vector);
	*/
	/// Total Gradient (approximate)

	cv::addWeighted(abs_sobeld_x, 0.5, abs_sobeld_y, 0.5, 0, sobel_map);
	cv::addWeighted(abs_sobeld_bi_x, 0.5, abs_sobeld_bi_y, 0.5, 0, sobel_bi_map);
	debug_image("sobel_bilaterial", sobel_bi_map);
	debug_image("sobel_gaussian", sobel_map);
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
void gradientXY(Mat &x1i, Mat &y1i, Mat &mag1u)
{
	const int H = x1i.rows, W = x1i.cols;
	mag1u.create(H, W, CV_8U);
	for (int r = 0; r < H; r++)
	{
		const int *x = x1i.ptr<int>(r), *y = y1i.ptr<int>(r);
		BYTE* m = mag1u.ptr<BYTE>(r);
		for (int c = 0; c < W; c++)
			m[c] = (BYTE)min(x[c] + y[c], 255);   //((int)sqrt(sqr(x[c]) + sqr(y[c])), 255);
	}
}

void gradientGray(Mat &bgr3u, Mat &mag1u)
{
	Mat g1u;
	cv::cvtColor(bgr3u, g1u, COLOR_BGR2GRAY);
	const int H = g1u.rows, W = g1u.cols;
	Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);

	// Left/right most column Ix
	for (int y = 0; y < H; y++)
	{
		Ix.at<int>(y, 0) = abs(g1u.at<BYTE>(y, 1) - g1u.at<BYTE>(y, 0)) * 2;
		Ix.at<int>(y, W - 1) = abs(g1u.at<BYTE>(y, W - 1) - g1u.at<BYTE>(y, W - 2)) * 2;
	}

	// Top/bottom most column Iy
	for (int x = 0; x < W; x++)
	{
		Iy.at<int>(0, x) = abs(g1u.at<BYTE>(1, x) - g1u.at<BYTE>(0, x)) * 2;
		Iy.at<int>(H - 1, x) = abs(g1u.at<BYTE>(H - 1, x) - g1u.at<BYTE>(H - 2, x)) * 2;
	}

	// Find the gradient for inner regions
	for (int y = 0; y < H; y++)
		for (int x = 1; x < W - 1; x++)
			Ix.at<int>(y, x) = abs(g1u.at<BYTE>(y, x + 1) - g1u.at<BYTE>(y, x - 1));
	for (int y = 1; y < H - 1; y++)
		for (int x = 0; x < W; x++)
			Iy.at<int>(y, x) = abs(g1u.at<BYTE>(y + 1, x) - g1u.at<BYTE>(y - 1, x));

	gradientXY(Ix, Iy, mag1u);
}

static inline int bgrMaxDist( const Vec3b &u, const Vec3b &v )
{
int b = abs( u[0] - v[0] ), g = abs( u[1] - v[1] ), r = abs( u[2] - v[2] );
b = max( b, g );
return max( b, r );
}
static inline int vecDist3b( const Vec3b &u, const Vec3b &v )
{
return abs( u[0] - v[0] ) + abs( u[1] - v[1] ) + abs( u[2] - v[2] );
}
void gradientRGB(Mat &bgr3u, Mat &mag1u)
{
	const int H = bgr3u.rows, W = bgr3u.cols;
	Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);

	// Left/right most column Ix
	for (int y = 0; y < H; y++)
	{
		Ix.at<int>(y, 0) = bgrMaxDist(bgr3u.at<Vec3b>(y, 1), bgr3u.at<Vec3b>(y, 0)) * 2;
		Ix.at<int>(y, W - 1) = bgrMaxDist(bgr3u.at<Vec3b>(y, W - 1), bgr3u.at<Vec3b>(y, W - 2)) * 2;
	}

	// Top/bottom most column Iy
	for (int x = 0; x < W; x++)
	{
		Iy.at<int>(0, x) = bgrMaxDist(bgr3u.at<Vec3b>(1, x), bgr3u.at<Vec3b>(0, x)) * 2;
		Iy.at<int>(H - 1, x) = bgrMaxDist(bgr3u.at<Vec3b>(H - 1, x), bgr3u.at<Vec3b>(H - 2, x)) * 2;
	}

	// Find the gradient for inner regions
	for (int y = 0; y < H; y++)
	{
		const Vec3b *dataP = bgr3u.ptr<Vec3b>(y);
		for (int x = 2; x < W; x++)
			Ix.at<int>(y, x - 1) = bgrMaxDist(dataP[x - 2], dataP[x]);  //  bgr3u.at<Vec3b>(y, x+1), bgr3u.at<Vec3b>(y, x-1));
	}
	for (int y = 1; y < H - 1; y++)
	{
		const Vec3b *tP = bgr3u.ptr<Vec3b>(y - 1);
		const Vec3b *bP = bgr3u.ptr<Vec3b>(y + 1);
		for (int x = 0; x < W; x++)
			Iy.at<int>(y, x) = bgrMaxDist(tP[x], bP[x]);
	}
	gradientXY(Ix, Iy, mag1u);
}

void gradientHSV(Mat &bgr3u, Mat &mag1u)
{
	Mat hsv3u;
	cvtColor(bgr3u, hsv3u, COLOR_BGR2HSV);
	const int H = hsv3u.rows, W = hsv3u.cols;
	Mat Ix(H, W, CV_32S), Iy(H, W, CV_32S);

	// Left/right most column Ix
	for (int y = 0; y < H; y++)
	{
		Ix.at<int>(y, 0) = vecDist3b(hsv3u.at<Vec3b>(y, 1), hsv3u.at<Vec3b>(y, 0));
		Ix.at<int>(y, W - 1) = vecDist3b(hsv3u.at<Vec3b>(y, W - 1), hsv3u.at<Vec3b>(y, W - 2));
	}

	// Top/bottom most column Iy
	for (int x = 0; x < W; x++)
	{
		Iy.at<int>(0, x) = vecDist3b(hsv3u.at<Vec3b>(1, x), hsv3u.at<Vec3b>(0, x));
		Iy.at<int>(H - 1, x) = vecDist3b(hsv3u.at<Vec3b>(H - 1, x), hsv3u.at<Vec3b>(H - 2, x));
	}

	// Find the gradient for inner regions
	for (int y = 0; y < H; y++)
		for (int x = 1; x < W - 1; x++)
			Ix.at<int>(y, x) = vecDist3b(hsv3u.at<Vec3b>(y, x + 1), hsv3u.at<Vec3b>(y, x - 1)) / 2;
	for (int y = 1; y < H - 1; y++)
		for (int x = 0; x < W; x++)
			Iy.at<int>(y, x) = vecDist3b(hsv3u.at<Vec3b>(y + 1, x), hsv3u.at<Vec3b>(y - 1, x)) / 2;

	gradientXY(Ix, Iy, mag1u);
}






void run_render(render_ * _render) {
	for (int j = 0; j < _render->mm_depth; j++)
		if (_render->mm_aStroke_set[j].size() == 0) {
			cout << _render->m_tag << " stroke_size==0" << endl;
			return;
		}

	_render->success_or_fail = _render->PainterlyRendering();
//	image_save(g_image_name + _render->m_tag_, _render->result_image, _render->grid_map_1c[MAX_DEPTH]);
	}

int   RenderingImage(char * src_name, char * deploy_name)
{
	time_t s_time, e_time;
	tm t_s, t_e;
	time(&s_time);
	localtime_s(&t_s, &s_time);

	char s_buff[20];
	strftime(s_buff, 20, "%Y-%m-%d %H:%M:%S", &t_s);
	char e_buff[20];
	//time_t now = time(NULL);

	//	cv::Mat org_saliency_sobeld_Map;
		//, org_QuadTreeMap, org_DensityMap, org_rstImg;

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
	/* ROI test
	Mat clone = x_srcImg.clone();
	Point Point_LT, Point_RB;
	Point_LT.x = 0;
	Point_LT.y = 0;
	Point_RB.x = 100;
	Point_RB.y = 100;
	Rect roi = Rect(Point_LT, Point_RB);
	Size s(Point_RB);
	Mat mask(s, CV_8UC1);
	mask.setTo(255);
	Mat ROI = clone(roi);
	Mat sobel_ROI= get_sobel_map(ROI);
	debug_image("ROI.jpg", ROI);
	debug_image("ROI_sobel.jpg", sobel_ROI);
	debug_image("src_ROI.jpg", clone);
	Mat a;
	cvtColor(sobel_ROI, a, CV_GRAY2BGR);
	//a.copyTo(ROI,mask);
	a.copyTo(ROI);
	debug_image("copyTo_ROI.jpg", clone);
	return -12345;
	*/
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
		_render[i] = new  render_(i, x_srcImg);
	}

	//Brush initialization


	//g_saliency_method;//without, itti, pregraph, blackandwhite,resudual,fine_sobelined


	cout << "Saliency Method " << g_saliency_method << endl;
	//	cout << "g_Render_method " << g_Render_method << endl;
	time_t s_s_time, s_e_time;
	tm s_t_s, s_t_e;
	time(&s_s_time);
	localtime_s(&s_t_s, &s_s_time);

	char s_s_buff[20];
	strftime(s_s_buff, 20, "%Y-%m-%d %H:%M:%S", &s_t_s);
	char s_e_buff[20];

	sobel_8UC1 = get_sobel_map(x_srcImg);
	if (sobel_8UC1.cols == 0 ||
		sobel_8UC1.rows == 0)
	{
		cout << "unexpected sobel_gradient_Map = 0" << endl;
		return -20002;
	}

	debug_image("0_sobel_gradient", sobel_8UC1);
#ifdef CANNY
	Mat canny_map = get_canny_map(x_srcImg);
#endif

	cv::cvtColor(sobel_8UC1, sobel_8UC3, CV_GRAY2RGB);

	Mat saliency_8UC1;

	Mat saliency_32F;

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
	sa = saliencyAlgorithm_O->computeSaliency(x_srcImg, saliencyMap);

	int ndet = int(saliencyMap.size());//int(sa_size);
	int maxd = 7, step = 255 / maxd, jitter = 9; // jitter to seperate single rects
	Mat draw = x_srcImg.clone();
	for (int i = 0; i < std::min(maxd, ndet); i++) {
		Vec4i bb = saliencyMap[i];
		Scalar col = Scalar(((i*step) % 255), 50, 255 - ((i*step) % 255));
		Point off(theRNG().uniform(-jitter, jitter), theRNG().uniform(-jitter, jitter));
		rectangle(draw, Point(bb[0] + off.x, bb[1] + off.y), Point(bb[2] + off.x, bb[3] + off.y), col, 2);
		rectangle(draw, Rect(20, 20 + i * 10, 10, 10), col, -1); // mini temperature scale
		debug_image("BING_" + to_string(i), draw);
	}

	Mat mag1u, mag1u_rgb, mag1u_hsv;
	gradientGray(x_srcImg, mag1u);
	debug_image("BING_g_Gray", mag1u);
	gradientRGB(x_srcImg, mag1u_rgb);
	debug_image("BING_g_RGB", mag1u_rgb);
	gradientHSV(x_srcImg, mag1u_hsv);
	debug_image("BING_g_HSV", mag1u_hsv);
	*/

#endif		
		if (g_saliency_method == string("Sobel")) {
		}
		else if (g_saliency_method == string("Residual")) {

			//Mat saliency_residual_8UC1;
			Ptr<StaticSaliencySpectralResidual> saliencyAlgorithm = StaticSaliencySpectralResidual::create();
			saliencyAlgorithm->computeSaliency(x_srcImg, saliency_8UC1);
			//	mat_print(saliency_8UC1, "Residual");
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliencyAlgorithm->computeSaliency(sobel_8UC3, saliency_8UC1);
			gradient_Map_G = saliency_8UC1.clone();
			//	}

		}
		else if (g_saliency_method == string("Fine_grained")) {
			//	Mat saliency_finegrained_8UC1;
			Ptr<StaticSaliencyFineGrained> saliencyAlgorithmG = StaticSaliencyFineGrained::create();
			saliencyAlgorithmG->computeSaliency(x_srcImg, saliency_8UC1);
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliencyAlgorithmG->computeSaliency(sobel_8UC3, saliency_8UC1);
			gradient_Map_G = saliency_8UC1.clone();
			//	}

		}
		else if (g_saliency_method == string("Blackandwhite")) {

			//Mat saliency_blackandwhite_8UC1;
			saliency_8UC1 = saliency_blackandwhite_main(x_srcImg);
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliency_8UC1 = saliency_blackandwhite_main(sobel_8UC3);
			gradient_Map_G = saliency_8UC1.clone();
			//	}
		}
		else if (g_saliency_method == string("Pregraph")) {

			saliency_32F = preGraph_main(x_srcImg);
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliency_32F = preGraph_main(sobel_8UC3);
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_G = saliency_8UC1.clone();
			//	}

		}
		else if (g_saliency_method == string("Itti")
			) {
			//Mat saliency_itti_32F;

			saliency_32F = saliency_itti_main(x_srcImg, "C");
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliency_32F = saliency_itti_main(sobel_8UC3, "G");
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_G = saliency_8UC1.clone();
			//	}
		}
		else if (g_saliency_method == string("Perazzi")) {

			saliency_32F = saliency_Perazzi_main(x_srcImg);
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_C = saliency_8UC1.clone();
			//	if (g_Render_method != "Saliency") {
			saliency_32F = saliency_Perazzi_main(sobel_8UC3);
			saliency_32F.convertTo(saliency_8UC1, CV_8UC1, 255.);
			gradient_Map_G = saliency_8UC1.clone();
			//	}

		}

		else {
			cout << "No Saliency method assigned" << endl;
			return -5555;
		}
		_render[RENDER_SOBEL]->add_gradient_map(Gradient_Sobel, sobel_8UC1);
		if (g_saliency_method != string("Sobel")) {

			debug_image("0_Saliency_C_" + g_saliency_method, gradient_Map_C);
			//if (g_Render_method != "Sobel") {
			debug_image("0_Saliency_G_sobel", gradient_Map_G);
			gradient_Union = saliency_union(gradient_Map_C, gradient_Map_G);
			debug_image("0_Saliency_union_gra(sobel)_gra(" + g_saliency_method + ")", gradient_Union);
			//}


			for (int i = RENDER_SALIENCY; i < RENDER_MAX; i++) {
				_render[i]->add_gradient_map(Gradient_Sobel, sobel_8UC1);
				_render[i]->add_gradient_map(Gradient_Saliency_C, gradient_Map_C);
				
				_render[i]->add_gradient_map(Gradient_Saliency_G, gradient_Map_G);
				_render[i]->add_gradient_map(Gradient_Union, gradient_Union);
				_render[i]->add_gradient_map(Gradient_Twopass, gradient_Union);
				//	}
			}
		}// if != Sobel
		time(&s_e_time);
		localtime_s(&s_t_e, &s_e_time);
		strftime(s_e_buff, 20, "%Y-%m-%d %H:%M:%S", &s_t_e);
		cout << "saliency finished " << s_s_buff << " : " << s_e_buff << endl;
		clog << "saliency finished " << s_s_buff << " , " << s_e_buff << endl;
		int how_many_render;
		if (g_saliency_method == string("Sobel")) {
			how_many_render = 1;
		}
		else {
			how_many_render = RENDER_MAX;
			_render[RENDER_TWOPASS_ATTACH]->add_render(_render[0], _render[2]);//render_sobel, render_union
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
			for (int j = 0; j < _render[i]->mm_depth; j++){
				cout << _render[i]->m_tag << " : "
					<<_render[i]->mm_aStroke_set[j].size() << endl;
				tot += (int)_render[i]->mm_aStroke_set[j].size();
			}
			cout << _render[i]->mm_depth << "  " << tot << endl;
			strftime(p_s_buff, 20, "%Y-%m-%d %H:%M:%S", &p_t_s[i]);
			strftime(p_e_buff, 20, "%Y-%m-%d %H:%M:%S", &p_t_e[i]);
			cout << " Prepare Finished " << _render[i]->m_tag << " " << p_s_buff << " : " << p_e_buff << endl;
			clog << " Prepare Finished, " << _render[i]->m_tag << " " << p_s_buff << ", " << p_e_buff << endl;
		}

		cout <<" ++++++++++++++++++++++++++++++++++++++++" << endl;
		//return 997788;

#ifdef RUN_THREAD	
		


		std::thread *p_render[RENDER_MAX];

		p_render[0] = new thread(run_render, _render[RENDER_SOBEL]);

	
	cout << "p_sobel  t_id : " << p_render[RENDER_SOBEL]->get_id() << endl;
	
	
	std::cout << "Number of threads = " 
		<< std::thread::hardware_concurrency() << std::endl;
	


		
		if (g_saliency_method != string("Sobel")) {
			p_render[RENDER_SALIENCY] = new thread(run_render, _render[RENDER_SALIENCY]);
			p_render[RENDER_UNION] = new thread(run_render, _render[RENDER_UNION]);
			
			p_render[RENDER_TWOPASS_MERGE] = new thread(run_render, _render[RENDER_TWOPASS_MERGE]);
		
			p_render[RENDER_TWOPASS_ATTACH] = new thread(run_render, _render[RENDER_TWOPASS_ATTACH]);
			
			cout << "p_saliency t_id: " << p_render[RENDER_SALIENCY]->get_id() << endl;
			cout << "p_union    t_id: " << p_render[RENDER_UNION]->get_id() << endl;
			cout << "p_twopass MERGE t_id: " << p_render[RENDER_TWOPASS_MERGE]->get_id() << endl;
			cout << "p_twopass  ATTACH t_id: " << p_render[RENDER_TWOPASS_ATTACH]->get_id() << endl;
			std::cout << "Number of threads Twopass = "
				<< std::thread::hardware_concurrency() << std::endl;
			p_render[RENDER_SOBEL]->join();
			p_render[RENDER_SALIENCY]->join();
			p_render[RENDER_UNION]->join();
			p_render[RENDER_TWOPASS_MERGE]->join();
			p_render[RENDER_TWOPASS_ATTACH]->join();
		
			_render[RENDER_SALIENCY]->post_process();
			_render[RENDER_UNION]->post_process();
			_render[RENDER_TWOPASS_MERGE]->post_process();
			_render[RENDER_TWOPASS_ATTACH]->post_process();
		}
		_render[RENDER_SOBEL]->post_process();
#else

		std::thread *p_render[RENDER_MAX];

		p_render[0] = new thread(run_render, _render[RENDER_SOBEL]);
		p_render[RENDER_SOBEL]->join();
		_render[RENDER_SOBEL]->post_process();
		if (g_saliency_method != string("Sobel")) {
			p_render[RENDER_SALIENCY] = new thread(run_render, _render[RENDER_SALIENCY]);
			p_render[RENDER_SALIENCY]->join();
			p_render[RENDER_UNION] = new thread(run_render, _render[RENDER_UNION]);
			p_render[RENDER_UNION]->join();
			p_render[RENDER_TWOPASS_MERGE] = new thread(run_render, _render[RENDER_TWOPASS_MERGE]);
			p_render[RENDER_TWOPASS_MERGE]->join();
			p_render[RENDER_TWOPASS_ATTACH] = new thread(run_render, _render[RENDER_TWOPASS_ATTACH]);
			p_render[RENDER_TWOPASS_ATTACH]->join();
				
			

			_render[RENDER_SALIENCY]->post_process();
			_render[RENDER_UNION]->post_process();
			_render[RENDER_TWOPASS_MERGE]->post_process();
			_render[RENDER_TWOPASS_ATTACH]->post_process();
		}
		
#endif
	time(&e_time);
	localtime_s(&t_e, &e_time);
	strftime(e_buff, 20, "%Y-%m-%d %H:%M:%S", &t_e);
	cout <<  "R end, " << s_buff << " : " << e_buff << endl;
	clog << "E end, " << s_buff << ", " << e_buff << endl;
	return 0;
}

