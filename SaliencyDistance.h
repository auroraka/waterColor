#ifndef SALIENCYDIATANCE_H
#define SALIENCYDIATANCE_H

#include "include\geometry\Geometry.h"
#include <opencv2/opencv.hpp>


class SaliencyDistance
{
public:
	void saliency(cv::Mat& src, cv::Mat& saliency);
	void jumpFlooding(cv::Mat& src, cv::Mat& jump);
	void jumpFlooding(cv::Mat& saliency, cv::Mat& jump, Geometry::Point** ancestor);
	void SaliencyDistance::normalize(cv::Mat &dis, Geometry::Point** ancestor);
	void deal(cv::Mat& src, cv::Mat& dstSaliency, cv::Mat& dstDis);
	void deal(cv::Mat& src, cv::Mat& dst);
	void jumpFlooding(const cv::Mat& saliency, cv::Mat& jump);
	void deal(const cv::Mat &input, cv::Mat &output);
};


#endif
