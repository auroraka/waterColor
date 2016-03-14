#include "ColorAdjustment.h"
#include "ToolBox.h"
#include "Debug.h"
using namespace cv;

ColorAdjustment::ColorAdjustment(){
//	MEAN = Scalar(42, 18, 27);
//	STDDEV = Scalar(25, 21, 28);
	MEAN = Scalar(57,14,21);
	STDDEV = Scalar(21, 19, 25);
}

void ColorAdjustment::deal(const Mat &input, Mat &output)
{
	//colorTransform(input, output);
	//getTargetStdAndMean();
	//output = input.clone();
	
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
	dstLab = mul((srcLab - srcMean), (divVec(targetStd, srcStd))) + targetMean;

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
	dstLab = mul((srcLab - srcMean), (divVec(targetStd, srcStd))) + targetMean;

	Mat dstd;
	cvtColor(dstLab, dstd, CV_Lab2BGR);

	DoubleToImage(dstd, dst);
}


void ColorAdjustment::getTargetStdAndMean()
{
	MEAN = Scalar::all(0);
	STDDEV = Scalar::all(0);
	int count = 0;
	for (int i = 24; i <= 38; i++)
	{
		Debug() <<"loading: "<< i;
		char fileDir[300]; 
		sprintf(fileDir, ".//source//%d.jpg", i);
		
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


void ColorAdjustment::getTargetStdAndMean(Mat &img, Scalar &imgStd, Scalar &imgMean)
{
	Mat imgd;
	ImageToDouble(img, imgd);

	Mat imgLab;
	cvtColor(imgd, imgLab, CV_BGR2Lab);

	meanStdDev(imgLab, imgMean, imgStd);

}

#include <fstream>
void ColorAdjustment::chooseOneStyle(char* style){
	Debug() << "choose style: " << style;
	char dir[300];
	sprintf(dir, "styleLib/%s/list.txt", style);
	ifstream fin(dir);
	char name[300];
	Scalar stdAll = Scalar::all(0), meanAll = Scalar::all(0); int count = 0;
	while (fin>>name){
		sprintf(dir, "styleLib/%s/%s", style, name);
		Debug() << "load pic: " << name;
		Mat src = imread(dir);
		Scalar std, mean;
		getTargetStdAndMean(src, std, mean);
		stdAll = stdAll+std;
		meanAll = meanAll+mean;
		count++;
	}
	MEAN = divVec(meanAll, count);
	STDDEV = divVec(stdAll, count);
	Debug() << "style mean :" << MEAN;
	Debug() << "style std :" << STDDEV;
}