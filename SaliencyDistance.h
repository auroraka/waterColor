#ifndef SALIENCYDIATANCE_H
#define SALIENCYDIATANCE_H

#include <opencv2/opencv.hpp>

class SaliencyDistance
{
public:
	void deal(const cv::Mat &input, cv::Mat &output);
};


#endif