#include "stdafx.h"

#include "bsShowImage.h"



#include <iostream>
using namespace std;
#include "debug_image.h"

char bsShowImage::key = -1;
/*
void bsShowImage::show(const char* winname, const cv::Mat& img, int delay)
{
	imshow(winname, img);
	char ch = waitKey(delay);
	if (ch != -1)
		key = ch;
}
*/
void bsShowImage::save(const char* winname, const cv::Mat& img, int delay)
{
	imwrite(winname, img);
	char ch = waitKey(delay);
	if (ch != -1)
		key = ch;
}

char bsShowImage::getKey()
{
	char ret = key;
	key = -1;
	return ret;
}

cv::Mat bsShowImage::TakeGaussianBlurImg(cv::Mat srcImg)
{
	cv::Mat rstImg = srcImg.clone();

	unsigned char* srcData = (unsigned char*)srcImg.data;
	unsigned char* rstData = (unsigned char*)rstImg.data;
	int width = srcImg.size().width;
	int height = srcImg.size().height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index;
			if (rstImg.channels() == 1)
				index = (int)(y * rstImg.step1() + x);
			else
				index = (int)((y * rstImg.step1()) + (x * rstImg.channels()));

			int left		= x == 0			? index : index - 1;
			int right		= x == width - 1	? index : index - 1;
			int top			= y == 0			? index : index - (int)rstImg.step1();
			int bottom		= y == height - 1	? index : index + (int)rstImg.step1();

			int lefttop		= x == 0 && y == 0			? index : (x == 0 && y != 0			? index - (int)rstImg.step1() : (x != 0 && y == 0 ? index - 1 : index - 1 - (int)rstImg.step1()));
			int leftbottom	= x == 0 && y == height - 1 ? index : (x == 0 && y != 0			? index + (int)rstImg.step1() : (x != 0 && y == 0 ? index - 1 : index - 1 + (int)rstImg.step1()));
			int righttop	= x == width - 1 && y == 0	? index : (x == width - 1 && y != 0 ? index - (int)rstImg.step1() : (x != width - 1 && y == 0 ? index - 1 : index - 1 - (int)rstImg.step1()));
			int rightbottim = x == width - 1 && y == height - 1 ? index : (x == width - 1 && y != height - 1 ? index + (int)rstImg.step1() : (x != width - 1 && y == height - 1 ? index - 1 : index - 1 + (int)rstImg.step1()));

			if (rstImg.channels() == 1)
				rstData[index] =(unsigned int)( 0.0625 * srcData[lefttop] + 0.125 * srcData[top] + 0.0625 * srcData[righttop] + 0.125 * srcData[left] + 0.25 * srcData[index]
								+ 0.125 * srcData[right] + 0.0625 * srcData[leftbottom] + 0.125 * srcData[bottom] + 0.0625 * srcData[rightbottim]);
			else
			{
				rstData[index + 2] = (unsigned int)( 0.0625 * srcData[lefttop + 2] + 0.125 * srcData[top + 2] + 0.0625 * srcData[righttop + 2] + 0.125 * srcData[left + 2] + 0.25 * srcData[index + 2]
									+ 0.125 * srcData[right + 2] + 0.0625 * srcData[leftbottom + 2] + 0.125 * srcData[bottom + 2] + 0.0625 * srcData[rightbottim + 2]);
				rstData[index + 1] = (unsigned int)(0.0625 * srcData[lefttop + 1] + 0.125 * srcData[top + 1] + 0.0625 * srcData[righttop + 1] + 0.125 * srcData[left + 1] + 0.25 * srcData[index + 1]
									+ 0.125 * srcData[right + 1] + 0.0625 * srcData[leftbottom + 1] + 0.125 * srcData[bottom + 1] + 0.0625 * srcData[rightbottim + 1]);
				rstData[index + 0] = (unsigned int)(0.0625 * srcData[lefttop] + 0.125 * srcData[top] + 0.0625 * srcData[righttop] + 0.125 * srcData[left] + 0.25 * srcData[index]
									+ 0.125 * srcData[right] + 0.0625 * srcData[leftbottom] + 0.125 * srcData[bottom] + 0.0625 * srcData[rightbottim]);
			}
		}
	}

	return rstImg;
}

cv::Mat bsShowImage::RGB2GRAY(cv::Mat srcImg)
{
	int width = srcImg.size().width;
	int height = srcImg.size().height;

	cv::Mat rstImg(height, width, CV_8UC1);

	unsigned char* srcData = (unsigned char*)srcImg.data;
	unsigned char* rstData = (unsigned char*)rstImg.data;
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = (int)(y * srcImg.step1() + x * srcImg.channels());

			unsigned int r = srcData[index + 2];
			unsigned int g = srcData[index + 1];
			unsigned int b = srcData[index + 0];

			unsigned int gray = (int)(0.2126 * (double)r + 0.7152 * (double)g + 0.0722 + (double)b);

			rstData[y * rstImg.step1() + x] = gray;
		}
	}

	return rstImg;
}

cv::Mat bsShowImage::TakeGradient(cv::Mat srcImg)
{
	cv::Mat rstImg = srcImg.clone();

	int width = rstImg.size().width;
	int height = rstImg.size().height;

	int onerowsize =(int)rstImg.step1();//with channels value
	int channels = rstImg.channels();
	int endx = width - 1;
	int endy = height - 1;


	cv::Mat sobel_y = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat sobel_x = cv::Mat::zeros(height, width, CV_8UC1);

	unsigned char* srcData = (unsigned char*)srcImg.data;
	unsigned char* rstData = (unsigned char*)rstImg.data;
	unsigned char * sobel_x_Data = (unsigned char *)sobel_x.data;
	unsigned char * sobel_y_Data = (unsigned char *)sobel_y.data;

/*	cout << "channel " << rstImg.channels() << ", step1 " << rstImg.step1() << endl;
	cout << "Size : "<< rstImg.size() << endl;
	cout << "channel " << sobel_x.channels() << ", step1 " << sobel_x.step1() << endl;
	cout << "width " << width << " height " << height << endl;
	*/
	//findout 8 neighbours
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int apixel_index = (int)((y * onerowsize) + (x * channels));

			//corner check
			int left		= x == 0		   ? apixel_index : apixel_index - 1*channels;
			int right		= x == endx   ? apixel_index : apixel_index - 1*channels;
			int upper			= y == 0		   ? apixel_index : apixel_index - onerowsize;
			int below		= y == endy  ? apixel_index : apixel_index + onerowsize;

			int lefttop	=( x == 0 && y == 0)	? apixel_index : 
				(x == 0		&&		y != 0	? apixel_index - onerowsize :
				(x != 0		&&		y == 0	? apixel_index - 1*channels : apixel_index - 1*channels - onerowsize));
			int leftbottom = x == 0 && y == endy	? apixel_index : 
				(x == 0		&&		y != 0				? apixel_index + onerowsize :
				(x != 0		&&		y == 0				? apixel_index - 1*channels : apixel_index - 1*channels + onerowsize));
			int righttop	= x == endx && y == 0	? apixel_index : 
				(x == endx		&&		y != 0		? apixel_index - onerowsize :
				(x != endx		&&		y == 0		? apixel_index - 1*channels : apixel_index - 1*channels - onerowsize));
			int rightbottim = x == endx && y == endy ? apixel_index : 
				(x == endx		&&		y != endy		? apixel_index + (onerowsize) :
				(x != endx		&&		y == endy		? apixel_index - 1*channels : apixel_index - 1*channels + onerowsize));

			// get dx, dy for each channel //assumed only 1 channel // sobel operator
			int dx = srcData[righttop] - srcData[lefttop] + 2 * srcData[right]  
				- 2 * srcData[left] + srcData[rightbottim] - srcData[leftbottom];
			int dy = srcData[lefttop] - srcData[leftbottom] + 2 * srcData[upper] 
				- 2 * srcData[below] + srcData[righttop] - srcData[rightbottim];
			sobel_x_Data[apixel_index] = dx;
			sobel_y_Data[apixel_index] = dy;
			rstData[apixel_index] = abs(dx) + abs(dy);//approximate magnitude
		}
	}
	debug_image("sobel/take_gradient_x", sobel_x);
	debug_image("sobel/take_gradient_y", sobel_y);
	debug_image("sobel/take_gradient_abs__x_+_abs_y", rstImg);
	return rstImg;
}

cv::Mat bsShowImage::TakeSaliencyUnionMap(cv::Mat saliImg, cv::Mat gradImg)
{
	cv::Mat rstImg = gradImg.clone();
	unsigned char* rstData = (unsigned char*)rstImg.data;
	unsigned char* saliData = (unsigned char*)saliImg.data;
	unsigned char* gradData = (unsigned char*)gradImg.data;

	int width = rstImg.size().width;
	int height = rstImg.size().height;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y * (int)rstImg.step1() + x * (int)rstImg.channels();
			rstData[index] = saliData[index] > gradData[index] ? saliData[index] : gradData[index];
		}
	}
	return rstImg;
}