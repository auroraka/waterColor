#ifndef COLORADJUSTMENT_H
#define COLORADJUSTMENT_H


#include <opencv2/opencv.hpp>


class ColorAdjustment
{
public:
	ColorAdjustment();
	void colorTransform(cv::Mat& src, cv::Mat& target, cv::Mat& dst);
	void colorTransform(const cv::Mat& src, cv::Mat& target, cv::Mat& dst);
	void colorTransform(const cv::Mat& src, cv::Scalar targetMean, cv::Scalar targetStd, cv::Mat& dst);
	void deal(const cv::Mat &input, cv::Mat &output);
	void getTargetStdAndMean();
	void getTargetStdAndMean(cv::Mat& img, cv::Scalar& imgStd, cv::Scalar& imgMean);
	void chooseOneStyle(char* style);
	void loadPitureLibary(char* style);
	void getStdAndMean();
private:
	cv::Scalar MEAN;
	cv::Scalar STDDEV;
};

#endif