#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>
using namespace cv;


int main(){

		//----------main------------
	string inName = "src-small.jpg";
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

}
 