
#include "stdafx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "util.h"
using namespace cv;
using namespace std;

void Normalize(
	const vector<double>&           input,
	const int&                      width,
	const int&                      height,
	vector<double>&                 output,
	const int&                      normrange = 255) {

	double maxval(0);
	double minval(DBL_MAX);
	int i(0);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (maxval < input[i]) maxval = input[i];
			if (minval > input[i]) minval = input[i];
			i++;
		}
	}

	double range = maxval - minval;
	if (0 == range) range = 1;
	//int i(0);
	i = 0;
	output.clear();
	output.resize(width*height);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output[i] = ((normrange*(input[i] - minval)) / range);
			i++;
		}
	}
}

void RGB2LAB2(
	const vector<vector<uint> > &ubuff,
	vector<double>&                 lvec,
	vector<double>&                 avec,
	vector<double>&                 bvec) {

	int sz = int(ubuff.size());
	cout << "sz " << sz << endl;
	lvec.resize(sz);
	avec.resize(sz);
	bvec.resize(sz);

	for (int j = 0; j < sz; j++) {
		int sR = ubuff[j][2];
		int sG = ubuff[j][1];
		int sB = ubuff[j][0];
		//------------------------
		// sRGB to XYZ conversion
		// (D65 illuminant assumption)
		//------------------------
		double R = sR / 255.0;
		double G = sG / 255.0;
		double B = sB / 255.0;

		double r, g, b;

		if (R <= 0.04045)    r = R / 12.92;
		else                r = pow((R + 0.055) / 1.055, 2.4);
		if (G <= 0.04045)    g = G / 12.92;
		else                g = pow((G + 0.055) / 1.055, 2.4);
		if (B <= 0.04045)    b = B / 12.92;
		else                b = pow((B + 0.055) / 1.055, 2.4);

		double X = r*0.4124564 + g*0.3575761 + b*0.1804375;
		double Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
		double Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
		//------------------------
		// XYZ to LAB conversion
		//------------------------
		double epsilon = 0.008856;  //actual CIE standard
		double kappa = 903.3;     //actual CIE standard

		double Xr = 0.950456;   //reference white
		double Yr = 1.0;        //reference white
		double Zr = 1.088754;   //reference white

		double xr = X / Xr;
		double yr = Y / Yr;
		double zr = Z / Zr;

		double fx, fy, fz;
		if (xr > epsilon)    fx = pow(xr, 1.0 / 3.0);
		else                fx = (kappa*xr + 16.0) / 116.0;
		if (yr > epsilon)    fy = pow(yr, 1.0 / 3.0);
		else                fy = (kappa*yr + 16.0) / 116.0;
		if (zr > epsilon)    fz = pow(zr, 1.0 / 3.0);
		else                fz = (kappa*zr + 16.0) / 116.0;

		lvec[j] = 116.0*fy - 16.0;
		avec[j] = 500.0*(fx - fy);
		bvec[j] = 200.0*(fy - fz);
	}
}


void GaussianSmooth(
	const vector<double>&           inputImg,
	const int&                      width,
	const int&                      height,
	const vector<double>&           kernel,
	vector<double>&                 smoothImg) {

	int center = int(kernel.size()) / 2;

	int sz = width*height;
	smoothImg.clear();
	smoothImg.resize(sz);
	vector<double> tempim(sz);
	int rows = height;
	int cols = width;

	int index(0);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			double kernelsum(0);
			double sum(0);
			for (int cc = (-center); cc <= center; cc++) {
				if (((c + cc) >= 0) && ((c + cc) < cols)) {
					sum += inputImg[r*cols + (c + cc)] * kernel[center + cc];
					kernelsum += kernel[center + cc];
				}
			}
			tempim[index] = sum / kernelsum;
			index++;
		}
	}
	//int index = 0;
	index = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			double kernelsum(0);
			double sum(0);
			for (int rr = (-center); rr <= center; rr++) {
				if (((r + rr) >= 0) && ((r + rr) < rows)) {
					sum += tempim[(r + rr)*cols + c] * kernel[center + rr];
					kernelsum += kernel[center + rr];
				}
			}
			smoothImg[index] = sum / kernelsum;
			index++;
		}
	}
}


void GetSaliencyMap(
	const vector<vector<uint> >&inputimg,
	const int&                      width,
	const int&                      height,
	vector<double>&                 salmap,
	const bool&                     normflag) {

	int sz = width*height;
	salmap.clear();
	salmap.resize(sz);

	vector<double> lvec(0), avec(0), bvec(0);
	RGB2LAB2(inputimg, lvec, avec, bvec);

	double avgl(0), avga(0), avgb(0);
	for (int i = 0; i < sz; i++) {
		avgl += lvec[i];
		avga += avec[i];
		avgb += bvec[i];
	}

	avgl /= sz;
	avga /= sz;
	avgb /= sz;

	vector<double> slvec(0), savec(0), sbvec(0);

	vector<double> kernel(0);
	kernel.push_back(1.0);
	kernel.push_back(2.0);
	kernel.push_back(1.0);

	GaussianSmooth(lvec, width, height, kernel, slvec);
	GaussianSmooth(avec, width, height, kernel, savec);
	GaussianSmooth(bvec, width, height, kernel, sbvec);

	for (int i = 0; i < sz; i++) {
		salmap[i] = (slvec[i] - avgl)*(slvec[i] - avgl) +
			(savec[i] - avga)*(savec[i] - avga) +
			(sbvec[i] - avgb)*(sbvec[i] - avgb);
	}


	if (true == normflag) {
		vector<double> normalized(0);
		Normalize(salmap, width, height, normalized);
		swap(salmap, normalized);
	}
}


Mat  saliency_blackandwhite_main(Mat &srcImg) {

	Mat image;
	image=srcImg.clone();

	std::vector<vector<uint>>array(image.cols*image.rows, vector<uint>
		(3, 0));

	for (int y = 0; y<image.rows; y++) {
		for (int x = 0; x<image.cols; x++) {
			Vec3b color = image.at<Vec3b>(Point(x, y));
			array[image.cols*y + x][0] = color[0]; array[image.cols*y + x]
				[1] = color[1]; array[image.cols*y + x][2] = color[2];
		}
	}


	vector<double> salmap; bool normflag = true;

	GetSaliencyMap(array, image.size().width, image.size().height, salmap,
		normflag);

	Mat output;
	output = Mat(image.rows, image.cols, CV_8UC1);
	int k = 0;

	for (int y = 0; y<image.rows; y++) {
		for (int x = 0; x<image.cols; x++) {
			output.at<uchar>(Point(x, y)) = int(salmap[k]);
			k++;
		}
	}
	return output;
}