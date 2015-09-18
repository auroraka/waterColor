#ifndef WATERCOLOR_H
#define WATERCOLOR_H

#include <opencv2/opencv.hpp>

class WaterColor
{
public:
	void deal(const cv::Mat &input, cv::Mat &output);
	void deal(cv::Mat& src, cv::Mat& dst);
};


#endif