#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
using namespace cv;
using namespace std;

//*************************************
//�ر�ע��
//1.Scalar�� / �������������, Ҫ���Լ���д��div(),
//2.Mat�� * / Ϊ�������,Ҫ��Mat::mul(),Mat::div()
//***************************************




//��ͼdouble����,maxVal(255->1)
void ImageToDouble(const Mat &input, Mat &output);

//�ڰ�ͼ��תdouble����,maxVal(255->1)
void ArrayToDouble(const Mat &input, Mat &output);

//double����ת�ڰ�ͼ,maxVal(1->255)
void DoubleToImage(const Mat &input, Mat &output);

//double����ת��ͼ,maxVal(1->255)
void DoubleToArray(const Mat &input, Mat &output);

//�����һ��
void DoubleNomolization(Mat &a);

////ͼ��ľ�ֵ
//Scalar getMean(Mat &a);
//
////ͼ���׼��
//Scalar getStdDev(Mat &a);


//a��ÿһ��ͨ���ֱ����b��Ӧ��ֵ
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



//ͼ��ĳһ�������ֵ(��Mat::at()��)  eg. dataAt<Vec3b>(img,3,5)
template<typename T>
T dataAt(cv::Mat & src, int i, int j){
	return src.at<T>(i, j);
	T* curRow = src.ptr<T>(i);
	return *(curRow + j * src.channels());
}

//�ڿ���̨��ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ
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

//�ڿ���̨��ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,����1λС��
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


//���ļ���ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,Ĭ�ϴ�ӡ����Mat
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

//���ļ���ӡMat��(startX,startY)Ϊ���,����Ϊ(lenX,lenY)��Χ������ֵ,Ĭ�ϴ�ӡ����Mat,����һλС��
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