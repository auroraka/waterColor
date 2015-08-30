#ifndef COLORADJUSTMENT_H
#define COLORADJUSTMENT_H


#include <opencv2/opencv.hpp>


class ColorAdjustment
{
public:
	ColorAdjustment();
	void deal(const cv::Mat &input, cv::Mat &output);
	void getStdAndMean();
private:
	cv::Scalar MEAN;
	cv::Scalar STDDEV;
};

#endif