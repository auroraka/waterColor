#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(){


	//----------main------------
		Mat input = imread("test.jpg"),output;
		WaterColor watercolor;
		imshow("src", input);
		watercolor.deal(input, output);
		imshow("dst", output);
		waitKey();
		imwrite("output.jpg", output);
		

////    DEBUG TEST-----------------
//	Debug::Status = StdOut;
//
//	Mat a(3, 3, CV_8U);
//	for (int i = 0; i < 3; i++){
//		for (int j = 0; j < 3; j++){
//			dataAt<uchar>(a, i, j) = i * 10 + j;
//		}
//	}
//	printMatF<uchar>(a, 3, 3, 0, 0);
//	Debug() << 3;
//	Debug() << 1 << 2;
//	Debug::pause();

////	TEST----------------
//	Mat test(100, 100, CV_32FC3);
//	int count = 0;
//	for (int i = 0; i < 100; i++)
//		for (int j = 0; j < 100; j++)
//			
//			dataAt<Vec3f>(test,i, j) = Vec3f(i, j, count++);
//			//test.at<Vec3f>(i, j) = Vec3f(i, j, count++);
//
//	writeMatF<Vec3f>(test);
}
