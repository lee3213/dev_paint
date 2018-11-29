#include "stdafx.h"
#include "debug_define.h"
#include "brush_pgm.h"
#include "extern.h"
#include "dir.h"
int brush_pgm() {


	string render_stencil_path("\\render\\stencil");
	check_and_create(render_stencil_path,false);

	string f_name;
	int nth=0;
	Mat centered_pgm;
	Brush_pgm *__brush;
	int bsize_width, bsize_height;
	
	for (; nth < g_BrushNumber; nth++) {
		f_name = format("/render/stencil/stencil_%d.pgm", nth + 1);
	/*	vector<int> compression_params;
		compression_params.push_back(CV_IMWRITE_PXM_BINARY);
		compression_params.push_back(1);
		cv::imwrite(f_name, centered_pgm, compression_params);// 1CH
	*/	
		Mat mask_8UC1 = imread(f_name.c_str(), IMREAD_GRAYSCALE);
		bsize_width = mask_8UC1.size().width;
		bsize_height = mask_8UC1.size().height;
		if (bsize_width == 0 || bsize_height == 0)
		{
			cout << f_name << endl;
			exit(-9997);
		}
		__brush = new Brush_pgm;
		__brush->brush_8UC1 = mask_8UC1.clone(); \
			__brush->brush_8UC1_Size.width = bsize_width;
		__brush->brush_8UC1_Size.height = bsize_height;
		__brush->brush_no = nth;
		g__brush_pgm_list.push_back(__brush);
	}
	int ncnt=0;
	vector <Brush_pgm *>::iterator it = g__brush_pgm_list.begin();
	for (; it != g__brush_pgm_list.end();it++) {
		Mat th;
		Brush_pgm *_b = (*it);
		cv::threshold(_b->brush_8UC1, th, (double)g_alpha_TH, g_alpha_TH, 1);
		int cnz = countNonZero(th);
		
			cout <<"pgm brush "<< ncnt << ", "  " = " << setw(7) << cnz << " : " << setw(7) << _b->brush_8UC1_Size.height * _b->brush_8UC1_Size.width <<
				std::fixed << setw(15) <<
				setprecision(2) << (float)(float(cnz) / (float(_b->brush_8UC1_Size.width)*(float)(_b->brush_8UC1_Size.height))*100.0) << endl;
			ncnt++;
	}
	cout << "pgm brush count = "<<ncnt<<endl;
	return ncnt;
}