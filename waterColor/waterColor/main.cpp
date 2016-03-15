#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(int argc,char *argv[]){

	//file dir
	string inName = "src-small.jpg";
	string inDir = "src/" + inName;
	string outName = "output.png";
	string outDir = "dst/" + outName;
	if (argc == 3){
		inDir = string(argv[1]);
		outDir = string(argv[2]);
	}

	//read picture
	Mat input = imread(inDir),output;
	Debug() << "img size: " << input.rows << " " << input.cols;
	
	//process
	WaterColor watercolor;
	imwrite("process/src.jpg", input);
	watercolor.deal(input, output);
	
	//show & store result
	imshow("src", input);
	imshow("dst", output);
	imwrite(outDir, output);
	imwrite("process/dst.png", output);
	waitKey();
}
 