#include "ColorAdjustment.h"
#include "ToolBox.h"
using namespace cv;

ColorAdjustment::ColorAdjustment(){
	MEAN = Scalar(64, 3, 10);
	STDDEV = Scalar(20, 10, 15);
}

void ColorAdjustment::deal(const Mat &input, Mat &output)
{
	//colorTransform(input, output);
	//getTargetStdAndMean();
	colorTransform(input, MEAN, STDDEV, output);
}



//分为4步
//1.将src和target转为double格式
//2.将src和target从RGB空间转为Lab空间
//3.作变换dstLab = (srcLab - mean(src)) * (std(target) / std(src)) + mean(taarget)
//4.dstLab转回RGB空间并变回uchar格式
void ColorAdjustment::colorTransform(const Mat &src,Mat &target,Mat &dst){
	
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
	dstLab = mul((srcLab - srcMean), (div(targetStd, srcStd))) + targetMean;

	Mat dstd;
	cvtColor(dstLab, dstd, CV_Lab2BGR);

	DoubleToImage(dstd, dst);
}

void ColorAdjustment::colorTransform(const Mat &src, Scalar targetMean,Scalar targetStd, Mat &dst){
	
	Mat srcd;
	ImageToDouble(src, srcd);
	
	Mat srcLab;
	cvtColor(srcd, srcLab, CV_BGR2Lab);
	
	Scalar srcMean, srcStd;
	meanStdDev(srcLab, srcMean, srcStd);

	Mat dstLab;
	dstLab = mul((srcLab - srcMean), (div(targetStd, srcStd))) + targetMean;

	Mat dstd;
	cvtColor(dstLab, dstd, CV_Lab2BGR);

	DoubleToImage(dstd, dst);
}


void ColorAdjustment::getTargetStdAndMean()
{
	MEAN = Scalar::all(0);
	STDDEV = Scalar::all(0);
	int count = 0;
	for (int i = 1; i <= 23; i++)
	{
		char fileDir[300]; 
		sprintf(fileDir, ".//source//%d.jpg", i);
		//sprintf(fileDir, "test4.jpg");
		Mat img = imread(fileDir);

		Mat imgd;
		ImageToDouble(img, imgd);

		Mat imgLab;
		cvtColor(imgd, imgLab, CV_BGR2Lab);

		Scalar imgMean, imgStd;
		meanStdDev(imgLab, imgMean, imgStd);

		MEAN = MEAN + imgMean;
		STDDEV = STDDEV + imgStd;

		count++;
	}
	MEAN /= count;
	STDDEV /= count;
	std::cout << "allMean: " << MEAN[0] << " " << MEAN[1] << " " << MEAN[2] << std::endl;
	std::cout << "allStdDev: " << STDDEV[0] << " " << STDDEV[1] << " " << STDDEV[2] << std::endl;
	system("pause");
};