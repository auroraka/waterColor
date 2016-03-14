#ifndef ABSTRECTION_H
#define ABSTRECTION_H

#include <opencv2/opencv.hpp>

class Abstraction
{
public:
	void deal(const cv::Mat &input, cv::Mat &output);
	void deal(const cv::Mat& src, cv::Mat& mySaliency, cv::Mat& myDis, cv::Mat& dst);
	void deal(cv::Mat& src, cv::Mat& mySaliency, cv::Mat& myDis, cv::Mat& dst);
};

#endif