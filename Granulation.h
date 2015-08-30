#ifndef GRANULATION_H
#define GRANULATION_H

#include <opencv2/opencv.hpp>

class Granulation
{
public:
	void deal(const cv::Mat &input, cv::Mat &output);

};


#endif