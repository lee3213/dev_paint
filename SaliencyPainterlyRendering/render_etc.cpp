
#include "stdafx.h"

#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "render_.h"
void render_::proof_box(Point &s, int i_width, int i_height) {
	Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << s.y << endl;

}

void render_::proof_box(Point &s, int i_width, int i_height, char * p) {
	//Rect rr;
	if (s.x < 0)
		r_cout << "error 1:" << p << s.x << endl;
	if (s.x >= i_width)
		r_cout << "error 2:" << p << s.x << endl;



	if (s.y < 0) r_cout << "error 3:" << p << s.y << endl;

	if (s.y >= i_height)
		r_cout << "error 4:" << p << s.y << endl;

}
