#include "Abstraction.h"
#include "ToolBox.h"
#include "Debug.h"
#include "include\meanshift\MeanShift.h"
using namespace cv;


//ver 3.0
void meanShift(Mat &src, Mat &dst)
{
	pyrMeanShiftFiltering(src, dst, 40,40,3);
	RNG rng = theRNG();
	Mat mask(dst.rows + 2, dst.cols + 2, CV_8UC1, Scalar::all(0));
	for (int i = 0; i<dst.rows; i++)    //opencv图像等矩阵也是基于0索引
		for (int j = 0; j<dst.cols; j++)
			if (mask.at<uchar>(i + 1, j + 1) == 0)
			{
				Scalar newcolor(rng(256), rng(256), rng(256));
				floodFill(dst, mask, Point(j, i), newcolor, 0, Scalar::all(5), Scalar::all(5));
			}
	imwrite("process/meanshift.jpg", dst);
}



Vec3b getInMean(Mat &src,Mat &meanshift, int x, int y){
	int count = 0;
	int size = 2;///
	Scalar sum = Scalar::all(0);
	for (int i = x - size; i <= x + size; i++){
		for (int j = y - size; j <= y + size; j++){
			if (dataAt<uchar>(meanshift, x, y) == dataAt<uchar>(meanshift, i, j)){
				sum += Scalar(dataAt<Vec3b>(src, i, j));
				count++;
			}
		}
	}
	Scalar ans = divVec(sum, float(count));
	//Debug() <<"ans:"<< ans<<" "<<"sum: "<<sum<<count;
	//Debug().pause();
	return Vec3b(ans[0],ans[1],ans[2]);
}
int clamp(int a, int x, int y){
	if (a < x) return x;
	if (a > y) return y;
	return a;
}
bool haveColorDifference(Mat &dis,Mat &meanshift,int x,int y){
	float d = dataAt<float>(dis, x, y);
	int size = clamp(5 * 2 * (d + 0.3), 4, 9) ;///
	for (int i = x - size; i <= x + size; i++){
		for (int j = y - size; j <= y + size; j++) if (ArraySpace::inMap(dis,myPoint(i,j))){
			if (dataAt<uchar>(meanshift, x, y) != dataAt<uchar>(meanshift, i, j) &&
				fabs(dataAt<float>(dis, i, j) - d) < 0.3*d){
				
				return true;
			}
		}
	}
	return false;
}
Vec3b getOutMean(Mat &src, Mat& myMeanShift,int x, int y, int size){
	Scalar sum = Scalar::all(0);
	int count = 0;
	for (int i = x - size; i <= x + size; i++){
		for (int j = y - size; j <= y + size; j++) if (ArraySpace::inMap(src, myPoint(i, j))){
			if (dataAt<uchar>(myMeanShift, x, y) == dataAt<uchar>(myMeanShift, i, j)){
				sum += Scalar(dataAt<Vec3b>(src, i, j));
				count++;
			}
		}
	}
	Scalar ans = divVec(sum, float(count));
	//Debug() << ans;
	//Debug().pause();
	return Vec3b(ans[0], ans[1], ans[2]);
}

//ver 3.0
void Abstraction::deal(Mat &src, Mat &mySaliency, Mat &myDis, Mat &dst)
{
	Mat ref = src.clone();

	Debug().setStatus(StdOut);
	dst = src.clone();
	Mat myMeanShift;
	Debug() << "	mean shift...";
	meanShift(src, myMeanShift);
	//imshow("meanshift", myMeanShift);
	Debug() << "	apply mean filter...";
	
	for (int i = 0; i < ref.rows; i++){
		for (int j = 0; j < ref.cols; j++){

			//if in the same color region
			//if (dataAt<uchar>(mySaliency, i, j) != 0){
				int size1 = 10 / 2;///
				int count = 0;
				Scalar sum = Scalar::all(0);
				for (int x = i - size1; x <= i + size1; x++){
					for (int y = j - size1; y <= j + size1; y++) if (ArraySpace::inMap(ref, x, y)){
						if (dataAt<Vec3b>(myMeanShift, x, y) == dataAt<Vec3b>(myMeanShift, i, j)){
							sum = sum + Scalar(dataAt<Vec3b>(ref, x, y));
							count++;
						}
					}
				}
				if (dataAt<uchar>(mySaliency, i, j) == 0){
					double d = dataAt<float>(myDis, i, j);
					int size2 = clamp(5 * 2 * (d + 0.3), 4, 9) / 2;///
					for (int x = i - size2; x <= i + size2; x++){
						for (int y = j - size2; y <= j + size2; y++) if (ArraySpace::inMap(ref, myPoint(x, y))){
							if (dataAt<uchar>(myMeanShift, x, y) != dataAt<uchar>(myMeanShift, i, j)
								&& abs(dataAt<float>(myDis, x, y) - dataAt<float>(myDis, i, j)) < 0.3*d){

								sum += Scalar(dataAt<Vec3b>(ref, x, y));
								count++;
							}
						}
					}
				}

			Scalar ans = divVec(sum, float(count));
			dataAt<Vec3b>(dst, i, j) = Vec3b(ans[0], ans[1], ans[2]);
		}
	}

	Debug() << "	complete";
}