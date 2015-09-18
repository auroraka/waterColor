#include "Abstraction.h"
#include "ToolBox.h"
#include "include\meanshift\MeanShift.h"
using namespace cv;


void meanShift(Mat &src, Mat &dst)
{
	imwrite("src.jpg", src);
	dst = Mat(src.size(), CV_8U);
	IplImage *img = cvLoadImage("src.jpg");
	// Mean shift
	int **ilabels = new int *[img->height];
	for (int i = 0; i < img->height; i++) ilabels[i] = new int[img->width];
	int regionCount = MeanShift(img, ilabels);

	vector<int> color(regionCount);
	vector<int> mcolor(regionCount);
	CvRNG rng = cvRNG(cvGetTickCount());
	int count = 0;
	for (int i = 0; i < regionCount; i++){
		color[i] = ++count;
		mcolor[i] = cvRandInt(&rng);
	}
	
	// Draw color
	for (int i = 0; i<img->height; i++)
		for (int j = 0; j<img->width; j++)
		{
			int cl = ilabels[i][j];
			dataAt<uchar>(dst, i, j) = color[cl];
			
			//--Debug-use--
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = (mcolor[cl]) & 255;
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = (mcolor[cl] >> 8) & 255;
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = (mcolor[cl] >> 16) & 255;
			//--------
		}
	imwrite("process/meanshift.jpg", Mat(img));
	//dst=Mat(img);
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
	Scalar ans = div(sum, float(count));
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
	Scalar ans = div(sum, float(count));
	//Debug() << ans;
	//Debug().pause();
	return Vec3b(ans[0], ans[1], ans[2]);
}
//void Abstraction::deal(Mat &src, Mat &mySaliency, Mat &myDis, Mat &dst)
//{
//	Debug().setStatus(StdOut);
//	dst = src.clone();
//	Mat myMeanShift;
//	Debug() << "	mean shift...";
//	meanShift(src,myMeanShift);
//	//imshow("meanshift", myMeanShift);
//	
//	Debug() << "	apply mean filter...";
//	for (int i = 0; i < src.rows; i++){
//		for (int j = 0; j < src.cols; j++){
//			//ver 1.0
////			//inside the saliency region
////			if (dataAt<uchar>(mySaliency, i, j)>0){
////				dataAt<Vec3b>(dst,i,j)=getInMean(dst, myMeanShift,i, j);
////			} else{//outside the saliency region
////				/*if (haveColorDifference(myDis, myMeanShift,i,j)){
////					float d = dataAt<float>(myDis, i, j);
////					int size = clamp(5 * 2 * (d + 0.3), 4, 9) ;///
////					dataAt<Vec3b>(dst, i, j) = getOutMean(dst, i, j, size);
////				}*/
////				float d = dataAt<float>(myDis, i, j);
////				int size = clamp(5 * 2 * (d + 0.3), 4, 9);///
////				dataAt<Vec3b>(dst, i, j) = getOutMean(dst,myMeanShift, i, j, size);
////			}
//				
//			//ver 2.0
//			
//			//if in the same color region
//			int size1 = 5/2;///
//			int count = 0;
//			Scalar sum = Scalar::all(0);
//			for (int x = i - size1; x <= i + size1; x++){
//				for (int y = j - size1; y <= j + size1; y++) if (ArraySpace::inMap(src,x,y)){
//					if (dataAt<uchar>(myMeanShift, x, y) == dataAt<uchar>(myMeanShift, i, j)){
//						sum += Scalar(dataAt<Vec3b>(src, x, y));
//						count++;
//					}
//				}
//			}
//
//			//not in the detect region & 
//			if (dataAt<uchar>(mySaliency, i, j) == 0){
//				double d = dataAt<float>(myDis, i, j);
//				int size2 = clamp(5 * 2 * (d + 0.3), 4, 9)/2;///
//
//				for (int x = i - size2; x <= i + size2; x++){
//					for (int y = j - size2; y <= j + size2; y++) if (ArraySpace::inMap(src, myPoint(x, y))){
//						if (dataAt<uchar>(myMeanShift, x, y) != dataAt<uchar>(myMeanShift, i, j)
//							&& abs(dataAt<float>(myDis,x,y)-dataAt<float>(myDis,i,j)) < 0.3*d ){
//							
//							sum += Scalar(dataAt<Vec3b>(src, x,y));
//							count++;
//						}
//					}
//				}
//
//			}
//
//			Scalar ans = div(sum, float(count));
//			//Debug() <<"ans:"<< ans<<" "<<"sum: "<<sum<<count;
//			//Debug().pause();
//			dataAt<Vec3b>(dst, i, j) = Vec3b(ans[0], ans[1], ans[2]);
//		}
//	}
//	Debug() << "	complete";
//}



void Abstraction::deal(Mat &src, Mat &mySaliency, Mat &myDis, Mat &dst)
{
	Debug().setStatus(StdOut);
	dst = src.clone();
	Mat myMeanShift;
	Debug() << "	mean shift...";
	meanShift(src, myMeanShift);
	exit(0);
	//imshow("meanshift", myMeanShift);
	Debug() << "	apply mean filter...";
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){

			//if in the same color region
			int size1 = 5 / 2;///
			int count = 0;
			Scalar sum = Scalar::all(0);
			for (int x = i - size1; x <= i + size1; x++){
				for (int y = j - size1; y <= j + size1; y++) if (ArraySpace::inMap(src, x, y)){
					if (dataAt<uchar>(myMeanShift, x, y) == dataAt<uchar>(myMeanShift, i, j)){
						sum = sum + Scalar(dataAt<Vec3b>(src, x, y));
						count++;
					}
				}
			}


			Scalar ans = div(sum, float(count));
			Debug() << "ans:" << ans << " " << "sum: " << sum << count << "  origin: " << dataAt<Vec3b>(dst, i, j);
			//Debug().pause();
			dataAt<Vec3b>(dst, i, j) = Vec3b(ans[0], ans[1], ans[2]);
		}
	}
	/*for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){

			//if in the same color region
	
			//not in the detect region & 
			if (dataAt<uchar>(mySaliency, i, j) == 0){

				int size1 = 5 / 2;///
				int count = 0;
				Scalar sum = Scalar::all(0);

				double d = dataAt<float>(myDis, i, j);
				int size2 = clamp(5 * 2 * (d + 0.3), 4, 9) / 2;///

				for (int x = i - size2; x <= i + size2; x++){
					for (int y = j - size2; y <= j + size2; y++) if (ArraySpace::inMap(src, myPoint(x, y))){
						if (dataAt<uchar>(myMeanShift, x, y) != dataAt<uchar>(myMeanShift, i, j)
							&& abs(dataAt<float>(myDis, x, y) - dataAt<float>(myDis, i, j)) < 0.3*d){

							sum += Scalar(dataAt<Vec3b>(src, x, y));
							count++;
						}
					}
				}
				Scalar ans = div(sum, float(count));
				//Debug() <<"ans:"<< ans<<" "<<"sum: "<<sum<<count;
				//Debug().pause();
				dataAt<Vec3b>(dst, i, j) = Vec3b(ans[0], ans[1], ans[2]);


			}


		}
	}
*/


	Debug() << "	complete";
}