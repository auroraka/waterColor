#ifndef EDGEDARKENING_H
#define EDGEDARKENING_H

#include <opencv2/opencv.hpp>

class EdgeDarkening
{
public:	
	void deal(const cv::Mat &input, cv::Mat &output);
};


#endif