#include "WaterColor.h"
#include <opencv2/opencv.hpp>
using namespace cv;

int main(){
	Mat input = imread("test3.jpg"),output;
	WaterColor watercolor;
	watercolor.deal(input, output);
	imwrite("output.jpg", output);
	system("pause");
	Scalar a;
	a.hue
}