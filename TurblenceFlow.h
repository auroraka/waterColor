#ifndef TURBLENCEFLOW_H
#define TURBLENCEFLOW_H

#include <opencv2/opencv.hpp>

class TurblenceFlow
{
public:
	void deal(const cv::Mat &input, cv::Mat &output);

};

#endif