#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(){
	

//	//filter2D(img,)
//	Mat sobelx,sobely;
//	cvtColor(img, img, CV_RGB2GRAY);
//	Sobel(img, sobelx, CV_16S, 1, 0);
//	imshow("sobelx", sobelx);
//	Sobel(img, sobely, CV_16S, 1, 0);
//	imshow("sobely", sobely);
//	waitKey();

	
	
		//----------main------------
	string inName = "tt2.jpg";
	string inDir = "src/" + inName;
	string outName = "output.jpg";
	string outDir = "dst/" + outName;

	Mat input = imread(inDir),output;
	Debug() << "img size: " << input.rows << " " << input.cols;
	WaterColor watercolor;
	imwrite("process/src.jpg", input);
	watercolor.deal(input, output);
	imshow("src", input);
	imshow("dst", output);
	imwrite(outDir, output);
	imwrite("process/dst.jpg", output);
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
 