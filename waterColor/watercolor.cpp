#include "ToolBox.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main(){
	Mat src = imread("test3.jpg");
	Mat target = imread("test4.jpg");
	
	Mat srcd, targetd;
	ImageToDouble(src, srcd);
	ImageToDouble(target, targetd);
	
	Mat srcLab, targetLab;
	cvtColor(srcd, srcLab, CV_BGR2Lab);
	cvtColor(targetd, targetLab, CV_BGR2Lab);
	
	Scalar srcMean, targetMean, srcStd, targetStd;
	meanStdDev(srcLab, srcMean, srcStd);
	meanStdDev(targetLab, targetMean, targetStd);
	
	Mat dstLab;
	dstLab = mul((srcLab - srcMean),(div(targetStd , srcStd))) + targetMean;
	
	Mat dstd;
	cvtColor(dstLab, dstd, CV_Lab2BGR);

	Mat dst;
	DoubleToImage(dstd, dst);

}