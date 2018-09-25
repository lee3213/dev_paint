#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "define.h"
#include "render_.h"

void render_::brush_resize()
{
	Brush *brush_resized;
	for (int i = 0; i < mm_depth; i++)
	{

		list<Brush>::iterator it = brush_set.begin();
		for (; it != brush_set.end(); it++) {
			brush_resized = new (Brush);
			resize((*it).brush, brush_resized->brush, Size(brush_size[i], brush_size[i]));
			resize((*it).bump, brush_resized->bump, Size(brush_size[i], brush_size[i]));
			resize((*it).brush_gray, brush_resized->brush_gray, Size(brush_size[i], brush_size[i]));
			cvtColor(brush_resized->brush, brush_resized->brush_hsv, CV_BGR2HSV);
			brush_resized_set[i].push_back(*brush_resized);
		}
	}
}