#include "ToolBox.h"
using namespace cv;
using namespace std;

//��pλ��[startX --- startX+lenX , startY --- startX+lenY]��
int ArraySpace::inMap(myPoint p,int lenX, int lenY,int startX ,int startY){
	return startX <= p.x&&p.x < startX + lenX && startY <= p.y&&p.y < startY + lenY;
}
//����Mat��Χ�ľ�����
int ArraySpace::inMap(Mat &a, myPoint p){
	return (0 <= p.x&&p.x < a.rows && 0 <= p.y&&p.y < a.cols);
}
void ArraySpace::checkMap(Mat &a, myPoint p){
	
	cout<<p.x<<" "<<p.y<<" "<<( 0 <= p.x&&p.x < a.rows && 0 <= p.y&&p.y < a.cols)<<endl;
}
Mat mul(const Mat &a, const Scalar &b){
	Mat c = a.clone();
	vector<Mat> clist;
	split(c, clist);
	for (int i = 0; i < 3; i++)
	{
		clist[i] = clist[i] * b[i];
	}
	merge(clist, c);
	return c;
}

//Mat::convertTo(*,*,alpha,beta)Ϊͳһ��alpha��beta��ת��
void ImageToDouble(const Mat &input, Mat &output)
{
	output = input.clone();
	output.convertTo(output, CV_32FC3, 1.0 / 255.0);
	return;
	output = input.clone();
	output.convertTo(output, CV_32FC3);
	double mymax;
	minMaxIdx(output, 0, &mymax);
	output = output / mymax;
}
void ArrayToDouble(const Mat &input, Mat &output)
{
	input.convertTo(output, CV_32F, 1.0 / 255.0);
	return;
	output = input.clone();
	output.convertTo(output, CV_32F);
	double mymax;
	minMaxIdx(output, 0, &mymax);
	output = output / mymax;
}

void DoubleToImage(const Mat &input, Mat &output)
{
	input.convertTo(output, CV_8UC3, 255.0);
	return;
	output = input.clone();
	double mymax;
	minMaxIdx(output, 0, &mymax);
	output = output / mymax * 255;
	output.convertTo(output, CV_8UC3);
}

void DoubleToArray(const Mat &input, Mat &output)
{
	input.convertTo(output, CV_8U, 255.0);
	return;
	output = input.clone();
	double mymax;
	minMaxIdx(output, 0, &mymax);
	output = output / mymax * 255;
	output.convertTo(output, CV_8U);
}

void DoubleNomolization(Mat &a)
{
	double mymax;
	minMaxIdx(a, 0, &mymax);
	a = a / mymax;
}

inline Scalar sqrt(const Scalar &a)
{
	return Scalar(sqrt(a[0]), sqrt(a[1]), sqrt(a[2]), sqrt(a[3]));
}

//ʹ��Mat::meanStdDev()
////float format
//Scalar getMean(Mat &a)
//{
//	Scalar x=Scalar::all(0);
//	for (int i = 0; i < a.rows; i++){
//		for (int j = 0; j < a.cols; j++){
//			//x = x+ Scalar(dataAt<Vec3f>(a,i, j));
//			x = x + Scalar(a.at<Vec3f>(i, j));
//		}
//	}
//	x = x / double(a.rows*a.cols);
//	return x;
//}

inline float sqr(float x)
{
	return x*x;
}

//ʹ��Mat::meanStdDev()
//Scalar getStdDev(Mat &a)
//{
//	Scalar myMean = getMean(a);
//	Scalar x = Scalar::all(0);
//	for (int i = 0; i < a.rows; i++){
//		for (int j = 0; j < a.cols; j++){
//			for (int k = 0; k < 3; k++){
//				x[k] += sqr(dataAt<Vec3f>(a,i, j)[k] - myMean[k]);
//			}
//		}
//	}
//	x = x / double(a.rows*a.cols);
//	return sqrt(x);
//}

