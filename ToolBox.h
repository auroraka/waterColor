#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "Debug.h"
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

struct myPoint{
	int x, y;
	myPoint(int x = 0, int y = 0) :x(x), y(y){};
	void print();
};


namespace ArraySpace{
	const int xo[] = { 1, -1, 0, 0 };
	const int yo[] = { 0, 0, 1, -1 };
	
	//新建二维数组并初始化
	template<typename T>
	T** newArray(int n, int m){
		T** a = new T*[n];
		for (int i = 0; i < n; i++) a[i] = new T[m]();
		return a;
	}

	//删除二维数组
	template<typename T>
	void deleteArray(T** a, int n, int m){
		for (int i = n - 1; i >= 0; i--) delete[] a[i];
		delete[] a;
	}

	//点p位于[startX,startY] --- (startX+lenX , startX+lenY)中,左上闭,右下开
	int inMap(myPoint p, int lenX, int lenY, int startX = 0, int startY = 0);
	int inMap(Mat &a, myPoint p);
	void checkMap(Mat &a, myPoint p);
}



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
T& dataAt(cv::Mat & src, int i, int j){
	return src.at<T>(i, j);
	T* curRow = src.ptr<T>(i);
	return *(curRow + j * src.channels());
}


//------------------------------------------------------FOR Debug----------------------------------------------------

//在控制台打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值
template<typename T>
void printMat(Mat &a, int lenX = 10, int lenY = 10,int startX=0,int startY=0)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(StdOut);
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() <<"from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]" ;
	for (int i = startX; i < startX+lenX; i++){
		Debug() << "("<<NoEnter;
		for (int j = startY; j < startY+lenY; j++){
			Debug() << int(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY-1) Debug() << ", " << NoEnter;
			//cout<<a.at<T>(i, j) << " ";
		}
		Debug()<<")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//在控制台打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,保留1位小数
template<typename T>
void printMatF(Mat &a, int lenX = 10, int lenY = 10, int startX = 0, int startY = 0, int digit = 1)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(StdOut);
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++){
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++){
			Debug()  << float(dataAt<T>(a, i, j))<<NoEnter;
			if (j != startY + lenY - 1) Debug() << ", " << NoEnter;
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}


//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat
template<typename T>
void writeMat(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0,string fileName="debug.txt")
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(FileOut);
	if (lenX == 0 && lenY == 0){
		lenX = 50;// a.rows;
		lenY = 50;// a.cols;
	}
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++){
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++){
			Debug() << int(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1) Debug() << ", " << NoEnter;
			//cout<<a.at<T>(i, j) << " ";
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//在文件打印Mat以(startX,startY)为起点,长宽为(lenX,lenY)范围的像素值,默认打印整个Mat,保留一位小数
#include <fstream>
template<typename T>
void writeMatF(Mat &a, int lenX = 0, int lenY = 0, int startX = 0, int startY = 0, string fileName = "debug.txt",int digit=1)
{
	DebugStatus temp = Debug().getStatus();
	Debug().setStatus(FileOut);
	if (lenX == 0 && lenY == 0){
		lenX = 50;// a.rows;
		lenY = 50;// a.cols;
	}
	Debug() << "Mat(" << a.rows << "," << a.cols << "):";
	Debug() << "from [" << startX << "," << startY << "] to [" << startX + lenX - 1 << "," << startY + lenY - 1 << "]";
	for (int i = startX; i < startX + lenX; i++){
		Debug() << "(" << NoEnter;
		for (int j = startY; j < startY + lenY; j++){
			Debug() << float(dataAt<T>(a, i, j)) << NoEnter;
			if (j != startY + lenY - 1) Debug() << ", " << NoEnter;
		}
		Debug() << ")";
	}
	Debug() << "";
	Debug().setStatus(temp);
}

//----------------------------------------------------END FOR Debug---------------------------------------------------

#endif