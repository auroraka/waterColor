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

void WaterColor::deal(const Mat &input, Mat &output)
{
	output = input.clone();
	//ImageToDouble(output, output);
	//ImageToDouble(output, output);
	output.convertTo(output, CV_32FC3, 1.0f / 255.0);

	//Pre deal
	ColorAdjustment colorAdjustment;
	colorAdjustment.deal(output, output);

	//Basic transform
	SaliencyDistance saliencyDiatance;
	saliencyDiatance.deal(output, output);
	
	Abstraction abstraction;
	abstraction.deal(output, output);
	
	//Add effects
	WetInWet wetInWet;
	wetInWet.deal(output, output);
	
	HandTremorEffect handTremoeEffect;
	handTremoeEffect.deal(output, output);
	
	EdgeDarkening edgeDarkening;
	edgeDarkening.deal(output, output);
	
	Granulation granulation;
	granulation.deal(output, output);
	
	TurblenceFlow turblenceFlow;
	turblenceFlow.deal(output, output);
	
	//After deal
	PigmentVariation pigmentVariation;
	pigmentVariation.deal(output, output);

	//DoubleToImage(output, output);
	//DoubleToImage(output, output);
	output.convertTo(output, CV_8UC3, 255.0);
}