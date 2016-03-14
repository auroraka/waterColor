#ifndef WETINWET_H
#define WEIINWET_H

#include <opencv2/opencv.hpp>

class WetInWet
{
public:
	void deal(cv::Mat& src, cv::Mat& myAbstraction, cv::Mat& dst);
	void deal(cv::Mat& src, cv::Mat& myAbstraction, cv::Mat& dst, cv::Mat& myCanny);
};


#endif