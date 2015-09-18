#include "WetInWet.h"
#include "ToolBox.h"
using namespace cv;

void getCanny(Mat &src, Mat &dst){
	Mat img = src;
	Mat imgCanny;
	Mat imgGray;
	cvtColor(img, imgGray, CV_RGB2GRAY);
	Mat imgBlur;
	blur(imgGray, imgBlur, Size(5, 5));
	Canny(imgBlur, imgCanny, 80, 120);
	dst = imgCanny;
	imwrite("process/canny.jpg", imgCanny);
}
void getSobel(Mat &src,Mat &dst){

	Mat src_gray;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert it to gray
	cvtColor(src, src_gray, CV_RGB2GRAY);

	/// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
//	writeMat<short>(grad_x, 50, 50);

	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);

	for (int i = 0; i < dst.rows; i++)
		for (int j = 0; j < dst.cols; j++){
			if (dataAt<uchar>(dst, i, j)<50){
				dataAt<uchar>(dst, i, j) = 0;
			} else{
				dataAt<uchar>(dst, i, j) = 255;
			}
		}
	imwrite("process/sobel.jpg", dst);
}

bool randomPoint(myPoint &p,int x, int y,Mat &src,int kernel){
	if (rand(3)) return false;
	p = myPoint(rand(x - kernel, x + kernel), rand(y - kernel, y + kernel));
	if (!ArraySpace::inMap(src, p)) return false;
	return true;
}
myPoint getDarkestPoint(int x,int y,Mat &gray,int kernel){
	myPoint rec;
	int val = 300;
	for (int i = x - kernel; i <= x + kernel; i++)
		for (int j = y - kernel; j <= y + kernel; j++) if (ArraySpace::inMap(gray,myPoint(i,j))){
			if (dataAt<uchar>(gray, i, j)<val){
				val = dataAt<uchar>(gray, i, j);
				rec = myPoint(i, j);
			}
		}
	return rec;
}
void WetInWet::deal(Mat &src,Mat &myAbstraction ,Mat &dst)
{
	int n = src.rows, m = src.cols;
	Mat myCanny, gray,mask = Mat::zeros(src.size(), CV_8U);
	dst = myAbstraction.clone();
	Debug() << "	canny...";
	getCanny(src,myCanny);
	cvtColor(src, gray, CV_RGB2GRAY);
	myPoint** nearest = ArraySpace::newArray<myPoint>(n, m);
	myPoint* Q = new myPoint[n*m + 5];
	int **visit = ArraySpace::newArray<int>(n, m);
	
	{//getRandom
		for (int i = 0; i < n; i++){
			for (int j = 0; j < m; j++){
				if (dataAt<uchar>(myCanny, i, j) != 0){
					int kernel = 5;
					myPoint p2;
					if (randomPoint(p2,i, j, src, kernel)){
						myPoint p(i, j), p1 = getDarkestPoint(i, j, gray, kernel);
						if (dataAt<uchar>(myCanny, p1.x, p1.y) == 0 && dataAt<uchar>(gray, p2.x, p2.y) - dataAt<uchar>(gray, p1.x, p1.y)>10){
							//Debug() << "random: " << (int)dataAt<uchar>(gray, p2.x, p2.y) << " drakest: " << (int)dataAt<uchar>(gray, p1.x, p1.y);
							if (p1.dis(p) < p1.dis(p2)){
								dataAt<Vec3b>(dst, p2.x, p2.y) = dataAt<Vec3b>(dst, p1.x, p1.y);
								dataAt<uchar>(mask, p2.x, p2.y) = 1;
							}
						}
					}
				}
			}
		}
	}
	//imwrite("process/wetinwet-maskpoint.jpg", mask*255);
	{//getNearest
		int p2 = 0;
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) if (dataAt<uchar>(mask, i, j) != 0){
				nearest[i][j] = myPoint(i, j);
				Q[++p2] = myPoint(i, j);
				visit[i][j] = 1;
			}
		for (int p1 = 1; p1 <= p2; p1++){
			myPoint v = Q[p1];
			for (int k = 0; k < 4; k++){
				int nx = v.x + ArraySpace::xo[k];
				int ny = v.y + ArraySpace::yo[k];
				if (ArraySpace::inMap(src, myPoint(nx, ny)) && !visit[nx][ny]){
					visit[nx][ny] = 1;
					Q[++p2] = myPoint(nx, ny);
					nearest[nx][ny] = nearest[v.x][v.y];
				}
			}
		}

	}
/*
	{//Debug
		Mat test=Mat::zeros(src.size(),CV_8UC3);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++){
				if (dataAt<uchar>(mask, i, j) != 0){
					dataAt<Vec3b>(test, i, j) = Vec3b(255, 255, 255);
				}
				if (dataAt<uchar>(myCanny, i, j) != 0){
					dataAt<Vec3b>(test, i, j) = Vec3b(0, 255, 0);
				}
			}
		imwrite("process/wetinwet-debug.jpg",test);
	}
*/
	{//makeMask
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) if (dataAt<uchar>(myCanny, i, j) != 0){
				if (!visit[i][j]){
					Debug() << "[ERROR] point not visit "<<i<<" "<<j;
					break;
				}
				myPoint p1(i, j), p2 = nearest[i][j];
				int dx = p1.x - p2.x, dy = p1.y - p2.y;
				if (dx != 0){
					double dd = double(dy) / dx;
					for (int k = p1.x; k <= p2.x; k++){
						int ddy = dd*(k - p1.x);
						if (ArraySpace::inMap(mask, k, ddy)){
							dataAt<uchar>(mask, k, ddy) = 1;
						}
					}
					for (int k = p2.x; k <= p1.x; k++){
						int ddy = dd*(k - p2.x);
						if (ArraySpace::inMap(mask, k, ddy)){
							dataAt<uchar>(mask, k, ddy) = 1;
						}
					}
				}
				else{
					for (int k = p1.y; k <= p2.y; k++){
						dataAt<uchar>(mask, p1.x, k) = 1;
					}
					for (int k = p2.y; k <= p1.y; k++){
						dataAt<uchar>(mask, p1.x, k) = 1;
					}
				}
			}
		//imwrite("process/wetinwet-maskline.jpg", mask * 255);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m;j++) 
				if (dataAt<uchar>(mask, i, j) != 0){
					for (int k = 0; k < 4; k++){
						int nx = i + ArraySpace::xo[k];
						int ny = j + ArraySpace::yo[k];
						if (ArraySpace::inMap(mask,myPoint( nx, ny))){
							dataAt<uchar>(mask, i, j) = 1;
						}
					}
				}
		
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) if (dataAt<uchar>(mask, i, j) != 0 && dataAt<uchar>(myCanny, i, j) != 0){
				dataAt<uchar>(mask, i, j) = 0;
			}
		//imwrite("process/wetinwet-mask.jpg", mask * 255);
	}
	{//meanFilter
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) if (dataAt<uchar>(mask, i, j) != 0){
				Scalar s = getMeanPoint<Vec3b>(dst, i, j, 5);
				dataAt<Vec3b>(dst, i, j) = Vec3b(s[0], s[1], s[2]);
			}
	}
}