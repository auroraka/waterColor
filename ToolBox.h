#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace std;

//*************************************
//特别注意
//1.Scalar的 / 重载是有问题的, 要用自己重写的div(),
//2.Mat的 * / 为矩阵操作,要用Mat::mul(),Mat::div()
//***************************************




//彩图double类型,maxVal(255->1)
void ImageToDouble(const Mat &input, Mat &output);

//黑白图像转double类型,maxVal(255->1)
void ArrayToDouble(const Mat &input, Mat &output);

//double类型转黑白图,maxVal(1->255)
void DoubleToImage(const Mat &input, Mat &output);

//double类型转彩图,maxVal(1->255)
void DoubleToArray(const Mat &input, Mat &output);

//数组归一化
void DoubleNomolization(Mat &a);

////图像的均值
//Scalar getMean(Mat &a);
//
////图像标准差
//Scalar getStdDev(Mat &a);


//a的每一个通道分别乘以b对应的值
Mat mul(const Mat &a, const Scalar &b);

//mul
inline Scalar mul(const Scalar &a, const Scalar &b){
	return Scalar(a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3]);
}

//div
inline Scalar div(const Scalar &a, const Scalar &b){
	return Scalar(a[0] / b[0], a[1] / b[1], a[2] / b[2], a[3] / b[3]);
}

//sqr
inline float sqr(float x);

//sqrt
inline Scalar sqrt(const Scalar &a);



//图像某一点的像素值(比Mat::at()快)  eg. dataAt<Vec3b>(img,3,5)
template<typename T>
T dataAt(cv::Mat & src, int i, int j){
	return src.at<T>(i, j);
	T* curRow = src.ptr<T>(i);
	return *(curRow + j * src.channels());
}

//在控制台打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值
template<typename T>
void printMat(Mat &a, int lenX = 10, int lenY = 10,int startX=0,int startY=0)
{
	for (int i = startX; i < startX+lenX; i++){
		for (int j = startY; j < startY+lenY; j++){
			cout << dataAt<T>(a, i, j) << " ";
			//cout<<a.at<T>(i, j) << " ";
		}
		cout << endl;
	}
}

//在控制台打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,保留1位小数
template<typename T>
void printMatF(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0,int digit=1)
{
	for (int i = startX; i < startX + lenX; i++){
		for (int j = startY; j < startY + lenY; j++){
			cout << setprecision(digit)<<fixed << dataAt<T>(a, i, j) << " ";
		}
		cout << endl;
	}
}


//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat
template<typename T>
void writeMat(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0,string fileName="debug.txt")
{
	ofstream fout(fileName);
	if (lenX == 0 && lenY == 0){
		lenX = a.rows;
		lenY = a.cols;
	}
	for (int i = startX; i < startX + lenX; i++){
		for (int j = startY; j < startY + lenY; j++){
			fout << dataAt<T>(a, i, j) << " ";
		}
		fout << endl;
	}
	fout.close();
}

//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat,保留一位小数
#include <fstream>
template<typename T>
void writeMatF(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "debug.txt",int digit=1)
{
	ofstream fout(fileName);
	if (lenX == 0 && lenY == 0){
		lenX = a.rows;
		lenY = a.cols;
	}
	char info[100];
	for (int i = startX; i < startX + lenX; i++){
		for (int j = startY; j < startY + lenY; j++){
			fout << setprecision(digit) << fixed << dataAt<T>(a, i, j) << " ";
		//	fout << setprecision(digit) << fixed << a.at<T>(i,j) << " ";
		}
		fout << endl;
	}
	fout.close();
}



#endif