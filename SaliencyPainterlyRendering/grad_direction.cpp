
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

static inline int bgrMaxDist(const Vec3b &u, const Vec3b &v)
{
	int b = abs(u[0] - v[0]), g = abs(u[1] - v[1]), r = abs(u[2] - v[2]);
	b = max(b, g);
	return max(b, r);
}
static inline int vecDist3b(const Vec3b &u, const Vec3b &v)
{
	return abs(u[0] - v[0]) + abs(u[1] - v[1]) + abs(u[2] - v[2]);
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
	//Scharr( src_8UC1, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );

	/// Total Gradient (approximate)
	//cv::addWeighted(abs_sobeld_x, 0.5, abs_sobeld_y, 0.5, 0, canny_map);
	debug_image("canny_3.jpg", canny_map);
	called++;
	return canny_map;
}

void func_gradient_orientation(Mat org_8UC1_Map, Mat sobeld_16S_x, Mat sobeld_16S_y, std::string label) {
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
	int histo_step = (mag_max / mag) - 1;

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
	int histo_thresh = int((gray_map_height*gray_map_width - mag_histo[0])*0.1);
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
	int histo_step2 = (mag2 / 100) + 1;

	cout << "gray_map_height*gray_map_width = " << gray_map_height*gray_map_width <<
		"gray_map_height*gray_map_width - mag_histo[0] " <<
		gray_map_height*gray_map_width - mag_histo[0] <<
		" histo_thresh= " << histo_thresh << " back = " << back
		<< " histo_step= " << histo_step << endl
		<< " mag_histo[back]= " << mag_histo[back]
		<< " histo_Step2 = " << histo_step2
		<< " mag2 =" << mag2
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
				if (!(i %	flow_Resolution_dir) || !(j %	flow_Resolution_dir)) {
					*(ptr_direction + (i*gray_map_width + j) * 3) = 255;
					*(ptr_direction + (i*gray_map_width + j) * 3 + 1) = 0;
					*(ptr_direction + (i*gray_map_width + j) * 3 + 2) = 0;
				}
			}
			else {

				C.x = (int)(v.x * length); C.y = (int)(v.y * length);
				p2 = p + C;

				//rotate
				float temp = v.x; v.x = -v.y; v.y = temp;

				p2_n.x = p.x + (int)(v.x * 6); p2_n.y = p.y + (int)(v.y * 6);

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

	debug_image("_direction_map_" + label, direction_map);
}

/*
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
