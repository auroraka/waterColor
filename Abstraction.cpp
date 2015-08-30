#include "Abstraction.h"
using namespace cv;

void Abstraction::deal(const Mat &input, Mat &output)
{
	output = input.clone();

}