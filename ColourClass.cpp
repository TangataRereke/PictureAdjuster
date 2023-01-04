#ifndef COLOURCLASS_H
	#include "ColourClass.h"
	#define COLOURCLASS_H
#endif
#include <algorithm>
#include <math.h>

void ColourClass::setRGB(short pRed, short pGreen, short pBlue){
	gColour.red = pRed;
	gColour.green = pGreen;
	gColour.blue = pBlue;
	setRatiosBasedOffRGB();
}

void ColourClass::setInteger(long unsigned pColour){
	long lRemainder = pColour;
	memset(&gColour, 0, sizeof(ColourRef));
	if(lRemainder>65536){
		gColour.blue = floor(lRemainder / 65536);
		lRemainder = lRemainder - (gColour.blue * 65536);
	}
	if(lRemainder>256){
		gColour.green = floor(lRemainder / 256);
		lRemainder = lRemainder - (gColour.green * 256);
	}
	gColour.red = lRemainder;
	setRatiosBasedOffRGB();
}

void ColourClass::setHue(short unsigned pHue){
	memset(&gColour, 0, sizeof(ColourRef));
	gPercentBlue = 0;
	gPercentGreen = 0;
	gPercentRed = 0;
	gHue = pHue;

	double lAffect = floor(pHue % 15);
	double lMain = 1;//(pHue % 2);
	int lTest = 0;
	double lSecondary = lMain / 2;
	if(lAffect==lTest++){
		gPercentRed = lMain;
		gPercentGreen = lMain;
		gPercentBlue = lMain;
	}else if(lAffect==lTest++){
		gPercentRed = lMain;
		// red
	}else if(lAffect==lTest++){
		gPercentRed = lMain;
		gPercentGreen = lSecondary;
		// red/green
	}else if(lAffect==lTest++){
		gPercentGreen = lMain;
		gPercentRed = lMain;
		gPercentBlue = lSecondary;
		// yellow/blue
	}else if(lAffect==lTest++){
		gPercentGreen = lMain;
		gPercentRed = lMain;
		// yellow

	}else if(lAffect==lTest++){
		gPercentGreen = lMain;
		gPercentRed = lSecondary;
		// green/red
	}else if(lAffect==lTest++){
		gPercentGreen = lMain;
		// green
	}else if(lAffect==lTest++){
		gPercentGreen = lMain;
		gPercentBlue = lSecondary;
		// green/blue
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentGreen = lMain;
		gPercentRed = lSecondary;
		// cyan/red
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentGreen = lMain;
		// cyan

	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentGreen = lSecondary;
		// blue/green
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		// blue
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentRed = lSecondary;
		// blue/red
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentRed = lMain;
		gPercentGreen = lSecondary;
		// magenta/green
	}else if(lAffect==lTest++){
		gPercentBlue = lMain;
		gPercentRed = lMain;
		// magenta
	}else if(lAffect==lTest++){
		gPercentRed = lMain;
		gPercentBlue = lSecondary;
		// red/blue
	}


	gColour.red = gPercentRed * 255;
	gColour.green = gPercentGreen * 255;
	gColour.blue = gPercentBlue * 255;
	setRatiosBasedOffRGB();
}

void ColourClass::setBrightness(double pBrightness){
	double lNewBrightness = (gBrightness-pBrightness) *-1;
	short lColour = (gOriginalRed + lNewBrightness) * 255;
	if(lColour>255){
		lColour=255;
	}else if(lColour<0){
		lColour = 0;
	}
	gPercentRed = (double)lColour/255;
	gColour.red = lColour;

	lColour = (gOriginalGreen + lNewBrightness) * 255;
	if(lColour>255){
		lColour=255;
	}else if(lColour<0){
		lColour = 0;
	}
	gPercentGreen = (double)lColour/255;
	gColour.green = lColour;

	lColour = (gOriginalBlue + lNewBrightness) * 255;
	if(lColour>255){
		lColour=255;
	}else if(lColour<0){
		lColour = 0;
	}
	gPercentBlue = (double)lColour/255;
	gColour.blue = lColour;
	gBrightness = pBrightness;
}

short ColourClass::getMax(){
    return max(max(gColour.red, gColour.green),gColour.blue);
};

short ColourClass::getMin(){
    return min(min(gColour.red, gColour.green),gColour.blue);
};
short unsigned ColourClass::getCalculatedHue(){
	short unsigned lRedRatio = 2;
	if(gPercentRed<.10){
		lRedRatio = 0;
	}else if(gPercentRed<.52){
		lRedRatio = 1;
	}
	short unsigned lGreenRatio = 2;
	if(gPercentGreen<.10){
		lGreenRatio = 0;
	}else if(gPercentGreen<.52){
		lGreenRatio = 1;
	}
	short unsigned lBlueRatio = 2;
	if(gPercentBlue<.10){
		lBlueRatio = 0;
	}else if(gPercentBlue<.52){
		lBlueRatio = 1;
	}
	if(max(max(lRedRatio, lGreenRatio), lBlueRatio)<2){
		if(lRedRatio==1){
			lRedRatio++;
		}
		if(lGreenRatio==1){
			lGreenRatio++;
		}
		if(lBlueRatio==1){
			lBlueRatio++;
		}
	}
	if(lBlueRatio==2&&lRedRatio==1&&lGreenRatio==1){
		lRedRatio = 0;
		lGreenRatio = 0;
	}else if(lBlueRatio==1&&lRedRatio==2&&lGreenRatio==1){
		lBlueRatio = 0;
		lGreenRatio = 0;
	}else if(lBlueRatio==1&&lRedRatio==1&&lGreenRatio==2){
		lBlueRatio = 0;
		lRedRatio = 0;

	}
	//std::cout << getDisplay() << " = " << gPercentRed << "," << gPercentGreen << "," << gPercentBlue << ": " << lRedRatio << "," << lGreenRatio << "," << lBlueRatio << std::endl;
	
	if(lRedRatio==lGreenRatio&&lGreenRatio==lBlueRatio){
		return 0;
	}else if(lRedRatio==2&&lGreenRatio==0&&lBlueRatio==0){
		return 1;
	}else if(lRedRatio==2&&lGreenRatio==1&&lBlueRatio==0){
		return 2;
	}else if(lRedRatio==2&&lGreenRatio==2&&lBlueRatio==1){
		return 3;
	}else if(lRedRatio==2&&lGreenRatio==2&&lBlueRatio==0){
		return 4;
	}else if(lRedRatio==1&&lGreenRatio==2&&lBlueRatio==0){
		return 5;
	}else if(lRedRatio==0&&lGreenRatio==2&&lBlueRatio==0){
		return 6;
	}else if(lRedRatio==0&&lGreenRatio==2&&lBlueRatio==1){
		return 7;
	}else if(lRedRatio==1&&lGreenRatio==2&&lBlueRatio==2){
		return 8;
	}else if(lRedRatio==0&&lGreenRatio==2&&lBlueRatio==2){
		return 9;
	}else if(lRedRatio==0&&lGreenRatio==1&&lBlueRatio==2){
		return 10;
	}else if(lRedRatio==0&&lGreenRatio==0&&lBlueRatio==2){
		return 11;
	}else if(lRedRatio==1&&lGreenRatio==0&&lBlueRatio==2){
		return 12;
	}else if(lRedRatio==2&&lGreenRatio==1&&lBlueRatio==2){
		return 13;
	}else if(lRedRatio==2&&lGreenRatio==0&&lBlueRatio==2){
		return 14;
	}else if(lRedRatio==2&&lGreenRatio==0&&lBlueRatio==1){
		return 15;
	}
	std::cout << "We have a 16 " << lRedRatio << "/" << lGreenRatio << "/" << lBlueRatio << "/" <<  std::endl;
	return 16;
}

void ColourClass::setRatiosBasedOffRGB(){
    gPercentRed = (double)gColour.red / 255;
	gOriginalRed = gPercentRed;
    gPercentGreen = (double)gColour.green / 255;
	gOriginalGreen = gPercentGreen;
    gPercentBlue = (double)gColour.blue / 255;
	gOriginalBlue = gPercentBlue;
	gSaturation = getMaxRange() - getMinRange();
	gBrightness = (gPercentRed + gPercentGreen + gPercentBlue) / 3;
	gHue = getCalculatedHue();
}

double ColourClass::getMaxRange(){
	double lMax = 0;
	lMax = gPercentRed;
	if(gPercentBlue>lMax){
		lMax = gPercentBlue;
	}
	if(gPercentGreen>lMax){
		lMax = gPercentGreen;
	}
	return lMax;
}
double ColourClass::getMidRange(){
	double lMax = getMaxRange();
	double lMin = getMinRange();
	double lMiddle = 0;
	
	if(gPercentBlue!=lMax&&gPercentBlue!=lMin){
		lMiddle = gPercentBlue;
	}else if(gPercentGreen!=lMax&&gPercentGreen!=lMin){
		lMiddle = gPercentGreen;
	}else if(gPercentRed!=lMax&&gPercentRed!=lMin){
		lMiddle = gPercentRed;
	}else{
		lMiddle = ((lMax - lMin) / 2) + lMin;
	}
	return lMiddle;
}
double ColourClass::getMinRange(){
	double lMin = 0;
	lMin = gPercentRed;
	if(gPercentBlue<lMin){
		lMin = gPercentBlue;
	}
	if(gPercentGreen<lMin){
		lMin = gPercentGreen;
	}
	return lMin;
}
void ColourClass::setSaturation(double pValue){
	double lNewR = gPercentRed;
	double lNewG = gPercentGreen;
	double lNewB = gPercentBlue;
	double lMax = getMaxRange();
	double lMin = getMinRange();
	double lMid = getMidRange();
	double lNewMax = pValue;
	double lNewMin = 1 - pValue;
	double lNewMid = lNewMin + ((lNewMax - lNewMin)/2);
	if(lNewMin>lNewMax){
		lNewMin = lNewMax;
	}
	if(lNewMid>lNewMax){
		lNewMid = lNewMax;
	}else if(lNewMid<lNewMin){
		lNewMid = lNewMid;
	}
	if(lNewR==lMax){
		lNewR = lNewMax;
	}
	if(lNewG==lMax){
		lNewG = lNewMax;
	}
	if(lNewB==lMax){
		lNewB = lNewMax;
	}
	if(gPercentRed==lMin){
		lNewR = lNewMin;
	}
	if(gPercentGreen==lMin){
		lNewG = lNewMin;
	}
	if(gPercentBlue==lMin){
		lNewB = lNewMin;
	}
	if(gPercentRed==lMid){
		lNewR = lNewMid;
	}
	if(gPercentGreen==lMid){
		lNewG = lNewMid;
	}
	if(gPercentBlue==lMid){
		lNewB = lNewMid;
	}
	gColour.red = (lNewR * gPercentRed) * 255;
	gColour.green = (lNewG * gPercentGreen) * 255;
	gColour.blue = (lNewB * gPercentBlue) * 255;
	setRatiosBasedOffRGB();
}

bool ColourClass::isSkin(){
	if(gHue<0||gHue>4){
		return false;
	}
	if(gSaturation>=.06125&&gSaturation<=.605){
		return true;
	}
	return false;
}

bool ColourClass::isMinorChange(ColourRef pCompare){
	ColourClass lCompareWith;
	lCompareWith.setRGB(pCompare.red, pCompare.green, pCompare.blue);
	if(getHue()!=lCompareWith.getHue()){
		return true;
	}
	return false;
}

bool ColourClass::isTintable(){
	return getHue() != 0;
}

const char *ColourClass::getDisplay(){
	stringstream lSS;
	lSS << "R" << (int)gColour.red << "G" << (int)gColour.green << "B" << (int)gColour.blue;
	return lSS.str().c_str();
}
