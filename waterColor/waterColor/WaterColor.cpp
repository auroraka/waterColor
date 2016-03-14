#include "WaterColor.h"
#include "ColorAdjustment.h"
#include "SaliencyDistance.h"
#include "Abstraction.h"
#include "WetInWet.h"
#include "HandTremorEffect.h"
#include "EdgeDarkening.h"
#include "Granulation.h"
#include "TurblenceFlow.h"
#include "PigmentVariation.h"
#include "ToolBox.h"
#include "Debug.h"
using namespace cv;

void texture(Mat &src,Mat &dst){
	vector<Mat> srcs;
	split(src, srcs);
	Mat texture = imread("texture.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat add(src.size(), CV_8U);

	for (int i = 0; i < add.rows; i++)
		for (int j = 0; j < add.cols; j++){
			int x = texture.at<uchar>(i%texture.rows, j%texture.cols);
			x = max(x, 240);
			add.at<uchar>(i, j) = min(x, 255);
		}
	//imshow("add", add);
	//waitKey();
	srcs.push_back(add);
	merge(srcs, dst);
	//imwrite("aaa.png", dst);
}	

void WaterColor::deal(Mat &src, Mat &dst)
{
	dst = src.clone();


	//Pre deal
	Debug() << "color transform...";
	Mat myColorTransform;
	ColorAdjustment colorAdjustment;
	colorAdjustment.chooseOneStyle("test");   //预处理图片风格
	colorAdjustment.deal(src, myColorTransform);
	//myColorTransform = src.clone();
	imwrite("process/colorTransform.jpg", myColorTransform);

//		dst = myColorTransform;////
//			return;////

	
	//Basic transform
	Debug() << "saliency...";
	Mat mySaliency,myDis;
	SaliencyDistance saliencyDiatance;
	saliencyDiatance.deal(myColorTransform, mySaliency,myDis);
	imwrite("process/dis.jpg", myDis*255);
	imwrite("process/saliency.jpg", mySaliency);

	
//	//Abstraciton
	Debug() << "abstraction...";
	Mat myAbstraction;
	Abstraction abstraction;
	abstraction.deal(myColorTransform, mySaliency,myDis,myAbstraction);
	imwrite("process/abstraciton.jpg", myAbstraction);


	//Add effects
	//Mat myAbstraction = imread("process/abstraciton.jpg");////
	Debug() << "wet in wet...";
	Mat myWetInWet, myCanny;
	WetInWet wetInWet;
	wetInWet.deal(src,myAbstraction, myWetInWet,myCanny);
	imwrite("process/wetinwet.jpg", myWetInWet);






//	Mat myHandTremor;
//	HandTremorEffect handTremoeEffect;
//	handTremoeEffect.deal(myWetInWet, myHandTremor,myCanny);
//	imwrite("process/handtremor.jpg", myHandTremor);


//	EdgeDarkening edgeDarkening;
//	edgeDarkening.deal(output, output);
//	
//	Granulation granulation;
//	granulation.deal(output, output);
//	
//	TurblenceFlow turblenceFlow;
//	turblenceFlow.deal(output, output);
//	
//	//After deal
//	PigmentVariation pigmentVariation;
//	pigmentVariation.deal(output, output);
//
//	//DoubleToImage(output, output);
//	//DoubleToImage(output, output);
//	output.convertTo(output, CV_8UC3, 255.0);

	
	//Mat myWetInWet = imread("process/wetinwet.jpg");
	Mat myTexture;
	texture(myWetInWet, myTexture);
	//Debug() << myTexture.channels();
	imwrite("process/texture.png", myTexture);
	dst = myTexture;
}