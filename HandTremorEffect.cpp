#include "HandTremorEffect.h"
#include "ToolBox.h"
using namespace cv;

//bool colorSame(Mat &a, myPoint p1, myPoint p2){
//	Vec3b x1 = dataAt<Vec3b>(a, p1.x, p1.y);
//	Vec3b x2 = dataAt<Vec3b>(a, p2.x, p2.y);
//	return abs(x1 - x2) < Vec3b(30, 30,30);
//}
//bool colorDifferent(Mat &a, myPoint p1, myPoint p2){
//	Vec3b x1 = dataAt<Vec3b>(a, p1.x, p1.y);
//	Vec3b x2 = dataAt<Vec3b>(a, p2.x, p2.y);
//	//Debug() << x1 << " " << x2 <<" "<<x1-x2;
//	return abs(x1 - x2) > Vec3b(20,20,20);
//}
////横向
//bool sameInRow(Mat &a,int x,int y){
//	for (int i = y - 4; i <= y + 4; i++) 
//		for (int j = i + 1; j <= y + 4; j++) if (ArraySpace::inMap(a,x, i) && ArraySpace::inMap(a,x, j)){
//			if (colorSame(a, myPoint(x, i), myPoint(x, j))) return true;
//		}
//	return false;
//}
////横向
//bool differentInRow(Mat &a, int x, int y){
//	return true;
//	for (int i = y - 2; i <= y + 2; i++)
//		for (int j = i + 1; j <= y + 2; j++) if (ArraySpace::inMap(a, x, i) && ArraySpace::inMap(a, x, j)){
//			if (colorDifferent(a, myPoint(x, i), myPoint(x, j))) return true;
//		}
//	return false;
//}
////纵向
//bool sameInCol(Mat &a, int x, int y){
//	for (int i = x - 2; i <= x + 2; i++)
//		for (int j = i + 1; j <= x + 2; j++) if (ArraySpace::inMap(a, i,y) && ArraySpace::inMap(a,  j,y)){
//			if (colorSame(a, myPoint(i,y), myPoint( j,y))) return true;
//		}
//	return false;
//}
////纵向
//bool differentInCol(Mat &a, int x, int y){
//	for (int i = x - 3; i <= x + 3; i++)
//		for (int j = i + 1; j <= x + 3; j++) if (ArraySpace::inMap(a, i, y) && ArraySpace::inMap(a, j, y)){
//			if (colorDifferent(a, myPoint(i, y), myPoint(j, y))) return true;
//		}
//	return false;
//}
//
Vec3b getMean(Mat &a, int x, int y, int l = 5){
	Scalar sum = Scalar::all(0);
	int tot = 0;
	for (int i = x - l; i <= x + 5; i++)
		for (int j = y - l; j <= y + l; j++){
			tot++;
			Vec3b tt = dataAt<Vec3b>(a, i, j);
			sum[0] += tt[0];
			sum[1] += tt[1];
			sum[2] += tt[2];
		}
	return Vec3b(sum[0] / tot, sum[1] / tot, sum[2] / tot);
}
void smooth(Mat &a,Mat &mask){
	Mat b = a.clone();
	for (int i = 0; i < a.rows; i++)
		for (int j = 0; j < a.cols; j++) if (ArraySpace::inMap(a,i,j))
			if (dataAt<uchar>(mask, i, j) != 0){
				dataAt<Vec3b>(b, i, j) = getMean(a, i, j,5);
			}
	a = b.clone();
}
Vec3b getUpMean(Mat &src,int i,int j){
	Scalar sum = Scalar::all(0); int tot = 0;
	for (int x = i - 1; x <= i + 1; x++)
		for (int y = j - 5; y <= j; y++) if (ArraySpace::inMap(src, i, j)){
			tot++;
			Vec3b tt = dataAt<Vec3b>(src, i, j);
			sum[0] += tt[0];
			sum[1] += tt[1];
			sum[2] += tt[2];
		}
	return Vec3b(sum[0] / tot, sum[1] / tot, sum[2] / tot);
}
Vec3b getDownMean(Mat &src, int i, int j){
	Scalar sum = Scalar::all(0); int tot = 0;
	for (int x = i - 1; x <= i + 1; x++)
		for (int y = j; y <= j+5; y++) if (ArraySpace::inMap(src, i, j)){
			tot++;
			Vec3b tt = dataAt<Vec3b>(src, i, j);
			sum[0] += tt[0];
			sum[1] += tt[1];
			sum[2] += tt[2];
		}
	return Vec3b(sum[0] / tot, sum[1] / tot, sum[2] / tot);
}
Vec3b getLeftMean(Mat &src, int i, int j){
	Scalar sum = Scalar::all(0); int tot = 0;
	for (int x = i -5; x <= i ; x++)
		for (int y = j-1; y <= j + 1; y++) if (ArraySpace::inMap(src, i, j)){
			tot++;
			Vec3b tt = dataAt<Vec3b>(src, i, j);
			sum[0] += tt[0];
			sum[1] += tt[1];
			sum[2] += tt[2];
		}
	return Vec3b(sum[0] / tot, sum[1] / tot, sum[2] / tot);
}
Vec3b getRightMean(Mat &src, int i, int j){
	Scalar sum = Scalar::all(0); int tot = 0;
	for (int x = i; x <= i+5; x++)
		for (int y = j - 1; y <= j + 1; y++) if (ArraySpace::inMap(src, i, j)){
			tot++;
			Vec3b tt = dataAt<Vec3b>(src, i, j);
			sum[0] += tt[0];
			sum[1] += tt[1];
			sum[2] += tt[2];
		}
	return Vec3b(sum[0] / tot, sum[1] / tot, sum[2] / tot);
}
bool isSimilar(const Vec3b &a, const Vec3b &b){
	return abs(a - b) < Vec3b(30, 30,30);
}
bool isDifferent(const Vec3b &a,const Vec3b &b){
	return abs(a - b) > Vec3b(20,20,20);
}
bool checkInRow(Mat &src, int i, int j){
	Vec3b a = getUpMean(src, i, j);
	Vec3b b = getDownMean(src, i, j);
	Vec3b c = getLeftMean(src, i, j);
	Vec3b d = getRightMean(src, i, j);
	return isSimilar(c, d) && isDifferent(a, b);
}
bool checkInCol(Mat &src, int i, int j){
	Vec3b a = getUpMean(src, i, j);
	Vec3b b = getDownMean(src, i, j);
	Vec3b c = getLeftMean(src, i, j);
	Vec3b d = getRightMean(src, i, j);
	return isSimilar(c, d) && isDifferent(a, b);
}
void HandTremorEffect::deal(Mat &src, Mat &dst, Mat &myCanny)
{
	Mat msk = Mat::zeros(src.size(), CV_8U);
	dst = src.clone();
	Mat debug = dst.clone();

	//横向
	for (int i = 1; i < src.rows-1; i++){
		int last = 0;
		for (int j = 1; j < src.cols-1; j++) {
			int w = last; int flag = 0;
			if (dataAt<uchar>(myCanny, i, j) != 0 /*&& checkInRow(src,i,j)*/){
				//cout << "!!!";
				int tt = round(3 * sin(j / 18.0));
				if (tt > w) w=last+1;
				else if (tt == w) w = last;
				else if (tt < w) w = last - 1;
				flag = 1;
			}
			if (!flag){
				if (w > 0) w--;
				if (w < 0) w++;
			}
			last = w;
			if (w<0){		
				int l = max(0, i + w), r = i;
				//Debug() << l << " " << r;
//				for (int x = l; x <= r; x++) dataAt<Vec3b>(dst, x, j) = dataAt<Vec3b>(src, min(r + x - l, src.rows - 1), j);
//				for (int x = l; x <= r; x++) dataAt<Vec3b>(debug, x, j) = Vec3b(0, 0, 0);// dataAt<Vec3b>(src, min(r + x - l, src.rows - 1), j);
				//Debug().setStatus(FileOut);
				//Debug() << l << " " << r;
				for (int x = l; x <= r; x++){
					if (ArraySpace::inMap(msk, x, j)){
						//Debug() << "Error";
						msk.at<uchar>(x, j) = 2;// dataAt<uchar>(msk, x, j) = 0;
					}
				}
			}
//			if (w>0){
//				int l = i, r = min(i + w, src.rows - 1);
//				for (int x = l; x <= r; x++) dataAt<Vec3b>(dst, x, j) = dataAt<Vec3b>(src, max(0, l - x + r), j);
//				for (int x = l; x <= r; x++) dataAt<Vec3b>(debug, x, j) = Vec3b(0, 0, 0);// dataAt<Vec3b>(src, max(0, l - x + r), j);
				//for (int x = max(0, l - 2); x <= r; x++) msk.at<uchar>(x, j) = 1;//dataAt<uchar>(mask, x, j) = 1;
//			}
		}
	}
	imwrite("process/handtremor-mask.jpg", msk * 255);
	smooth(dst,msk);
	//纵向
	imwrite("process/handtremor-debug.jpg", debug);
}
//void HandTremorEffect::deal(Mat &src, Mat &dst,Mat &myCanny)
//{
//	dst = src.clone();
//	Mat debug = dst.clone();
//	for (int i = 1; i < src.rows - 1; i++){
//		int last = 0;
//		for (int j = 1; j < src.cols - 1; j++){
//			if (dataAt<uchar>(myCanny, i, j) != 0){
//				//横向
//				if (sameInRow(src, i, j) && differentInCol(src, i, j)){
//					int w = round(2 * sin(j / 18.0));
//					if (w < 0){
//						int l = max(0, i + w), r = i;
//						for (int x = l + 1; x <= r; x++) dataAt<Vec3b>(dst, x, j) = dataAt<Vec3b>(src, min(i + 1, src.rows - 1), j);
//						dataAt<Vec3b>(dst, l, j) = mean(dataAt<Vec3b>(src, min(i + 1, src.rows - 1), j), dataAt<Vec3b>(src, l, j));
//					}
//					if (w > 0){
//						int l = i, r = min(i + w, src.rows - 1);
//						for (int x = l; x <= r - 1; x++) dataAt<Vec3b>(dst, x, j) = dataAt<Vec3b>(src, max(0, i - 1), j);
//						dataAt<Vec3b>(dst, r, j) = mean(dataAt<Vec3b>(src, max(0, i - 1), j), dataAt<Vec3b>(src, r, j));
//					}
//					//debug
//					if (w <= 0) for (int x = max(0, i + w); x <= i; x++) dataAt<Vec3b>(debug, x, j) = Vec3b(0, 0, 0); //dataAt<Vec3b>(src, i, j + 1);
//					if (w > 0) for (int x = i; x <= min(i + w, src.rows - 1); x++) dataAt<Vec3b>(debug, x, j) = Vec3b(0, 0, 0); //dataAt<Vec3b>(src, i, j - 1);
//				}
//
//
//				//纵向
//				if (sameInCol(src, i, j) && differentInRow(src, i, j)){
//					int w = round(2 * sin(i / 18.0));
//					if (w < 0){
//						int l = max(0, j + w), r = j;
//						for (int x = l+1; x <= j; x++) dataAt<Vec3b>(dst, i, x) = dataAt<Vec3b>(src, i, min(j + 2, src.cols - 1));
//						dataAt<Vec3b>(dst, i, l) = mean(dataAt<Vec3b>(src, i, l),dataAt<Vec3b>(src, i, min(j + 2, src.cols - 1)));
//					}
//					if (w > 0){
//						int l = j, r = min(j + w, src.cols - 1);
//						for (int x = j; x <= r-1; x++) dataAt<Vec3b>(dst, i, x) = dataAt<Vec3b>(src, i, max(0, j - 1));
//						dataAt<Vec3b>(dst, i, r - 1) = mean(dataAt<Vec3b>(src, i, r - 1),dataAt<Vec3b>(src, i, max(0, j - 1)));
//					}
//					//debug
//					if (w < 0) for (int x = max(0, j + w); x <= j; x++) dataAt<Vec3b>(debug, i, x) = Vec3b(0, 0, 0);// dataAt<Vec3b>(src, i + 1, x);
//					if (w > 0) for (int x = j; x <= min(j + w, src.cols - 1); x++) dataAt<Vec3b>(debug, i, x) = Vec3b(0, 0, 0); //dataAt<Vec3b>(src, i - 1, x);
//				}
//			}
//		}
//	}
//	imwrite("process/handtremor-debug.jpg", debug);
//}