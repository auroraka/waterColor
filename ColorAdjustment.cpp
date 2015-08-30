#include "ColorAdjustment.h"
#include "ToolBox.h"
using namespace cv;

ColorAdjustment::ColorAdjustment(){
	//MEAN = Scalar(139, 155, 166);
	//STDDEV = Scalar(52, 55, 57);
}

Mat operator * (const Mat &a, const Scalar &b){
	Mat c = a.clone();
	vector<Mat> clist;
	split(c,clist);
	for (int i = 0; i < 3; i++)
	{
		clist[i] = clist[i] * b[i];
	}
	merge(clist, c);
	return c;
}
void ColorAdjustment::deal(const Mat &input, Mat &output)
{
//	getStdAndMean();
//
//	output = input.clone();
//	//DoubleNomolization(output);
//	Mat lab;
//	cvtColor(output, lab, CV_BGR2Lab);
//	Scalar Mean, Stddev;
//	meanStdDev(lab, Mean, Stddev);
//	//output = lab;// (lab - Mean) *(STDDEV / Stddev) + MEAN;
//	output = (lab - Mean) *(STDDEV / Stddev) + MEAN;
//	cvtColor(output, output, CV_Lab2BGR);
//	
//	//write<Vec3b>(output, 20, 20);

	Mat src = imread("test3.jpg");
	Mat tar = imread("test4.jpg");
	src.convertTo(src, CV_32FC3, 1.0 / 255.0);
	tar.convertTo(tar, CV_32FC3, 1.0 / 255.0);
	cvtColor(src, src, CV_BGR2Lab);
	cvtColor(tar, tar, CV_BGR2Lab);
	Scalar meanSrc = getMean(src), stdSrc = getStdDev(src);
	Scalar meanTar = getMean(tar), stdTar = getStdDev(tar);
	Mat dst;
	dst = (src - meanSrc)*(stdTar / stdSrc) + meanTar;
	cvtColor(dst, dst, CV_Lab2BGR);
	imshow("dst",dst);
}

void ColorAdjustment::getStdAndMean()
{
	Scalar allMean(0,0,0), allStdDev(0,0,0);
	int count = 0;
	for (int i = 3; i <= 3; i++)
	{
		char fileDir[300]; 
		//sprintf(fileDir, ".//source//%d.jpg", i);
		sprintf(fileDir, "test4.jpg");
		Mat img = imread(fileDir);
		//ImageToDouble(img, img);
		//DoubleNomolization(img);
		img.convertTo(img, CV_32FC3, 1.0 / 255.0);
		Mat lab;
		cvtColor(img, lab, CV_BGR2Lab);
		Scalar Mean, StdDev;
		meanStdDev(lab, Mean, StdDev);
		allMean += Mean;
		allStdDev += StdDev;
		count++;
	}
	allMean /= count;
	allStdDev /= count;
	std::cout << "allMean: "<<allMean[0] << " " << allMean[1] << " " << allMean[2]<< std::endl;
	std::cout << "allStdDev: "<<allStdDev[0] << " " << allStdDev[1] << " " << allStdDev[2]<<std::endl;
	MEAN = allMean;
	STDDEV = allStdDev;
	system("pause");
};