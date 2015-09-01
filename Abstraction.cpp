#include "Abstraction.h"
#include "ToolBox.h"
#include "include\meanshift\MeanShift.h"
using namespace cv;


void meanShift(Mat &src,Mat &dst)
{
	imwrite("src.jpg", src);
	IplImage *img = cvLoadImage("src.jpg");
	// Mean shift
	int **ilabels = new int *[img->height];
	for (int i = 0; i<img->height; i++) ilabels[i] = new int[img->width];
	int regionCount = MeanShift(img, ilabels);
	vector<int> color(regionCount);
	CvRNG rng = cvRNG(cvGetTickCount());
	for (int i = 0; i<regionCount; i++)
		color[i] = cvRandInt(&rng);

	// Draw random color
	for (int i = 0; i<img->height; i++)
		for (int j = 0; j<img->width; j++)
		{
			int cl = ilabels[i][j];
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0] = (color[cl]) & 255;
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1] = (color[cl] >> 8) & 255;
			((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2] = (color[cl] >> 16) & 255;
		}

	dst=Mat(img);
}


void Abstraction::deal(Mat &src, Mat &mySaliency, Mat &myDis, Mat &dst)
{
	Mat myMeanShift;
	meanShift(src,myMeanShift);
	imshow("meanshift", myMeanShift);
	waitKey();
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			//inside the saliency region
			if (dataAt<uchar>(mySaliency, i, j)>0){
				
			} else{//outside the saliency region
				
			}
		}
	}
}