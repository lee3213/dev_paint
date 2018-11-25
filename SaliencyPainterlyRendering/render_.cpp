
#include "stdafx.h"
#include <list>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include "define.h"
#include "brush.h"
#include "QuadTree.h"
#include "debug_image.h"
#include "brush_pgm.h"
#include "render_.h"
void  render_::p_poke_canvas(unsigned char * p, int p_x, int p_y, int p_0, int p_1, int p_2) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 3;
	p[index] = p_0;
	p[index + 1] = p_1;
	p[index + 2] = p_2;
}
 void  render_::p_poke_canvas_1c(unsigned char * p, int p_x, int p_y, int p_0) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 1;
	p[index] = p_0;

}
void  render_::p_peek_canvas(unsigned char * p, int p_x, int p_y, int &p_0, int &p_1, int &p_2) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width) * 3;

	p_0 = p[index];
	p_1 = p[index + 1];
	p_2 = p[index + 2];
}
void  render_::p_peek_canvas_1c(unsigned char * p, int p_x, int p_y, int &p_0) {
	int index = ((p_x + x_canvas_bezel_size) + (p_y + x_canvas_bezel_size) * x_canvas_size_width);

	p_0 = p[index];

}