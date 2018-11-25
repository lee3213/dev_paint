#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "define.h"

#include "extern.h"
#include "debug_image.h"
#include "brush_pgm.h"
using namespace std;
using namespace cv;
int render_::calc_render_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
	int _render_brush_size[], string tag)
{
	int brush_step;
	int k_depth;
	//int last_depth;
	int _brush_step;
	//basic depth 
	if (render_method > RENDER_SALIENCY) {
		if (render_sobel->render_depth > render_saliency->render_depth) {
			k_depth = render_sobel->render_depth;
		}
		else 	k_depth = render_saliency->render_depth;
	}
	else k_depth = render_depth;
	brush_step = (int)((_BrushMaxSize - _BrushMinSize) / (k_depth+3));

	for (int i = (k_depth - 1); i >= 0; i--)
	{
		_render_brush_size[i] = (int)(_BrushMinSize + (k_depth - i)* brush_step)*g_brush_scale[i];
		if (_render_brush_size[i] >x_canvas_bezel_size)
			_render_brush_size[i] =x_canvas_bezel_size;
		if (_render_brush_size[i] < _BrushMinSize || (render_depth - 1) == i)
			_render_brush_size[i] = _BrushMinSize;
		if (_render_brush_size[i] > stroke_size[i]) {
			_render_brush_size[i] = stroke_size[i] - 1;
		}
		cout << tag + " , " << ", " << "render_brush_size : " + to_string(i) + " , " <<
			_render_brush_size[i] << endl;
		g_file_cstat << g_para_method + "," << g_image_name << ", " << "render_brush_size" + to_string(i) + "," <<
			_render_brush_size[i] << endl;
	}

	x_last_depth = k_depth;

	if (render_method == RENDER_TWOPASS_ENHANCE) {

		/*if (k_depth == 1)
			brush_step = (int)((_BrushMaxSize - _BrushMinSize));
		else
			brush_step = (int)((_BrushMaxSize - _BrushMinSize) / (k_depth - 1));
*/

	int  _B = _render_brush_size[k_depth - 1];
	if (_B > g_BrushAttachSize) {

		if ((render_depth - k_depth) == 0)
			_brush_step = (_B - g_BrushAttachSize);
		else
			_brush_step = (_B - g_BrushAttachSize) / (render_depth - k_depth);
	}
	else _brush_step = 0;

	for (int i = 0; i < (render_depth - k_depth); i++)
	{
		_render_brush_size[i + k_depth] = (int)(_B - (i + 1)* _brush_step);
	}
	}
	_render_brush_size[0] *= 2;
		x_last_depth = render_depth;
		//	r_cout << "_brush_step " << _brush_step << endl;
	
		//g_paint_try_scale[last_depth-1] *= 2;
		//_brush_step = 3;
		//for (int i = (render_depth - 1); i >= 1; i--)
	//	{
	//		_render_brush_size[i ] = x_BrushMinSize + (render_depth- i-1)*_brush_step;
	//	}
	//	_render_brush_size[0] = x_BrushMaxSize ;

	r_cout << tag + " image depth  " << _depth << ", image size: " << g_src_image_width << ",  " << g_src_image_height << endl;
	r_cout << "  _BrushMaxSize " << _BrushMaxSize << ", " <<x_canvas_bezel_size << " ";
	r_cout << "  _BrushMinSize " << _BrushMinSize << ", at size " << g_BrushAttachSize;
	r_cout << "  _BrushAttachSize " << g_BrushAttachSize;
	r_cout << "  _depth  " << _depth;
	r_cout << "  last_depth  " << x_last_depth;
	r_cout << "  brush_step " << brush_step <<
		endl;
	for (int i = 0; i < _depth; i++) {
		r_cout << tag << " : Br size : " << setw(5) << i <<
			setw(5) << g_brush_scale[i] << setw(10) << _render_brush_size[i] << " " <<
			setw(5)<<stroke_size[i]<<
			setw(6) << QT_grid_count[i] << " * " << setw(5)<< x_strk_times[i]<<setw(5) << g_paint_try_scale[i] << " = " <<
			setw(6) << g_paint_try_scale[i] * QT_grid_count[i] << endl;
	}
	//brush_minimum_size = _render_brush_size[x_last_depth - 1];
	
	
	return _depth;
}



void render_::brush_resize(	vector <Brush*> _brush_set)
{
	render_Brush *brush_resized;
	
		int no = 0;
		vector <Brush*>::iterator it = _brush_set.begin();

		for (int j=0; it != _brush_set.end(); it++,j++) {
			for (int i = 0; i < render_depth; i++) {
				if (render_brush_size[i] == 0) continue;
				brush_resized = new (render_Brush);

				resize((*it)->brush_8UC1, brush_resized->brush_8UC1, Size(render_brush_size[i], render_brush_size[i]));
				//resize((*it)->bump, brush_resized->bump, Size(render_brush_size[i], render_brush_size[i]));
			//	resize((*it)->brush_8UC1, brush_resized->brush_8UC1, Size(render_brush_size[i], render_brush_size[i]));
			//	cvtColor(brush_resized->brush, brush_resized->brush_hsv, CV_BGR2HSV);
				brush_resized->brush_thumbnail = (*it)->brush_thumbnail;
				brush_resized->brush_no = j;
			//	resize(brush_resized->brush_thumbnail, brush_resized->brush_thumbnail_minimum,
			//		Size(brush_minimum_size, brush_minimum_size));

				//			brush_resized_set[i].push_back(brush_resized);
				brush_resized->brush_8UC1_data = brush_resized->brush_8UC1.data;
				brush_resized->brush_thumbnail_data = brush_resized->brush_thumbnail.data;
			//	brush_resized->brush_thumbnail_minimum_data = brush_resized->brush_thumbnail_minimum.data;
				brush_resized_array[i][j] = brush_resized;
				Mat th  ;
				cv::threshold(brush_resized->brush_8UC1,th,(double)g_alpha_TH,g_alpha_TH,1);
				int cnz=countNonZero(th);
				if (j == 0) {
					r_cout << i << ", " << j << " = " << setw(7) << cnz << " : " << setw(7) << render_brush_size[i] * render_brush_size[i] <<
						std::fixed << setw(15) <<
						setprecision(2) << (float)((float(cnz) / float(render_brush_size[i] * render_brush_size[i]) )*100.0) << endl;
						debug_image("br2/th_" + to_string(i) + "_" + to_string(j), brush_resized->brush_thumbnail);
						debug_image("br2/inv_th_" + to_string(i) + "_" + to_string(j), th);
				}
#ifdef  DEBUG_BR2
				string x = "br2/_";
				string y = x + m_tag_;
				string z = y + to_string(i) + "_" + to_string(no) + "_" + to_string(render_brush_size[i]);
				debug_image(z, brush_resized->brush_8UC1);
#endif
			}
	}
		int b_max = render_brush_size[0];
		Mat brush_set_image;
		brush_set_image.create(b_max, b_max*render_depth, CV_8UC1);
		brush_set_image.setTo(255);
		
			for (int j = 0; j < g_BrushNumber; j++) {
				for (int i = 0; i < render_depth; i++) {
				int x = i*b_max;
				Rect r(Point(x, 0), Point(x +render_brush_size[i], render_brush_size[i]));
				cv::rectangle(brush_set_image, r,Scalar(0,0,0));
				render_Brush *p_brush;
				p_brush = brush_resized_array[i][j];
				if ( j == 0 )
					debug_image("br2/br" + to_string(j) + "_"+to_string(i), p_brush->brush_8UC1);
				Mat roi=brush_set_image(r);
				p_brush->brush_8UC1.copyTo(roi);
			}
			debug_image("br2/" + to_string(j) + "_", brush_set_image);
		}
}

void render_::brush_pgm_resize(vector <Brush_pgm*> _brush_pgm_list)
{
	render_Brush_pgm *brush_pgm_resized;

	int no = 0;
	vector <Brush_pgm*>::iterator it = _brush_pgm_list.begin();
	Size sz_pgm;
	for (int j = 0; it != _brush_pgm_list.end(); it++, j++) {
		for (int i = 0; i < render_depth; i++) {
			if (render_brush_size[i] == 0) continue;
			brush_pgm_resized = new (render_Brush_pgm);
			sz_pgm.height = render_brush_size[i];
			sz_pgm.width = render_brush_size[i];
			resize((*it)->brush_8UC1, brush_pgm_resized->brush_8UC1,sz_pgm);
			//resize((*it)->bump, brush_resized->bump, Size(render_brush_size[i], render_brush_size[i]));
			//	resize((*it)->brush_8UC1, brush_resized->brush_8UC1, Size(render_brush_size[i], render_brush_size[i]));
			//	cvtColor(brush_resized->brush, brush_resized->brush_hsv, CV_BGR2HSV);
			resize((*it)->brush_8UC1,brush_pgm_resized->brush_thumbnail,Size(g_brush_thumbnail_size,g_brush_thumbnail_size));
			brush_pgm_resized->brush_no = j;
			brush_pgm_resized->brush_size = render_brush_size[i];
			brush_pgm_resized->pgm_size = sz_pgm;
			//resize(brush_pgm_resized->brush_thumbnail, brush_pgm_resized->brush_thumbnail_minimum,
				//Size(brush_minimum_size, brush_minimum_size));

			//			brush_resized_set[i].push_back(brush_resized);
			brush_pgm_resized->brush_8UC1_data = brush_pgm_resized->brush_8UC1.data;
			brush_pgm_resized->brush_thumbnail_data = brush_pgm_resized->brush_thumbnail.data;
		//	brush_pgm_resized->brush_thumbnail_minimum_data = brush_pgm_resized->brush_thumbnail_minimum.data;
			brush_pgm_resized_array[i][j] = brush_pgm_resized;
			Mat th;
			cv::threshold(brush_pgm_resized->brush_8UC1, th, (double)g_alpha_TH, g_alpha_TH, 1);
			int cnz = countNonZero(th);
			if (j == 0) {
				r_cout << i << ", " << j << " = " << setw(7) << cnz << " : " << setw(7) << render_brush_size[i] * render_brush_size[i] <<
					std::fixed << setw(15) <<
					setprecision(2) << (float)((float(cnz) / float(render_brush_size[i] * render_brush_size[i]))*100.0) << endl;

				debug_image("br2/pgm_th_" + to_string(i) + "_" + to_string(j), th);
			}
#ifdef  DEBUG_BR2
			string x = "br2/pgm_";
			string y = x + m_tag_;
			string z = y + to_string(i) + "_" + to_string(no) + "_" + to_string(render_brush_size[i]);
			debug_image(z, brush_resized->brush_8UC1);
#endif
		}
	}
	int b_max = render_brush_size[0];
	Mat brush_set_image;
	brush_set_image.create(b_max, b_max*render_depth, CV_8UC1);
	brush_set_image.setTo(255);

	for (int j = 0; j < g_BrushNumber; j++) {
		for (int i = 0; i < render_depth; i++) {
			int x = i*b_max;
			Rect r(Point(x, 0), Point(x + render_brush_size[i], render_brush_size[i]));
			render_Brush_pgm *p_pgm_brush;
			p_pgm_brush = brush_pgm_resized_array[i][j];
			if (j == 0)
				debug_image("br2/pgm_" + to_string(j) + "_" + to_string(i), p_pgm_brush->brush_8UC1);
			Mat roi = brush_set_image(r);
			p_pgm_brush->brush_8UC1.copyTo(roi);
		}
		debug_image("br2/pgm_" + to_string(j) + "_", brush_set_image);
	}
}