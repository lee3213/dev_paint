#pragma once
#include "stdafx.h"
#include "opencv2\\opencv.hpp"
#include "opencv2\\core.hpp"
#include "opencv2\\face.hpp"
#include "util.h"
#include "render_.h"
using namespace cv::face;
#include "opencv2\\face\\facerec.hpp"
static Mat norm_0_255(InputArray _src) {
	Mat src = _src.getMat();
	// Create and return normalized image:
	Mat dst;
	switch (src.channels()) {
	case 1:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}
int  render_::lbph() {
	vector<Mat> images;
	vector<int> labels;
	string fn_csv = string("/render/brush/train.csv");
	string output_folder = string("/rst");
	try {
		read_csv(fn_csv, images, labels);
	}
	catch (cv::Exception& e) {
		cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
		// nothing more we can do
		return -8888;
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
		CV_Error(Error::StsError, error_message);
	}
	int height = images[0].rows;
	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();
	//Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create(2, 4, 10, 10);
	Ptr<EigenFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);
	int predictedLabel = model->predict(testSample);
	//
	// To get the confidence of a prediction call the model with:
	//
	//      int predictedLabel = -1;
	//      double confidence = 0.0;
	//      model->predict(testSample, predictedLabel, confidence);
	//
	string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
	cout << result_message << endl;
	// Here is how to get the eigenvalues of this Eigenfaces model:
	Mat eigenvalues = model->getEigenValues();
	// And we can do the same to display the Eigenvectors (read Eigenfaces):
	Mat W = model->getEigenVectors();
	// Get the sample mean from the training data
	Mat mean = model->getMean();
	imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
	for (int i = 0; i < min(10, W.cols); i++) {
		string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
		cout << msg << endl;
		// get eigenvector #i
		Mat ev = W.col(i).clone();
		// Reshape to original size & normalize to [0...255] for imshow.
		Mat grayscale = norm_0_255(ev.reshape(1, height));
		// Show the image & apply a Jet colormap for better sensing.
		Mat cgrayscale;
		applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
		// Display or save:
		
			imwrite(format("%s/eigenface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
		}


for (int num_components = min(W.cols, 10); num_components < min(W.cols, 300); num_components += 15) {
	// slice the eigenvectors from the model
	Mat evs = Mat(W, Range::all(), Range(0, num_components));
	Mat projection = LDA::subspaceProject(evs, mean, images[0].reshape(1, 1));
	Mat reconstruction = LDA::subspaceReconstruct(evs, mean, projection);
	// Normalize the result:
	reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
	// Display or save:
	
		imwrite(format("%s/eigenface_reconstruction_%d.png", output_folder.c_str(), num_components), reconstruction);
	}
return 0;
}
