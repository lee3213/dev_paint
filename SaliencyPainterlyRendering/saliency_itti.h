#include "stdafx.h"

#ifndef _SALIENCY_MAP_H_
#define _SALIENCY_MAP_H_

//*************************
// Include Files
//*************************

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
using namespace cv;

//*************************
// Constant & Definitions
//*************************

static const float WEIGHT_INTENSITY = 0.30f;
static const float WEIGHT_COLOR = 0.30f;
static const float WEIGHT_ORIENTATION = 0.20f;
static const float WEIGHT_MOTION = 0.20f;
static const float RANGEMAX = 255.00f;
static const float SCALE_GAUSS_PYRAMID = (float)1.7782794100389228012254211951927; // = 100^0.125
static const float DEFAULT_STEP_LOCAL = 8.f;

//*************************
// Class Definition
//*************************

class SMFeatureWeights { // weights for fundamental features
public:
	SMFeatureWeights() {
		SetWeights(0.0,0.0,0.0,0.0);
	};
  SMFeatureWeights(float _w_intensity, float _w_color, float _w_orient, float _w_motion) {
	  SetWeights(_w_intensity, _w_color,  _w_orient,  _w_motion);
  };
  ~SMFeatureWeights() {
  };
  int SetWeights(float _w_intensity, float _w_color, float _w_orient, float _w_motion) {
	  w_intensity=_w_intensity;
	  w_color = _w_color;
	  w_orient = _w_orient;
	  w_motion = _w_motion;
	  return 0;
  };
  int GetWeights(float &_w_intensity, float &_w_color, float &_w_orient, float &_w_motion)
  {
	  _w_intensity = w_intensity;
	  _w_color = w_color;
	  _w_orient = w_orient;
	  _w_motion = w_motion;
	  return 0;
  };
  
protected:
  float w_intensity, w_color, w_orient, w_motion;
};

class SMParams: public SMFeatureWeights { // parameters necessary to construct saliency maps
public:
	SMParams() {
		SetWeights(0.0, 0.0, 0.0, 0.0);
		outputScale = 0.0;

	};
  SMParams(float _w_intensity, float _w_color, float _w_orient, float _w_motion, float param, float scale) {
	  SetWeights(_w_intensity, _w_color, _w_orient, _w_motion);
	  outputScale = scale;
	 // param;
  };
  ~SMParams() {
  };
  int SetFrameSize(cv::Size _frame_size);
  int SetOutputScale(float scale);
  cv::Size GetFrameSize();
  float GetRetinalDecayParam();
  float GetOutputScale();

protected:
  cv::Size frame_size;
  float outputScale; // resolution parameter of the saliency map (large value = low resolution)
};

class SaliencyMap
{
public:
  // Constructors and descructors
  SaliencyMap(int height, int width);
//  ~ParallelSaliencyMap(void);
   ~SaliencyMap(void);
// Core: saliency map generation
  cv::Mat SMGetSM(Mat & src,string _tag);
  // Interfaces
  //cv::Mat* SMGetSMFromVideoFrame(CvCapture * input_video, IplImage * &inputFrame_cur, int frameNo);
 // cv::Mat* SMGetSMFromVideoFrameWebcam(videoInput &vi, int dev_id, IplImage * &inputFrame_cur);
  // Parameters
  SMParams smParams;
  string tag;
private:
  Mat  R,  G,  B,  I;
  Mat  prev_frame;

  Mat  GaborKernel0;
  Mat  GaborKernel45;
  Mat  GaborKernel90;
  Mat  GaborKernel135;
  
private:
  // splsaliencyng color channels
  void SMExtractRGBI(Mat  inputImage, Mat  &R, Mat  &G, Mat  &B, Mat  &I);
  // extracting feature maps
  void IFMGetFM(Mat & src, Mat  dst[6]);
  void CFMGetFM(Mat & R, Mat  &G, Mat  &B, Mat  RGFM[6], Mat  BYFM[6]);
  void OFMGetFM(Mat & I, Mat  dst[24]);
  void MFMGetFM(Mat & I, Mat  dst_x[6], Mat  dst_y[6]);
  // normalization
  void normalizeFeatureMaps(Mat  FM[6], Mat  NFM[6], int width, int height, int num_maps);
  Mat SMNormalization(cv::Mat & src); // Itti normalization
  Mat SMRangeNormalize(cv::Mat & src); // dynamic range normalization
  // extracting conspicuity maps
  Mat  ICMGetCM(cv::Mat IFM[6], cv::Size size);
  Mat  CCMGetCM(cv::Mat CFM_RG[6], cv::Mat CFM_BY[6], cv::Size size);
  Mat  OCMGetCM(cv::Mat OFM[24], cv::Size size);
  Mat  MCMGetCM(cv::Mat MFM_X[6], cv::Mat MFM_Y[6], cv::Size size);
  void FMGaussianPyrCSD(Mat & src, Mat dst[6], char *p);
  void SaliencyMap::FMCreateGaussianPyr(Mat& src, Mat dst[9], char *p);
  void SaliencyMap::FMCenterSurroundDiff(Mat  GaussianMap[9], Mat dst[6]);
};

#endif /* _SALIENCY_MAP_H_ */

static const double GaborKernel_0[9][9] = {

{1.85212E-06, 1.28181E-05, -0.000350433, -0.000136537, 0.002010422, -0.000136537, -0.000350433, 1.28181E-05, 1.85212E-06},
{2.80209E-05, 0.000193926, -0.005301717, -0.002065674, 0.030415784, -0.002065674, -0.005301717, 0.000193926, 2.80209E-05},
{0.000195076, 0.001350077, -0.036909595, -0.014380852, 0.211749204, -0.014380852, -0.036909595, 0.001350077, 0.000195076},
{0.00062494, 0.004325061, -0.118242318, -0.046070008, 0.678352526, -0.046070008, -0.118242318, 0.004325061, 0.00062494},
{0.000921261, 0.006375831, -0.174308068, -0.067914552, 1, -0.067914552, -0.174308068, 0.006375831, 0.000921261},
{0.00062494, 0.004325061, -0.118242318, -0.046070008, 0.678352526, -0.046070008, -0.118242318, 0.004325061, 0.00062494},
{0.000195076, 0.001350077, -0.036909595, -0.014380852, 0.211749204, -0.014380852, -0.036909595, 0.001350077, 0.000195076},
{2.80209E-05, 0.000193926, -0.005301717, -0.002065674, 0.030415784, -0.002065674, -0.005301717, 0.000193926, 2.80209E-05},
{1.85212E-06, 1.28181E-05, -0.000350433, -0.000136537, 0.002010422, -0.000136537, -0.000350433, 1.28181E-05, 1.85212E-06}

};
static const double GaborKernel_45[9][9] = {

{4.0418E-06, 2.2532E-05, -0.000279806, -0.001028923, 3.79931E-05, 0.000744712, 0.000132863, -9.04408E-06, -1.01551E-06},
{2.2532E-05, 0.00092512, 0.002373205, -0.013561362, -0.0229477, 0.000389916, 0.003516954 , 0.000288732, -9.04408E-06},
{-0.000279806, 0.002373205, 0.044837725, 0.052928748, -0.139178011, -0.108372072, 0.000847346 , 0.003516954, 0.000132863},
{-0.001028923, -0.013561362, 0.052928748, 0.46016215, 0.249959607, -0.302454279, -0.108372072, 0.000389916, 0.000744712},
{3.79931E-05, -0.0229477, -0.139178011, 0.249959607, 1, 0.249959607, -0.139178011, -0.0229477, 3.79931E-05},
{0.000744712, 0.000389916, -0.108372072, -0.302454279, 0.249959607, 0.46016215, 0.052928748, -0.013561362, -0.001028923},
{0.000132863, 0.003516954, 0.000847346, -0.108372072, -0.139178011, 0.052928748, 0.044837725, 0.002373205, -0.000279806},
{-9.04408E-06, 0.000288732, 0.003516954, 0.000389916, -0.0229477, -0.013561362, 0.002373205, 0.00092512, 2.2532E-05},
{-1.01551E-06, -9.04408E-06, 0.000132863, 0.000744712, 3.79931E-05, -0.001028923, -0.000279806, 2.2532E-05, 4.0418E-06}

};
static const double GaborKernel_90[9][9] = {

{1.85212E-06, 2.80209E-05, 0.000195076, 0.00062494, 0.000921261, 0.00062494, 0.000195076, 2.80209E-05, 1.85212E-06},
{1.28181E-05, 0.000193926, 0.001350077, 0.004325061, 0.006375831, 0.004325061, 0.001350077, 0.000193926, 1.28181E-05},
{-0.000350433, -0.005301717, -0.036909595, -0.118242318, -0.174308068, -0.118242318, -0.036909595, -0.005301717, -0.000350433},
{-0.000136537, -0.002065674, -0.014380852, -0.046070008, -0.067914552, -0.046070008, -0.014380852, -0.002065674, -0.000136537},
{0.002010422, 0.030415784, 0.211749204, 0.678352526, 1, 0.678352526, 0.211749204, 0.030415784, 0.002010422},
{-0.000136537, -0.002065674, -0.014380852, -0.046070008, -0.067914552, -0.046070008, -0.014380852, -0.002065674, -0.000136537},
{-0.000350433, -0.005301717, -0.036909595, -0.118242318, -0.174308068, -0.118242318, -0.036909595, -0.005301717, -0.000350433},
{1.28181E-05, 0.000193926, 0.001350077, 0.004325061, 0.006375831, 0.004325061, 0.001350077, 0.000193926, 1.28181E-05},
{1.85212E-06, 2.80209E-05, 0.000195076, 0.00062494, 0.000921261, 0.00062494, 0.000195076, 2.80209E-05, 1.85212E-06}

};
static const double GaborKernel_135[9][9] = {

{-1.01551E-06, -9.04408E-06, 0.000132863, 0.000744712, 3.79931E-05, -0.001028923, -0.000279806, 2.2532E-05, 4.0418E-06},
{-9.04408E-06, 0.000288732, 0.003516954, 0.000389916, -0.0229477, -0.013561362, 0.002373205, 0.00092512, 2.2532E-05},
{0.000132863, 0.003516954, 0.000847346, -0.108372072, -0.139178011, 0.052928748, 0.044837725, 0.002373205, -0.000279806},
{0.000744712, 0.000389916, -0.108372072, -0.302454279, 0.249959607, 0.46016215, 0.052928748, -0.013561362, -0.001028923},
{3.79931E-05, -0.0229477, -0.139178011, 0.249959607, 1, 0.249959607, -0.139178011, -0.0229477, 3.79931E-05},
{-0.001028923, -0.013561362, 0.052928748, 0.46016215, 0.249959607 , -0.302454279, -0.108372072, 0.000389916, 0.000744712},
{-0.000279806, 0.002373205, 0.044837725, 0.052928748, -0.139178011, -0.108372072, 0.000847346, 0.003516954, 0.000132863},
{2.2532E-05, 0.00092512, 0.002373205, -0.013561362, -0.0229477, 0.000389916, 0.003516954, 0.000288732, -9.04408E-06},
{4.0418E-06, 2.2532E-05, -0.000279806, -0.001028923, 3.79931E-05 , 0.000744712, 0.000132863, -9.04408E-06, -1.01551E-06}

};

