#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "define.h"
#include "render_.h"
#include "extern.h"
#include "debug_image.h"

void render_::brush_resize(	vector <Brush*> _brush_set)
{
	Brush *brush_resized;
	for (int i = 0; i < mm_depth; i++)
	{
		int no = 0;
		vector <Brush*>::iterator it = _brush_set.begin();
		for (; it != _brush_set.end(); it++) {
			brush_resized = new (Brush);
			
			resize((*it)->brush, brush_resized->brush, Size(brush_size[i], brush_size[i]));
			resize((*it)->bump, brush_resized->bump, Size(brush_size[i], brush_size[i]));
			resize((*it)->brush_gray, brush_resized->brush_gray, Size(brush_size[i], brush_size[i]));
		//	cvtColor(brush_resized->brush, brush_resized->brush_hsv, CV_BGR2HSV);
			brush_resized->brush_no = no;
			
			no++;
			brush_resized_set[i].push_back(brush_resized);
#ifdef  DEBUG_BR2
			string x = "br2/_";
			string y = x + m_tag_;
			string z=y+to_string(i)+"_"+to_string(no)+"_"+to_string(brush_size[i]);
			debug_image(z,  brush_resized->brush_gray);
#endif
		}
	}
}