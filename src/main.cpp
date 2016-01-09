#include <iostream>
#include <string>
#include "JonDepth.h"

#include <opencv2\opencv.hpp>

int main(int argc, char* argv[])
{
	std::string depthFileName(argv[1]);
	cv::Mat_<ushort> depthData = cv::imread(depthFileName, CV_LOAD_IMAGE_ANYDEPTH);
	Jon::depth depthImage((ushort*)(depthData.data), depthData.rows, depthData.cols);
	uchar* colorImage = depthImage.convert2Color();
	cv::Mat_<cv::Vec3b> cvColorImage;
	cvColorImage.create(depthData.rows, depthData.cols);
	std::copy(colorImage, colorImage + depthData.rows * depthData.cols * 3, cvColorImage.data);
	delete[] colorImage;
	cv::imshow("display", cvColorImage);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}