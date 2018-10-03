/*
    Copyright (c) 2012, Philipp Krähenbühl
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Stanford University nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Philipp Krähenbühl ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Philipp Krähenbühl BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "saliency_perazzi.h"
#include <cstdio>
#include "util.h"
Mat saliency_Perazzi_main( Mat &im ) {
	//if (argc < 2) {
		//printf("Usage: %s image\n", argv[0] );
	//	return -1;
//	}
//	Mat im = imread( argv[1] );
	Saliency saliency;

	Mat_<float> sal = saliency.saliency( im );
	
	double adaptive_T = 2.0 * sum( sal )[0] / (sal.cols*sal.rows);
	while (sum( sal > adaptive_T )[0] == 0)
		adaptive_T /= 1.2;
//	char fname[256];

	Mat uc3;
	std::string ty;

//	ty = type2str(uc3.type());
//	printf("uc3: %s %dx%d \n", ty.c_str(), uc3.cols, uc3.rows);
	//ty = type2str(im.type());
//	printf("im: %s %dx%d \n", ty.c_str(), im.cols, im.rows);
	//ty = type2str(sal.type());
	//printf("sal: %s %dx%d \n", ty.c_str(), sal.cols, sal.rows);

	//sprintf_s
	//(fname, "im_%s.ppm", argv[1]);
	//imwrite(fname,im);

//	imshow( "im", im );
//	sprintf_s(fname, "sal_%s.jpg", argv[1]);
//	sal.convertTo(uc3, CV_8UC3,256);
//	ty = type2str(uc3.type());
//	printf_s("uc3: %s %dx%d \n", ty.c_str(), uc3.cols, uc3.rows);
//	imwrite(fname,uc3);
//	imshow( "sal", sal );
//	sprintf_s(fname, "T_%s.jpg", argv[1]);
//	imwrite(fname,sal > adaptive_T);

	//imshow( "T", sal > adaptive_T );

//	waitKey();
	
	return sal;
}
