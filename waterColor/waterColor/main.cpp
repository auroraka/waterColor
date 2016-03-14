#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(){
//	Mat a = Mat::zeros(300, 300, CV_8U);
//	int i = 50;
//	for (int j = 1; j <= 100; j++){
//	//	Debug() << i << ": " << 5 * sin(i / 5.0);
//		int w = 3 * sin(j / 8.0);
//		if (w < 0) for (int x = i + w; x <= i; x++) dataAt<uchar>(a, x,j) = 255;
//		if (w > 0) for (int x = i; x <= i+w; x++) dataAt<uchar>(a,  x,j) = 255;
//	}
//	imshow("aaa", a);
//	waitKey();
//	//Debug().pause();
//	return 0;
		

//	//filter2D(img,)
//	Mat sobelx,sobely;
//	cvtColor(img, img, CV_RGB2GRAY);
//	Sobel(img, sobelx, CV_16S, 1, 0);
//	imshow("sobelx", sobelx);
//	Sobel(img, sobely, CV_16S, 1, 0);
//	imshow("sobely", sobely);
//	waitKey();

	
	
		//----------main------------
	string inName = "src.jpg";
	string inDir = "src/" + inName;
	string outName = "output.png";
	string outDir = "dst/" + outName;

	Mat input = imread(inDir),output;
	Debug() << "img size: " << input.rows << " " << input.cols;
	WaterColor watercolor;
	imwrite("process/src.jpg", input);
	watercolor.deal(input, output);
	imshow("src", input);
	imshow("dst", output);
	imwrite(outDir, output);
	imwrite("process/dst.png", output);
	waitKey();


		
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
 