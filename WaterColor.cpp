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
using namespace cv;

void WaterColor::deal(Mat &src, Mat &dst)
{
	dst = src.clone();


	//Pre deal
	Debug() << "color transform...";
	Mat myColorTransform;
	ColorAdjustment colorAdjustment;
	//colorAdjustment.chooseOneStyle("test");   //预处理图片风格
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
	//Mat myAbstraction = imread("process/abstraciton.jpg");
	Debug() << "wet in wet...";
	Mat myWetInWet;
	WetInWet wetInWet;
	wetInWet.deal(src,myAbstraction, myWetInWet);
	imwrite("process/wetinwet.jpg", myWetInWet);

	//	HandTremorEffect handTremoeEffect;
//	handTremoeEffect.deal(output, output);
//	
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
}