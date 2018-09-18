
#include "stdafx.h"
#include "rgb2hsv.h"
/*******************************************************************************/

void RGB2HSV(Scalar  RGB, Scalar &HSV) {
	unsigned char min, max, delta;

	if (RGB.val[_R]<RGB.val[_G])min = RGB.val[_R];
	else min = RGB.val[_G];
	if (RGB.val[_B]<min) min = RGB.val[_B];

	if (RGB.val[_R]>RGB.val[_G])max = RGB.val[_R];
	else max = RGB.val[_G];
	if (RGB.val[_B]>max)max = RGB.val[_B];

	HSV.val[_V] = max;                // v, 0..255

	delta = max - min;                      // 0..255, < v

	if (max != 0)
		HSV.val[_S] = (int)(delta) * 255 / max;        // s, 0..255
	else {
		// r = g = b = 0        // s = 0, v is undefined
		HSV.val[_S] = 0;
		HSV.val[_H] = 0;
		return;
	}

	if (RGB.val[_R] == max)
		HSV.val[_H] = (RGB.val[_G] - RGB.val[_B]) * 60 / delta;        // between yellow & magenta
	else if (RGB.val[_G] == max)
		HSV.val[_H] = 120 + (RGB.val[_B] - RGB.val[_R]) * 60 / delta;    // between cyan & yellow
	else
		HSV.val[_H] = 240 + (RGB.val[_R] - RGB.val[_G]) * 60 / delta;    // between magenta & cyan

	if (HSV.val[_H] < 0)
		HSV.val[_H] += 360;
}
void RGB2HSV(struct _RGB_set RGB, struct _HSV_set *HSV) {
	unsigned char min, max, delta;

	if (RGB.r<RGB.g)min = RGB.r; else min = RGB.g;
	if (RGB.b<min)min = RGB.b;

	if (RGB.r>RGB.g)max = RGB.r; else max = RGB.g;
	if (RGB.b>max)max = RGB.b;

	HSV->v = max;                // v, 0..255

	delta = max - min;                      // 0..255, < v

	if (max != 0)
		HSV->s = (int)(delta) * 255 / max;        // s, 0..255
	else {
		// r = g = b = 0        // s = 0, v is undefined
		HSV->s = 0;
		HSV->h = 0;
		return;
	}

	if (RGB.r == max)
		HSV->h = (RGB.g - RGB.b) * 60 / delta;        // between yellow & magenta
	else if (RGB.g == max)
		HSV->h = 120 + (RGB.b - RGB.r) * 60 / delta;    // between cyan & yellow
	else
		HSV->h = 240 + (RGB.r - RGB.g) * 60 / delta;    // between magenta & cyan

	if (HSV->h < 0)
		HSV->h += 360;
}

/*******************************************************************************
* Function HSV2RGB
* Description: Converts an HSV color value into its equivalen in the RGB color space.
* Copyright 2010 by George Ruinelli
* The code I used as a source is from http://www.cs.rit.edu/~ncs/color/t_convert.html
* Parameters:
*   1. struct with HSV color (source)
*   2. pointer to struct RGB color (target)
* Notes:
*   - r, g, b values are from 0..255
*   - h = [0,360], s = [0,255], v = [0,255]
*   - NB: if s == 0, then h = 0 (undefined)
******************************************************************************/
void HSV2RGB(struct _HSV_set HSV, struct _RGB_set *RGB) {
	int i;
	float f, p, q, t, h, s, v;

	h = (float)HSV.h;
	s = (float)HSV.s;
	v = (float)HSV.v;

	s /= 255;

	if (s == 0) { // achromatic (grey)
		RGB->r = RGB->g = RGB->b = v;
		return;
	}

	h /= 60;            // sector 0 to 5
	i = floor(h);
	f = h - i;            // factorial part of h
	p = (unsigned char)(v * (1 - s));
	q = (unsigned char)(v * (1 - s * f));
	t = (unsigned char)(v * (1 - s * (1 - f)));

	switch (i) {
	case 0:
		RGB->r = v;
		RGB->g = t;
		RGB->b = p;
		break;
	case 1:
		RGB->r = q;
		RGB->g = v;
		RGB->b = p;
		break;
	case 2:
		RGB->r = p;
		RGB->g = v;
		RGB->b = t;
		break;
	case 3:
		RGB->r = p;
		RGB->g = q;
		RGB->b = v;
		break;
	case 4:
		RGB->r = t;
		RGB->g = p;
		RGB->b = v;
		break;
	default:        // case 5:
		RGB->r = v;
		RGB->g = p;
		RGB->b = q;
		break;
	}
}

void HSV2RGB(Scalar HSV, Scalar & RGB) {
	int i;
	float f, p, q, t, h, s, v;

	h = (float)HSV.val[_H];
	s = (float)HSV.val[_S];
	v = (float)HSV.val[_V];

	s /= 255;

	if (s == 0) { // achromatic (grey)
		RGB.val[_R] = RGB.val[_G] = RGB.val[_B] = v;
		return;
	}

	h /= 60;            // sector 0 to 5
	i = floor(h);
	f = h - i;            // factorial part of h
	p = (unsigned char)(v * (1 - s));
	q = (unsigned char)(v * (1 - s * f));
	t = (unsigned char)(v * (1 - s * (1 - f)));

	switch (i) {
	case 0:
		RGB.val[_R] = v;
		RGB.val[_G] = t;
		RGB.val[_B] = p;
		break;
	case 1:
		RGB.val[_R] = q;
		RGB.val[_G] = v;
		RGB.val[_B] = p;
		break;
	case 2:
		RGB.val[_R] = p;
		RGB.val[_G] = v;
		RGB.val[_B] = t;
		break;
	case 3:
		RGB.val[_R] = p;
		RGB.val[_G] = q;
		RGB.val[_B] = v;
		break;
	case 4:
		RGB.val[_R] = t;
		RGB.val[_G] = p;
		RGB.val[_B] = v;
		break;
	default:        // case 5:
		RGB.val[_R] = v;
		RGB.val[_G] = p;
		RGB.val[_B] = q;
		break;
	}
}

