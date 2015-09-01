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

void WaterColor::deal(const Mat &src, Mat &dst)
{
	Mat srcd;
	ImageToDouble(src, srcd);

	//Pre deal
	Mat myColorTransform;
	ColorAdjustment colorAdjustment;
	colorAdjustment.deal(src, myColorTransform);
	imshow("colorTransform", myColorTransform);

	//Basic transform
	Mat mySaliency,myDis;
	SaliencyDistance saliencyDiatance;
	saliencyDiatance.deal(myColorTransform, mySaliency,myDis);
	
	Mat myAbstraction;
	Abstraction abstraction;
	abstraction.deal(myColorTransform, mySaliency,myDis,myAbstraction);
	dst = mySaliency;
	//	
//	//Add effects
//	WetInWet wetInWet;
//	wetInWet.deal(output, output);
//	
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