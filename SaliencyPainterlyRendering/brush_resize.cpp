#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "define.h"

//#include "extern.h"
#include "debug_image.h"
using namespace std;
using namespace cv;
int render_::calc_brush_size(int _BrushMaxSize, int _BrushMinSize, int  & _depth,
	int _brush_size[], string tag)
{
	int brush_step;
	int k_depth;
	int last_depth;
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
		_brush_size[i] = (int)(_BrushMinSize + (k_depth - i)* brush_step)*g_brush_scale[i];
		if (_brush_size[i] > canvas_bezel_size)
			_brush_size[i] = canvas_bezel_size;
		if (_brush_size[i] < _BrushMinSize || (render_depth - 1) == i)
			_brush_size[i] = _BrushMinSize;
		if (_brush_size[i] > stroke_size[i]) {
			_brush_size[i] = stroke_size[i] - 1;
		}
		cout << tag + " , " << ", " << "brush_size : " + to_string(i) + " , " <<
			_brush_size[i] << endl;
		g_file_cstat << g_para_method + "," << g_image_name << ", " << "brush_size" + to_string(i) + "," <<
			_brush_size[i] << endl;
	}

	last_depth = k_depth;

	if (render_method == RENDER_TWOPASS_ATTACH) {

		/*if (k_depth == 1)
			brush_step = (int)((_BrushMaxSize - _BrushMinSize));
		else
			brush_step = (int)((_BrushMaxSize - _BrushMinSize) / (k_depth - 1));
*/	int _brush_step;

	int  _B = _brush_size[k_depth - 1];
	if (_B > g_BrushAttachSize) {

		if ((render_depth - k_depth) == 0)
			_brush_step = (_B - g_BrushAttachSize);
		else
			_brush_step = (_B - g_BrushAttachSize) / (render_depth - k_depth);
	}
	else _brush_step = 0;

	for (int i = 0; i < (render_depth - k_depth); i++)
	{
		_brush_size[i + k_depth] = (int)(_B - (i + 1)* _brush_step);
	}
	}
		last_depth = render_depth;
		//	r_cout << "_brush_step " << _brush_step << endl;
	
		g_paint_try_scale[last_depth-1] *= 2;
	


	r_cout << tag + " image depth  " << _depth << ", image size: " << g_src_image_width << ",  " << g_src_image_height << endl;
	r_cout << "  _BrushMaxSize " << _BrushMaxSize << ", " << canvas_bezel_size << " ";
	r_cout << "  _BrushMinSize " << _BrushMinSize << ", at size " << g_BrushAttachSize;
	r_cout << "  _BrushAttachSize " << g_BrushAttachSize;
	r_cout << "  _depth  " << _depth;
	r_cout << "  last_depth  " << last_depth;
	r_cout << "  brush_step " << brush_step <<
		endl;
	for (int i = 0; i < _depth; i++) {
		r_cout << tag << " : Br size : " << setw(5) << i <<
			setw(5) << g_brush_scale[i] << setw(10) << _brush_size[i] << " " <<
			setw(5)<<stroke_size[i]<<
			setw(6) << QT_grid_count[i] << " * " << setw(5) << g_paint_try_scale[i] << " = " <<
			setw(6) << g_paint_try_scale[i] * QT_grid_count[i] << endl;
	}
	brush_minimum_size = _brush_size[last_depth - 1];

	return _depth;
}



void render_::brush_resize(	vector <Brush*> _brush_set)
{
	render_Brush *brush_resized;
	
		int no = 0;
		vector <Brush*>::iterator it = _brush_set.begin();

		for (int j=0; it != _brush_set.end(); it++,j++) {
			for (int i = 0; i < render_depth; i++) {
				if (brush_size[i] == 0) continue;
				brush_resized = new (render_Brush);

				resize((*it)->brush_gray, brush_resized->brush_8UC1, Size(brush_size[i], brush_size[i]));
				//resize((*it)->bump, brush_resized->bump, Size(brush_size[i], brush_size[i]));
			//	resize((*it)->brush_gray, brush_resized->brush_gray, Size(brush_size[i], brush_size[i]));
			//	cvtColor(brush_resized->brush, brush_resized->brush_hsv, CV_BGR2HSV);
				brush_resized->brush_thumbnail = (*it)->brush_thumbnail;
				brush_resized->brush_no = j;
				resize(brush_resized->brush_thumbnail, brush_resized->brush_thumbnail_minimum,
					Size(brush_minimum_size, brush_minimum_size));

				//			brush_resized_set[i].push_back(brush_resized);
				brush_resized->brush_8UC1_data = brush_resized->brush_8UC1.data;
				brush_resized->brush_thumbnail_data = brush_resized->brush_thumbnail.data;
				brush_resized->brush_thumbnail_minimum_data = brush_resized->brush_thumbnail_minimum.data;
				brush_resized_array[i][j] = brush_resized;
#ifdef  DEBUG_BR2
				string x = "br2/_";
				string y = x + m_tag_;
				string z = y + to_string(i) + "_" + to_string(no) + "_" + to_string(brush_size[i]);
				debug_image(z, brush_resized->brush_gray);
#endif
			}
	}
}