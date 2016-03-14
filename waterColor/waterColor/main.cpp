#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(){

	//file dir
	string inName = "horse.jpg";
	string inDir = "picture/" + inName;
	string outName = "horse-dst.png";
	string outDir = "picture/" + outName;

	//read image
	Mat input = imread(inDir),output;
	Debug() << "img size: " << input.rows << " " << input.cols;
	
	//watercolor process
	WaterColor watercolor;
	watercolor.deal(input, output);//deal with watercolor
	imwrite(outDir, output);//store dst picture
	
	//show result
	imshow("src", input);
	imshow("dst", output);
	waitKey();


		
}
 