#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core.hpp"
#include "saliency_itti.h"
#include "..\debug_image.h"

SaliencyMap::SaliencyMap(int height, int width)
{
  // previous frame information
  prev_frame = NULL;
  
  // set Gabor Kernel (9x9)
  GaborKernel0.create(9, 9, CV_32FC1);
  GaborKernel45.create(9, 9, CV_32FC1);
  GaborKernel90.create(9, 9, CV_32FC1);
  GaborKernel135.create(9, 9, CV_32FC1);
  for(int i=0; i<9; i++) for(int j=0; j<9; j++){
    GaborKernel0.at<float>( i, j)= (float)GaborKernel_0[i][j]; // 0 degree orientation
    GaborKernel45.at<float>(i, j)= (float)GaborKernel_45[i][j]; // 45 degree orientation
    GaborKernel90.at<float>(i, j)= (float)GaborKernel_90[i][j]; // 90 degree orientation
    GaborKernel135.at<float>(i, j)= (float)GaborKernel_135[i][j]; // 135 degree orientation
  }
  // default definition params
  static const float WEIGHT_INTENSITY = (float)0.30;
  static const float WEIGHT_COLOR = (float)0.30;
  static const float WEIGHT_ORIENTATION = (float)0.20;
  static const float WEIGHT_MOTION = (float)0.20;

  static const float OFXSALIENCYMAP_DEF_RANGEMAX = (float)255.00;
  static const float OFXSALIENCYMAP_DEF_SCALE_GAUSS_PYRAMID = (float)1.7782794100389228012254211951927;	// = 100^0.125
  static const int   OFXSALIENCYMAP_DEF_DEFAULT_STEP_LOCAL = 8;
  smParams.SetWeights(WEIGHT_INTENSITY, WEIGHT_COLOR, WEIGHT_ORIENTATION, WEIGHT_MOTION);
}
SaliencyMap::~SaliencyMap(void)
{
	GaborKernel0.release();
	GaborKernel135.release();
	GaborKernel45.release();
	GaborKernel90.release();
	
	/*
  cvReleaseMat(&GaborKernel0);
  cvReleaseMat(&GaborKernel45);
  cvReleaseMat(&GaborKernel90);
  cvReleaseMat(&GaborKernel135);
  */
  }

cv::Mat SaliencyMap::SMGetSM(Mat &src,string tag_1)
{

  int inputWidth = src.cols; // width of the image
  int inputHeight = src.rows; // height of the image
  cv::Size sSize = cvSize(inputWidth, inputHeight);
  tag = tag_1;
  //=========================
  // Intensity and RGB Extraction
  //=========================
  Mat R, G, B, I;
  SMExtractRGBI(src, R, G, B, I);
  debug_image( "saliency/R_"+tag,R);
  debug_image( "saliency/G_"+tag,G);
  debug_image( "saliency/B_"+tag,B);
  debug_image( "saliency/I_"+tag,I);


  
  //=========================
  // Feature Map Extraction
  //=========================
  
  // intensity feature maps
  Mat IFM[6];
  IFMGetFM(I, IFM);
  for (int i = 0; i < 6; i++) {
	 
	  debug_image("saliency/IFM_"+tag,i,IFM[i]);
	 }
  // color feature maps
  cv::Mat CFM_RG[6];
  cv::Mat CFM_BY[6];

  CFMGetFM(R, G, B, CFM_RG, CFM_BY);
  for (int i = 0; i < 6; i++) {	 
	  debug_image( "saliency/CFM_RG_"+tag,i,CFM_RG[i]);
  }
  for (int i = 0; i < 6; i++) {
	  debug_image("saliency/CFM_BY_"+tag,i,CFM_BY[i]);
  }
  // orientation feature maps
  cv::Mat OFM[24];
  OFMGetFM(I, OFM);
  for (int i = 0; i < 24; i++) { 
	  debug_image("saliency/OFM_"+tag,i,OFM[i]);
  }
  // motion feature maps
  cv::Mat MFM_X[6];
  cv::Mat MFM_Y[6];
  MFMGetFM(I, MFM_X, MFM_Y);
  for (int i = 0; i < 6; i++) {
	  debug_image("saliency/MFM_X_"+tag,i,MFM_X[i]);
  }

  for (int i = 0; i < 6; i++) {
	  debug_image("saliency/MFM_Y_"+tag,i,MFM_Y[i]);
  }
  R.release();
  G.release();
  B.release();
  I.release();
  /*
  cvReleaseMat(&R);
  cvReleaseMat(&G);
  cvReleaseMat(&B);
  cvReleaseMat(&I);
  */
  //=========================
  // Conspicuity Map Generation
  //=========================

  cv::Mat ICM = ICMGetCM(IFM, sSize);
  cv::Mat CCM = CCMGetCM(CFM_RG, CFM_BY, sSize);
  cv::Mat OCM = OCMGetCM(OFM, sSize);
  cv::Mat MCM = MCMGetCM(MFM_X, MFM_Y, sSize);
  
  debug_image("saliency/ICM_"+tag,ICM);
  debug_image("saliency/CCM_"+tag,CCM);
  debug_image("saliency/OCM_"+tag,OCM);
  debug_image("saliency/MCM_"+tag,MCM);

  for(int i=0; i<6; i++){
    /*cvReleaseMat(&IFM[i]);
    cvReleaseMat(&CFM_RG[i]);
    cvReleaseMat(&CFM_BY[i]);
    cvReleaseMat(&MFM_X[i]);
    cvReleaseMat(&MFM_Y[i]);
  */
	  IFM[i].release();
	  CFM_RG[i].release();
	  CFM_BY[i].release();
	  MFM_X[i].release();
	  MFM_Y[i].release();
  }
  for(int i=0; i<24; i++) OFM[i].release();

  //=========================
  // Saliency Map Generation
  //=========================

  // Normalize conspicuity maps
  cv::Mat ICM_norm, CCM_norm, OCM_norm, MCM_norm;
  ICM_norm = SMNormalization(ICM);
  CCM_norm = SMNormalization(CCM);
  OCM_norm = SMNormalization(OCM);
  MCM_norm = SMNormalization(MCM);
  /*cvReleaseMat(&ICM);
  cvReleaseMat(&CCM);
  cvReleaseMat(&OCM);
  cvReleaseMat(&MCM);
  */
  // Adding Intensity, Color, Orientation CM to form Saliency Map
 // cv::Mat* SM_Mat = cvCreateMat(sHeight, sWidth, CV_32FC1); // Saliency Map matrix
  cv::Mat SM_Mat;SM_Mat.create(inputHeight, inputWidth, CV_32FC1); // Saliency Map matrix

  float _w_intensity, _w_color, _w_orient, _w_motion;
  smParams.GetWeights(_w_intensity, _w_color, _w_orient, _w_motion);
  cv::addWeighted(ICM_norm, _w_intensity, OCM_norm, _w_orient, 0.0, SM_Mat);
  debug_image("saliency/SM_Mat_"+tag,1,SM_Mat);
  cv::addWeighted(CCM_norm, _w_color, SM_Mat, 1.00, 0.0, SM_Mat);
  debug_image("saliency/SM_Mat_"+tag,2,SM_Mat);
  cv::addWeighted(MCM_norm, _w_motion, SM_Mat, 1.00, 0.0, SM_Mat);
/*  cvReleaseMat(&ICM_norm);
  cvReleaseMat(&CCM_norm);
  cvReleaseMat(&OCM_norm);
  cvReleaseMat(&MCM_norm);
  */
  // Output Result Map
  debug_image("saliency/SM_Mat_"+tag,3,SM_Mat);

  cv::Mat normalizedSM = SMRangeNormalize(SM_Mat);
  debug_image("saliency/normalizedSM_"+tag,normalizedSM);

  cv::Mat smoothedSM;smoothedSM.create(SM_Mat.rows, SM_Mat.cols, CV_32FC1); // Saliency Image Output
  cv::GaussianBlur(normalizedSM, smoothedSM, Size(7, 7),0); // smoothing (if necessary)
  cv::Mat SM;SM.create(inputHeight, inputWidth, CV_32FC1); // Saliency Image Output
  debug_image("saliency/smoothedSM_"+tag,smoothedSM);
  cv::resize(smoothedSM, SM, Size(inputWidth,inputHeight),CV_INTER_NN);
  debug_image("saliency/0_SMx256_"+tag,SM);
  /*cvReleaseMat(&SM_Mat);
  cvReleaseMat(&normalizedSM);
  cvReleaseMat(&smoothedSM);
  */
  //SM *= 256.;
  return SM;
}

void SaliencyMap::SMExtractRGBI(Mat  inputImage, Mat &R, Mat &G, Mat &B, Mat &I)
{
  int height = inputImage.rows;
  int width = inputImage.cols;

  // convert scale of array elements
  Mat  src;
  Mat i;
  debug_image("saliency/0_input_" + tag, inputImage);
  src.create(height, width, CV_32FC3);
  if (inputImage.channels() == 1) {
	 cvtColor(inputImage,i, CV_GRAY2RGB);
	  debug_image("saliency/0_input_8UC3_" + tag,i);
	  mat_print(i, "saliency/0_input_8UC3_" + tag);
	  inputImage = i.clone();
  }

  inputImage.convertTo(src,CV_32FC3);

  debug_image("saliency/1_saliency_src_32FC3"+tag, src);
   src *= 1 / 256.;
  
   // initalize matrix for I,R,G,B
  R.create(height, width, CV_32FC1);
  G.create(height, width, CV_32FC1);
  B.create(height, width, CV_32FC1);
  I.create(height, width, CV_32FC1);

  //Mat R_8UC3, G_8UC3,B_8UC3,I_8UC3;
  // split
  vector<Mat> channels;
  split(src, channels);
   channels[2].convertTo(R,CV_32FC1);
  channels[1].convertTo(G,CV_32FC1);
  channels[0].convertTo(B,CV_32FC1);
  for (int i = 0; i < 3; i++)
	  debug_image("saliency/channel_"+tag, i,channels[i]);
  // Split(src, B, G, R, NULL);

  // extract intensity image
  Mat I_8UC3,
	  I_256;
  cvtColor(src, I_8UC3, CV_BGR2GRAY);
  I_8UC3.convertTo(I_256,CV_32FC1);
  debug_image("saliency/I_8UC3_"+tag, I_8UC3);
  debug_image("saliency/I_32FC1_"+tag, I_256);

  I = I_256*1. / 256.;
  debug_image("saliency/I_32FC1_div_256_"+tag, I);
  // release
  src.release();
  //cvReleaseMat(&src);

}
void SaliencyMap::FMCenterSurroundDiff(Mat  GaussianMap[9], Mat dst[6])
{
	int i = 0;
	for (int s = 2; s<5; s++) {
		int now_height = GaussianMap[s].rows;
		int now_width = GaussianMap[s].cols;
	//	cout << endl<< "i= " << i << "s= " << s << " : " << "now height " << now_height << ", " << now_width << endl;
		Mat  tmp;
		tmp.create(now_height, now_width, CV_32FC1);
	
		dst[i].create(now_height, now_width, CV_32FC1);
		dst[i + 1].create(now_height, now_width, CV_32FC1);
		

		resize(GaussianMap[s + 3], tmp,Size(now_width,now_height), CV_INTER_LINEAR);
	
		absdiff(GaussianMap[s], tmp, dst[i]);
		resize(GaussianMap[s + 4], tmp, Size(now_width, now_height), CV_INTER_LINEAR);
		absdiff(GaussianMap[s], tmp, dst[i + 1]);
		tmp.release();
		//releaseMat(&tmp);
		i += 2;
	}

}
void SaliencyMap::FMCreateGaussianPyr(Mat& src, Mat dst[9],char *p)
{
	dst[0] = src.clone();// cvCloneMat(src);

	for (int i = 1; i<9; i++) {
		dst[i].create(dst[i - 1].rows / 2, dst[i - 1].cols / 2, CV_32FC1);
		pyrDown(dst[i - 1],dst[i]);
		debug_image("itty/pyr_"+tag+"_",i-1,dst[i - 1]);
	}
}

void  SaliencyMap::FMGaussianPyrCSD(Mat & src, Mat dst[6], char *p)
{
	Mat GaussianMap[9];
	FMCreateGaussianPyr(src, GaussianMap,p);
	FMCenterSurroundDiff(GaussianMap, dst);
	for (int i = 0; i < 9; i++)
		GaussianMap[i].release();
}



void SaliencyMap::IFMGetFM(Mat& src, Mat  dst[6])
{
  FMGaussianPyrCSD(src, dst,"FM");
}

void SaliencyMap::CFMGetFM(Mat & R, Mat 
	& G, Mat & B, Mat  RGFM[6], Mat BYFM[6])
{

  // allocate
  int height = R.rows;
  int width = R.cols;
  Mat tmp1;tmp1.create(height, width, CV_32FC1);
  Mat tmp2;tmp2.create(height, width, CV_32FC1);
  Mat RGBMax;RGBMax.create(height, width, CV_32FC1);
  Mat RGMin;RGMin.create(height, width, CV_32FC1);
  Mat RGMat;RGMat.create(height, width, CV_32FC1);
  Mat BYMat;BYMat.create(height, width, CV_32FC1);

  // Max(R,G,B)
  cv::max(R, G, tmp1);
  cv::max(B, tmp1, RGBMax);
  cv::max(RGBMax, 0.0001, RGBMax); // to prevent dividing by 0
  // Min(R,G)
  cv::min(R, G, RGMin);

  // R-G
  cv::subtract(R, G, tmp1);
  // B-Min(R,G)
  cv::subtract(B, RGMin, tmp2);
  // RG = (R-G)/Max(R,G,B)
  cv::divide(tmp1, RGBMax, RGMat);
  // BY = (B-Min(R,G)/Max(R,G,B)
  cv::divide(tmp2, RGBMax, BYMat);

  // Clamp negative value to 0 for the RG and BY maps
  cv::max(RGMat, 0, RGMat);
  cv::max(BYMat, 0, BYMat);

  // Obtain [RG,BY] color opponency feature map by generating Gaussian pyramid and performing center-surround difference
  FMGaussianPyrCSD(RGMat, RGFM,"RGFM");
  FMGaussianPyrCSD(BYMat, BYFM,"BYFM");

  // release
  /*
  cvReleaseMat(&tmp1);
  cvReleaseMat(&tmp2);
  cvReleaseMat(&RGBMax);
  cvReleaseMat(&RGMin);
  cvReleaseMat(&RGMat);
  cvReleaseMat(&BYMat);
  */
  }

void SaliencyMap::OFMGetFM(cv::Mat & I, cv::Mat dst[24])
{
  // Create gaussian pyramid
  cv::Mat GaussianI[9];
  FMCreateGaussianPyr(I, GaussianI,"I");

  // Convolution Gabor filter with intensity feature maps to extract orientation feature
  cv::Mat tempGaborOutput0[9];
  cv::Mat tempGaborOutput45[9];
  cv::Mat tempGaborOutput90[9];
  cv::Mat tempGaborOutput135[9];
  for(int j=2; j<9; j++){
    int now_height = GaussianI[j].rows;
    int now_width = GaussianI[j].cols;
    tempGaborOutput0[j].create(now_height, now_width, CV_32FC1);
    tempGaborOutput45[j].create(now_height, now_width, CV_32FC1);
    tempGaborOutput90[j].create(now_height, now_width, CV_32FC1);
    tempGaborOutput135[j].create(now_height, now_width, CV_32FC1);
    cv::filter2D(GaussianI[j], tempGaborOutput0[j],-1, GaborKernel0);
    cv::filter2D(GaussianI[j], tempGaborOutput45[j],-1, GaborKernel45);
    cv::filter2D(GaussianI[j], tempGaborOutput90[j], -1,GaborKernel90);
    cv::filter2D(GaussianI[j], tempGaborOutput135[j],-1, GaborKernel135);
	

  }
 // for(int j=0; j<9; j++) cvReleaseMat(&(GaussianI[j]));

  // calculate center surround difference for each orientation
  cv::Mat temp0[6];
  cv::Mat temp45[6];
  cv::Mat temp90[6];
  cv::Mat temp135[6];
  FMCenterSurroundDiff(tempGaborOutput0, temp0);
  FMCenterSurroundDiff(tempGaborOutput45, temp45);
  FMCenterSurroundDiff(tempGaborOutput90, temp90);
  FMCenterSurroundDiff(tempGaborOutput135, temp135);
  
  for (int j = 2; j < 9; j++) {
	  debug_image("saliency/GaborOutput0_"+tag, j, tempGaborOutput0[j]);
	  debug_image("saliency/GaborOutput45_" + tag, j, tempGaborOutput45[j]);
	  debug_image("saliency/GaborOutput90_" + tag, j, tempGaborOutput90[j]);
	  debug_image("saliency/GaborOutput135_" + tag, j, tempGaborOutput135[j]);
  }
  for (int j = 0; j<6; j++) {
	debug_image("saliency/GaborOutput0_CSD_" + tag,j, temp0[j]);
	debug_image("saliency/GaborOutput45_CSD_" + tag,j, temp45[j]);
	debug_image("saliency/GaborOutput90_CSD_" + tag, j,temp90[j]);
	debug_image("saliency/GaborOutput135_CSD_" + tag, j,temp135[j]);
	///*cvReleaseMat(&(tempGaborOutput0[i]));
    //cvReleaseMat(&(tempGaborOutput45[i]));
    //cvReleaseMat(&(tempGaborOutput90[i]));
    //cvReleaseMat(&(tempGaborOutput135[i]));
  
  }
 

  // saving the 6 center-surround difference feature map of each angle configuration to the destination pointer
  for(int i=0; i<6; i++){
    dst[i] = temp0[i];
    dst[i+6] = temp45[i];
    dst[i+12] = temp90[i];
    dst[i+18] = temp135[i];
  }
}

void SaliencyMap::MFMGetFM(cv::Mat& I, cv::Mat dst_x[], cv::Mat dst_y[])
{
  int height = I.rows;
  int width = I.cols;

  // convert
  cv::Mat I8U; I8U.create(height, width, CV_8UC1);
  I.convertTo( I8U, 256);

  // obtain optical flow information
  cv::Mat flowx;flowx.create(height, width, CV_32FC1);
  cv::Mat flowy;flowy.create(height, width, CV_32FC1);
  //cvSetZero(flowx);
  //cvSetZero(flowy);
  flowx.setTo(0.0);
  flowy.setTo(0.0);
  //if(this->prev_frame!=NULL){
   // cvCalcOpticalFlowLK(this->prev_frame, I8U, cvSize(7,7), flowx, flowy);

    //cvReleaseMat(&(this->prev_frame));
  //}

  // create Gaussian pyramid
  FMGaussianPyrCSD(flowx, dst_x,"dst_x");
  FMGaussianPyrCSD(flowy, dst_y,"dst_y");

  // update
  //this->prev_frame = cvCloneMat(I8U);

  // release
  //cvReleaseMat(&flowx);
  //cvReleaseMat(&flowy);
//  cvReleaseMat(&I8U);
}



void SaliencyMap::normalizeFeatureMaps(cv::Mat FM[], cv::Mat NFM[], int width, int height, int num_maps)
{
  for(int i=0; i<num_maps; i++){
	//  cout << "normalizeFeatureMaps : " << i << endl;
	
    cv::Mat  normalizedImage = SMNormalization(FM[i]);
    NFM[i].create(height, width, CV_32FC1);
	cv::resize(normalizedImage, NFM[i],Size(width,height), CV_INTER_LINEAR);
	debug_image("saliency/normalizedMap_" + tag, i, NFM[i]);
	//cv::ReleaseMat(&normalizedImage);
  }
}

double SMAvgLocalMax(cv::Mat &matsrc)
{
	int stepsize = (int)DEFAULT_STEP_LOCAL;
	int numlocal = 0;
	double lmaxmean = 0, lmax = 0, dummy = 0;
	CvMat cvmatsrc = matsrc;
	CvMat * src = &cvmatsrc;
	CvMat localMatHeader;
	cvInitMatHeader(&localMatHeader, stepsize, stepsize, CV_32FC1, src->data.ptr, src->step);
	//cvInitMatHeader(localMatHeader, stepsize, stepsize, CV_32FC1, src.data, src.step1());

	for (int y = 0; y<src->height - stepsize; y += stepsize) { // Note: the last several pixels may be ignored.
		for (int x = 0; x<src->width - stepsize; x += stepsize) {
			localMatHeader.data.ptr = src->data.ptr + sizeof(float)*x + src->step*y; // get local matrix by pointer trick
			cvMinMaxLoc(&localMatHeader, &dummy, &lmax);
			lmaxmean += lmax;
			numlocal++;
		}
	}

	return lmaxmean / numlocal;
}

cv::Mat SaliencyMap::SMNormalization(cv::Mat & src)
{
  
	cv::Mat result;result.create(src.rows, src.cols, CV_32FC1);

  // normalize so that the pixel value lies between 0 and 1
  cv::Mat tempResult = SMRangeNormalize(src);
 
  // single-peak emphasis / multi-peak suppression
  double lmaxmean = SMAvgLocalMax(tempResult);
  double normCoeff = (1-lmaxmean)*(1-lmaxmean);
 
 result=tempResult*normCoeff;
  
  //cvReleaseMat(&tempResult);
  return result;
}

cv::Mat SaliencyMap::SMRangeNormalize(cv::Mat& src)
{
	double maxx, minn;
	cv::minMaxLoc(src, &minn, &maxx);
	cv::Mat result; result.create(src.rows, src.cols, CV_32FC1);
	if (maxx != minn) {
	//src.convertTo(result, 1 / (maxx - minn), minn / (minn - maxx));
	result = (src * 1 / (maxx - minn)) + minn / (minn - maxx);

	}
	else {
		//src.convertTo(result, 1, -minn);
		result = src - minn;
	}
  return result;
}


cv::Mat  SaliencyMap::ICMGetCM(cv::Mat IFM[], cv::Size size)
{
  int num_FMs = 6;

  // Normalize all intensity feature maps
  cv::Mat NIFM[6];
  normalizeFeatureMaps(IFM, NIFM, size.width, size.height, num_FMs);

  // Formulate intensity conspicuity map by summing up the normalized intensity feature maps
  cv::Mat ICM(size.height, size.width, CV_32FC1);
 // cvSetZero(ICM);
  ICM.setTo(0.0);
  for (int i=0; i<num_FMs; i++){
    cv::add(ICM, NIFM[i], ICM);
    //cvReleaseMat(&NIFM[i]);
  }

  return ICM;
}

cv::Mat  SaliencyMap::CCMGetCM(cv::Mat CFM_RG[], cv::Mat CFM_BY[], cv::Size size)
{
  int num_FMs = 6;
  cv::Mat CCM_RG = ICMGetCM(CFM_RG, size);
  cv::Mat CCM_BY = ICMGetCM(CFM_BY, size);

  cv::Mat CCM(size.height, size.width, CV_32FC1);
  cv::add(CCM_BY, CCM_RG, CCM);

  //cvReleaseMat(&CCM_BY);
  //cvReleaseMat(&CCM_RG);

  return CCM;
}

cv::Mat  SaliencyMap::OCMGetCM(cv::Mat OFM[], cv::Size size)
{
  int num_FMs_perAngle = 6;
  int num_angles = 4;
  int num_FMs = num_FMs_perAngle * num_angles;

  // split feature maps into four sets
  cv::Mat  OFM0[6];
  cv::Mat  OFM45[6];
  cv::Mat  OFM90[6];
  cv::Mat  OFM135[6];
  for (int i=0; i<num_FMs_perAngle; i++){
    OFM0[i] = OFM[0*num_FMs_perAngle+i];
    OFM45[i] = OFM[1*num_FMs_perAngle+i];
    OFM90[i] = OFM[2*num_FMs_perAngle+i];
    OFM135[i] = OFM[3*num_FMs_perAngle+i];
  }

  // extract conspicuity map for each angle
  cv::Mat  NOFM_tmp[4];
  NOFM_tmp[0] = ICMGetCM(OFM0, size);
  NOFM_tmp[1] = ICMGetCM(OFM45, size);
  NOFM_tmp[2] = ICMGetCM(OFM90, size);
  NOFM_tmp[3] = ICMGetCM(OFM135, size);

  // Normalize all orientation features map grouped by their orientation angles
  cv::Mat NOFM[4];
  for (int i=0; i<4; i++){
    NOFM[i] = SMNormalization(NOFM_tmp[i]);
  //  cvReleaseMat(&NOFM_tmp[i]);
  }

  // Sum up all orientation feature maps, and form orientation conspicuity map
  cv::Mat OCM(size.height, size.width, CV_32FC1);
  //cvSetZero(OCM);
  OCM.setTo(0.0);
  for(int i=0; i<4; i++){
    cv::add(NOFM[i], OCM, OCM);
    //cvReleaseMat(&NOFM[i]);
  }

  return OCM;
}

cv::Mat  SaliencyMap::MCMGetCM(cv::Mat MFM_X[], cv::Mat MFM_Y[], cv::Size size)
{
  return CCMGetCM(MFM_X, MFM_Y, size);
}
/*
cv::Mat * SaliencyMap::SMGetSMFromVideoFrame(CvCapture *input_video, IplImage *&inputFrame_cur, int frameNo, bool retinal_mode)
{

  // read the video's frame size
  cv::Size frame_size;
  frame_size.height = (int)cvGetCaptureProperty(input_video, CV_CAP_PROP_FRAME_HEIGHT);
  frame_size.width = (int)cvGetCaptureProperty(input_video, CV_CAP_PROP_FRAME_WIDTH);

  // get current input frame
  cvSetCaptureProperty(input_video, CV_CAP_PROP_POS_FRAMES, frameNo);
  IplImage * cur_frame = cvQueryFrame(input_video);
  if (cur_frame == NULL){
    printf("Null frame found.");
    exit(1);
  }
  // copy
  inputFrame_cur = cvCloneImage(cur_frame);

  // generate (deterministic) saliency map
  cv::Mat * SMout = SMGetSM(inputFrame_cur); //saliency saliency generation

  // function for retinal filter
  if(retinal_mode){
    bool ib_mode = false;
    cv::Mat* SMout_withRetinal = IB(SMout, ib_mode);
    cvReleaseMat(&SMout);
    return SMout_withRetinal;
  }
  else return SMout;

}
*/
/*
#ifndef IGNORE_VIDEOINPUT_LIBRARY
cv::Mat * SaliencyMap::SMGetSMFromVideoFrameWebcam(videoInput &vi, int dev_id, IplImage *&inputFrame_cur, bool retinal_mode)
{

  // read the video's frame size
  cv::Size frame_size;
  frame_size.height = (int)vi.getHeight(dev_id);
  frame_size.width = (int)vi.getWidth(dev_id);
  std::cerr << " width = " << frame_size.width << " height = " << frame_size.height << std::endl;

  // prepare a buffer for frame data
  static IplImage * prev_inputFrame = cvCreateImage(frame_size, IPL_DEPTH_8U, 3);
  inputFrame_cur = cvCreateImage(frame_size, IPL_DEPTH_8U, 3);
  cvSetZero(inputFrame_cur);
  char * buffer = inputFrame_cur->imageData;

  // get current input frame
  if(vi.isFrameNew(dev_id)){
    vi.getPixels(dev_id, (unsigned char *)buffer, false, true);
    cvCopy(inputFrame_cur, prev_inputFrame);
  }
  else
    cvCopy(prev_inputFrame, inputFrame_cur);

  // flip input frame (if necessary)
  //cvConvertImage(inputFrame_cur, inputFrame_cur, CV_CVTIMG_FLIP); // maybe does not work well

  // size check and refine
  IplImage * inputFrame_cur2 = NULL;
  float expand_ratio = 1.0;
  cv::Size new_size = frame_size;
  int wh_min = MIN(frame_size.width, frame_size.height);
  if(frame_size.width < frame_size.height){ // If the width is smaller than the height, a new width should be set to 256.
    expand_ratio = 256/(float)frame_size.width;
    new_size = cvSize(256, frame_size.height*expand_ratio);
  }
  else {
    expand_ratio = 256/(float)frame_size.height;
    new_size = cvSize(frame_size.width*expand_ratio, 256);
  }
  inputFrame_cur2 = cvCreateImage(new_size, IPL_DEPTH_8U, 3);
  cvResize(inputFrame_cur, inputFrame_cur2, CV_INTER_LINEAR);

  // generate (deterministic) saliency map
  cv::Mat* SMout2 = SMGetSM(inputFrame_cur2); //saliency saliency generation
  cvReleaseImage(&inputFrame_cur2);
  // resize
  cv::Mat* SMout = cvCreateMat(frame_size.height, frame_size.width, CV_32FC1);
  cvResize(SMout2, SMout, CV_INTER_LINEAR);
  cvReleaseMat(&SMout2);

  // function for retinal filter
  if(retinal_mode){
    bool ib_mode = false;
    cv::Mat* SMout_withRetinal = IB(SMout, ib_mode);
    cvReleaseMat(&SMout);
    return SMout_withRetinal;
  }
  else return SMout;

}
#endif IGNORE_VIDEOINPUT_LIBRARY
*/