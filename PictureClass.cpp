#include "PictureClass.h"

PictureClass::PictureClass()
{
    //ctor
}

bool PictureClass::comparePart(PictureClass *pCompareWidth, int pX, int pY, int lWidth, int lHeight){
	for(int bY=pY;bY<lHeight;bY++){
		long unsigned lPosition = ((bY*gWidth)+pX)*3;
		if(memcmp(gData+lPosition, pCompareWidth->gData+lPosition, lWidth*3)!=0){
			return false;
		}
	}
	return true;
}

bool PictureClass::compare(PictureClass *pCompareWidth){
	int lHeight = gHeight * .75;
	int lWidth = gWidth * .75;
	for(int bX=0;bX<gWidth-lWidth;bX++){
		for(int bY=0;bY<gHeight-lHeight;bY++){
			if(comparePart(pCompareWidth, bX, bY, lWidth, lHeight)){
				return true;
			}
		}
	}
	return false;
}

void PictureClass::simplifyColours(){
	for(int bY=0;bY<gHeight;bY++){
		for(int bX=0;bX<gWidth-1;bX++){
			getPixel(bX, bY);
			memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
			gPenColour.simplifyColour();
			setPixel(bX, bY);
		}
	}
}

void PictureClass::deleteColourItems(){
		ColourItemSort *lDelete = gAllColourSortItems;
		while(lDelete!=0){
			ColourItemSort *gNext = lDelete->next;
			Point *lPoint = lDelete->allPoints;
			while(lPoint!=0){
				Point *lNext = lPoint->next;
				delete lPoint;
				lPoint = lNext;
			}
			delete gNext;
			lDelete = gNext;
		}
		gAllColourSortItems = 0;
}
void PictureClass::deleteNonColourItems(){
		ColourItemSort *lDelete = gAllNonColourSortItems;
		while(lDelete!=0){
			ColourItemSort *gNext = lDelete->next;
			delete gNext;
			lDelete = gNext;
		}
			gAllNonColourSortItems = 0;
	
}
PictureClass::~PictureClass()
{
	deleteColourItems();
	deleteNonColourItems();
  if(gData!=0){
     delete gData;
  }
    //dtor
}

void PictureClass::clearPicture(){
    if(gPenColour.getSaturation()==0){
        memset(gData, gCurrentColour.gColour.red, gSize);
        return;
    }
    RECT lRect;
    lRect.right = gWidth;
    lRect.bottom = gHeight;
    drawRectangle(lRect);
}

void PictureClass::showSorts(){
	ColourItemSort *lValue = gAllColourSortItems;
	cout << "\t\t\tSHOWING SORTS" << endl;
	while(lValue!=0){
		cout << "\t\t\t\tValue: " << lValue->lookupValue << endl;
		lValue = lValue->next;
	}
	cout << "\t\t\tSHOWING SORTS END" << endl << endl;
}

void PictureClass::insertColourSortItem(double pLookupValue, int pX, int pY){
	ColourItemSort *lLookup = gAllColourSortItems;
	ColourItemSort *lLast = 0;
	Point *lPoint = new Point;
	lPoint->x = pX;
	lPoint->y = pY;
	lPoint->next = 0;

	while(lLookup!=0){
		if(lLookup->lookupValue==pLookupValue){
			lLookup->lastPoint->next = lPoint;
			lLookup->lastPoint = lPoint;
			lLookup->count++;
			return;
		}else if(lLookup->lookupValue < pLookupValue){
				gSortItemColourCount++;
				ColourItemSort *lNew = new ColourItemSort;
				lNew->allPoints = lPoint;
				lNew->lookupValue = pLookupValue;
				lNew->previous = lLookup->previous;
				lNew->count = 1;
				lNew->lastPoint = lPoint;
				
				if(lLookup->previous!=0){
					lLookup->previous->next = lNew;
				}
				lNew->next = lLookup;
				lLookup->previous = lNew;
				
				if(lLookup==gAllColourSortItems){
					gAllColourSortItems = lNew;
				}
				return;
		}
		lLast = lLookup;
		lLookup = lLookup->next;
	}
	gSortItemColourCount++;
	ColourItemSort *lNew = new ColourItemSort;
	lNew->lookupValue = pLookupValue;
	lNew->previous = lLast;
	lNew->allPoints = lPoint;
	lNew->count = 1;
	lNew->next = 0;
	lNew->lastPoint = lPoint;
	
	if(lLast!=0){
		lLast->next = lNew;
	}
	if(gAllColourSortItems==0){
		gAllColourSortItems = lNew;
		return;
	}
	return;
}

void PictureClass::insertNonColourSortItem(short pLookupValue){
	ColourItemSort *lLookup = gAllNonColourSortItems;
	ColourItemSort *lLast = 0;
	
	while(lLookup!=0){
		if(lLookup->lookupValue==pLookupValue){
			return;
		}else if(lLookup->lookupValue > pLookupValue){
				gSortItemNonColourCount++;
				ColourItemSort *lNew = new ColourItemSort;
				lNew->lookupValue = pLookupValue;
				lNew->previous = lLookup->previous;
				if(lLookup->previous!=0){
					lLookup->previous->next = lNew;
				}
				lNew->next = lLookup;
				lLookup->previous = lNew;
				
				if(lLookup==gAllNonColourSortItems){
					gAllNonColourSortItems = lNew;
				}
				return;
		}
		lLast = lLookup;
		lLookup = lLookup->next;
	}
	gSortItemNonColourCount++;
	ColourItemSort *lNew = new ColourItemSort;
	lNew->lookupValue = pLookupValue;
	lNew->previous = lLast;
	lNew->lastPoint = 0;
	lNew->next = 0;
	if(lLast!=0){
		lLast->next = lNew;
	}
	if(gAllNonColourSortItems==0){
		gAllNonColourSortItems = lNew;
		return;
	}
	return;
}

bool PictureClass::removeInterferenceX(){
	bool lChanged = false;
	for(int bY=0;bY<gHeight;bY++){
		for(int bX=0;bX<gWidth-1;bX++){
			getPixel(bX, bY);
			memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
			getPixel(bX+1, bY);
			if(gPenColour.isMinorChange(gCurrentColour.gColour)){
				for(int bNewX=bX+2;bNewX<=bX+(gWidth*.0025);bNewX++){
					if(bNewX>=gWidth){
						break;
					}
					getPixel(bNewX, bY);
					if(!gPenColour.isMinorChange(gCurrentColour.gColour)){
						lChanged = true;
						setPixel(bX+1, bY);
						break;
					}
				}
			}
		}
	}
	return lChanged;
}
bool PictureClass::removeInterferenceY(){
	bool lChanged = false;
	for(int bY=0;bY<gHeight-1;bY++){
		for(int bX=0;bX<gWidth;bX++){
			getPixel(bX, bY);
			memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
			getPixel(bX, bY+1);
			if(gPenColour.isMinorChange(gCurrentColour.gColour)){
				for(int bNewY=bY+2;bNewY<=bY+(gHeight*.0025);bNewY++){
					if(bNewY>=gHeight){
						break;
					}
					getPixel(bX, bNewY);
					if(!gPenColour.isMinorChange(gCurrentColour.gColour)){
						lChanged = true;
						setPixel(bX, bY+1);
						break;
					}
				}
			}
		}
	}
	
	return lChanged;
}

void PictureClass::removeInterference(){
	while(removeInterferenceY()){		

	}
	while(removeInterferenceX()){		
	}
	
	while(removeInterferenceY()){		

	}
	while(removeInterferenceX()){		
	}
	
}

double PictureClass::getHighestColourMax(){
	deleteColourItems();
	gSortItemColourCount = 0;
	double lHighestColour = 0;
	double lHighestColourMax = 0;

	for(int bX=0;bX<gWidth;bX++){
		for(int bY=0;bY<gHeight;bY++){
			getPixel(bX, bY);
			double lValue = gCurrentColour.getBrightness();
			insertColourSortItem(lValue, bX, bY);
			if(lValue==lHighestColour){
				if(gCurrentColour.getMaximumBalance()<lHighestColourMax){
					lHighestColourMax = gCurrentColour.getMaximumBalance();
				}
			}else if(lValue>lHighestColour){
				lHighestColour = lValue;
				lHighestColourMax = gCurrentColour.getMaximumBalance();
			}

		}
	}
	return lHighestColourMax;
}

double PictureClass::getLowestColourMin(){
	double lLowestColour = 0;
	double lLowestColourMin = 0;

	for(int bX=0;bX<gWidth;bX++){
		for(int bY=0;bY<gHeight;bY++){
			getPixel(bX, bY);
			double lValue = gCurrentColour.getBrightness();
			if(lValue==lLowestColour){
				if(gCurrentColour.getMinimumBalance()>lLowestColour){
					lLowestColourMin = gCurrentColour.getMinimumBalance();
				}
			}else if(lValue<lLowestColour){
				lLowestColour = lValue;
				lLowestColourMin = gCurrentColour.getMinimumBalance();
			}

		}
	}
	return lLowestColourMin;
}

double PictureClass::changeBrightnessOfItem(bool pFirst, ColourItemSort *pColourSortItem, double pTargetBrightness, double pFactor){
	if(pTargetBrightness<0){
		return pTargetBrightness;
	}
	bool lChanged = true;
	Point *lPoint = pColourSortItem->allPoints;
	while(lPoint!=0){
		getPixel(lPoint->x, lPoint->y);
		double lBrightness  = pColourSortItem->lookupValue;
		gCurrentColour.setBrightness(pTargetBrightness);
		if(!pFirst&&gCurrentColour.getBrightness()==lBrightness){
			lChanged = false;
			break;
		}
		memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
		setPixel(lPoint->x, lPoint->y);
		lPoint = lPoint->next;
	}
	if(!lChanged){
		if(pFactor<=0){
			return 0;
		}
		return changeBrightnessOfItem(pFirst, pColourSortItem, pTargetBrightness - pFactor, pFactor);
	}
	return pTargetBrightness;
	
}

double PictureClass::autoBrightenSmallFactor(double pHighestBrightness, double pSmallFactor){
	double lTargetBrightness = pHighestBrightness;
	ColourItemSort *lItem = gAllColourSortItems;
	while(lItem!=0){
		lTargetBrightness = changeBrightnessOfItem(gAllColourSortItems==lItem, lItem, lTargetBrightness, pSmallFactor);
		if(lItem->next!=0){
				lTargetBrightness = lTargetBrightness - pSmallFactor;
				if(lTargetBrightness<0){
					return lTargetBrightness;
				}
		}
		lItem = lItem->next;
	}
	return lTargetBrightness;
	
}

void PictureClass::lastResortAutoBrighten(double pColourMax){
	double lHighestColourMax = pColourMax;
	double lLowestColourMin = getLowestColourMin();
	double lRate = (lHighestColourMax - lLowestColourMin) / gSortItemColourCount;
	double lBrightness = lHighestColourMax;
	ColourItemSort *lItem = gAllColourSortItems;
	while(lItem!=0){
		Point *lPoint = lItem->allPoints;
		while(lPoint!=0){
			getPixel(lPoint->x, lPoint->y);
			memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
			gPenColour.setBrightness(lBrightness);
			setPixel(lPoint->x, lPoint->y);
			lPoint = lPoint->next;
		}
		if(lItem->next!=0){
			lBrightness = lBrightness - lRate;	
			lItem = lItem->next;
		}else{
			return;
		}	
	}
}

bool PictureClass::autoBrighten(double pHighestBrightness, double pFactor, double pTryAgainAmount){
	if(pFactor<=0){
		return false;
	}
	if(pHighestBrightness<0){
		return false;
	}
	double lNewBrightness = autoBrightenSmallFactor(pHighestBrightness, .003);
	if(lNewBrightness<=0){
		if((pFactor-pTryAgainAmount)<=0){
			if(pTryAgainAmount==0.1){
				return false;
			}
			pTryAgainAmount = pTryAgainAmount / 10;
		}
		return autoBrighten(pHighestBrightness, pFactor - pTryAgainAmount, pTryAgainAmount);
	}
	
	if(lNewBrightness<=64){ // 156 good | 127 alright
		return true;
	}
	return autoBrighten(pHighestBrightness-1, pFactor, pTryAgainAmount);
}

void PictureClass::autoLighten(){
	double lHighestColourMax = getHighestColourMax();
	lastResortAutoBrighten(lHighestColourMax);
	return;
	if(!autoBrighten(lHighestColourMax, 0.3, .1)){
		lastResortAutoBrighten(lHighestColourMax);
	}
}


int PictureClass::recolourGradientFrom(int pX, int pY){
	int lStartX = pX;
	int lEndX = pX;
	getPixel(pX, pY);
	bool lDifferent = false;
	memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
	for(int bX = pX;bX>=0;bX--){
		getPixel(bX, pY);
		if(gPenColour.isMinorChange(gCurrentColour.gColour)){
			break;
		}
		if(!lDifferent&&memcmp(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef))==0){
			lDifferent = true;
		}
		lStartX = bX;
	}
	for(int bX = pX;bX<gWidth;bX++){
		getPixel(bX, pY);
		if(gPenColour.isMinorChange(gCurrentColour.gColour)){
			break;
		}
		if(!lDifferent&&memcmp(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef))==0){
			lDifferent = true;
		}
		lEndX = bX;
	}
	getPixel(lStartX, pY);
	memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
	getPixel(lEndX, pY);
	double lRedRate = (gCurrentColour.gColour.red - gPenColour.gColour.red) / (double)(lEndX - lStartX);
	double lGreenRate = (gCurrentColour.gColour.green - gPenColour.gColour.green) / (double)(lEndX - lStartX);
	double lBlueRate = (gCurrentColour.gColour.blue - gPenColour.gColour.blue) / (double)(lEndX - lStartX);
	double lRunningRed = gPenColour.gColour.red;
	double lRunningGreen = gPenColour.gColour.green;
	double lRunningBlue = gPenColour.gColour.blue;
	if(!lDifferent){
		return -1;
	}
	for(int bX = lStartX;bX<=lEndX;bX++){
		gPenColour.setRGB(lRunningRed, lRunningGreen, lRunningBlue);
		setPixel(bX, pY);
		if(bX<lEndX){
			lRunningRed = lRunningRed + lRedRate;
			lRunningGreen = lRunningGreen + lGreenRate;
			lRunningBlue = lRunningBlue + lBlueRate;
		}
	}
	return lEndX;
}

void PictureClass::removeTint(){
	log("removeTint begin");
	ColourClass::ColourRef gMainTint;
	memset(&gMainTint, 0, sizeof(ColourClass::ColourRef));
	bool lStartedTint = false;
	for(int bX=0;bX<gWidth;bX++){
		for(int bY=0;bY<gHeight;bY++){
			getPixel(bX, bY);
			if(!gCurrentColour.isTintable()){
				continue;
			}else if(!lStartedTint){
				memcpy(&gMainTint, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
				lStartedTint = true;
			}else{
				bool lStop = false;
				gMainTint = gCurrentColour.checkTint(gMainTint, lStop);
				if(lStop){
					return;
				}
			}
		}
	}
	ColourClass lClass;
	memcpy(&lClass.gColour, &gMainTint, sizeof(ColourClass::ColourRef));
	//lClass.setBrightness(0);
	double lBrightness = lClass.getBrightness();
	for(int bX=0;bX<gWidth;bX++){
		for(int bY=0;bY<gHeight;bY++){
			getPixel(bX, bY);
			if(gCurrentColour.isTintable()){
				gPenColour.gColour = gCurrentColour.applyTint(lClass.gColour);
				setPixel(bX, bY);
			}else{
				memcpy(&gPenColour.gColour, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
				double bBrightness = gCurrentColour.getBrightness();
				gPenColour.setBrightness(bBrightness-lBrightness);				
				setPixel(bX, bY);
			}
		}
	}
	autoLighten();
	log("removeTint end");
	
}

void PictureClass::removeDuplicatePixels(){
	log("removeDuplicatePixels begin");
	std::vector<std::thread> lThreads;
	for(int bY=0;bY<gHeight;bY++){
		for(int bX=0;bX<gWidth;bX++){
			getPixel(bX, bY);
			if(bX>0&&memcmp(&gCurrentColour, &gPenColour, sizeof(ColourClass::ColourRef))==0){
				int lResult = recolourGradientFrom(bX, bY);
				if(lResult>0){
					bX = lResult+1;
				}
			}
			memcpy(&gPenColour, &gCurrentColour, sizeof(ColourClass::ColourRef));
		}
	}
	
	log("removeDuplicatePixels end");
}



short PictureClass::getNewNonColourValueFor(short pOldValue){
	ColourItemSort *lItem = gAllNonColourSortItems;
	while(lItem!=0){
		if(lItem->lookupValue==pOldValue){
			return lItem->newValue;
		}
		lItem = lItem->next;
	}
	cout << "Couldn't find " << pOldValue << endl;
	return 0;
	
}

short PictureClass::getNewColourValueFor(short pOldValue){
	ColourItemSort *lItem = gAllColourSortItems;
	while(lItem!=0){
		if(lItem->lookupValue==pOldValue){
			return lItem->newValue;
		}
		lItem = lItem->next;
	}
	cout << "Couldn't find " << pOldValue << endl;
	return 0;
	
}

void PictureClass::drawRectangle(RECT pRectangle){
    for(int bX = pRectangle.left;bX<pRectangle.right;bX++){
        for(int bY = pRectangle.top;bY<pRectangle.bottom;bY++){
            setPixel(bX, bY);
        }
    }
}

void PictureClass::newImage(int pWidth, int pHeight){
    gWidth = pWidth;
    gHeight = pHeight;

    gSize = gWidth * gHeight* 3;
    if(gData!=0){
        delete gData;
    }
    gData = new BYTE[gSize];
    clearPicture();
}
void PictureClass::log(const char *pText){
    if(gLogging){
        cout << "PictureClass:" << pText << endl;
    }
}

void PictureClass::drawLine(RECT pRect){
    log("drawLine begin");
    double x{(double)pRect.right - (double)pRect.left}, y{(double)pRect.bottom - (double)pRect.top};
    const double lmax{max(fabs(x), fabs(y))};
    double lX = pRect.left;
    double lY = pRect.top;
    x /= lmax; y /= lmax;
    for (double n{0}; n < lmax; ++n)
    {
        if(gIndividualLog){
            cout << "\t\tlX: " << lX << ", lY: " << lY << endl;
        }
        setPixel(lX, lY);
        lX += x; lY += y;
    }
    gLastRect.left = lX;
    gLastRect.top = lY;
    log("drawLine end");

}

void PictureClass::lineTo(int pX, int pY){
    log("lineTo begin");
    gLastRect.bottom = pY;
    gLastRect.right = pX;
    log("lineTo end");

}


void PictureClass::setPen(int pPen){
    gCurrentPenStyle = pPen;
}

int PictureClass::draw1(int pX, int pY){
    log("draw1 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);
    log("draw1 end");
    return pX + TEXT_PADDING;
}

int PictureClass::draw2(int pX, int pY){
    log("draw2 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = lRect.bottom;
    drawLine(lRect);

    lRect.right = lRect.left;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.top = lRect.bottom;
    lRect.right = pX + TEXT_MAXWIDTH;
    drawLine(lRect);

    log("draw2 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw5(int pX, int pY){
    log("draw5 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = lRect.bottom;
    drawLine(lRect);

    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.top = lRect.bottom;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    drawLine(lRect);

    log("draw5 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw3(int pX, int pY){
    log("draw3 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT / 2);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT);
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT);
    drawLine(lRect);

    log("draw3 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw6(int pX, int pY){
    log("draw6 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT / 2);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT);
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT / 2);
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    log("draw6 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

bool PictureClass::hasRowGotSkin(int pRow){
    for(int lX=0;lX<gWidth;lX++){
        getPixel(lX, pRow);
        if(gCurrentColour.isSkin()){
            return true;
        }
    }
    return false;
}


void PictureClass::skinDetect(){

}

bool PictureClass::hasColumnGotSkin(int pColumn){
    for(int lY=0;lY<gHeight;lY++){
        getPixel(pColumn, lY);
        if(gCurrentColour.isSkin()){
            return true;
        }
    }
    return false;
}

void PictureClass::showSkin(){
	setPen(Pen_Invert);
	for(int lY=0;lY<gHeight;lY++){
		for(int lX=0;lX<gWidth;lX++){
			getPixel(lX, lY);
      if(gCurrentColour.isSkin()){
				setPixel(lX, lY);
			}
   }
	}
}

void PictureClass::bestRatioResize(int pNewWidth, int pNewHeight){
    log("bestRatioResize begin");
    double lWidthRatio = (double)((double)gWidth / (double)pNewWidth);
    double lHeightRatio = (double)((double)gHeight / (double)pNewHeight);
    double lSmallestRatio = min(lWidthRatio, lHeightRatio);
    int lNewHeight = gHeight / lSmallestRatio;
    int lNewWidth = gWidth / lSmallestRatio;
    double lCurrentX = 0;
    double lCurrentY = 0;
    long lSize = lNewWidth * lNewHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize];
    for(int lY=0;lY<lNewHeight;lY++){
        for(int lX=0;lX<lNewWidth;lX++){
            getPixel(lCurrentX, lCurrentY);
            long bPosition = ((lY*lNewWidth)+lX)*3;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
            lCurrentX = lCurrentX + lSmallestRatio;
        }
        lCurrentX = 0;
        lCurrentY = lCurrentY + lSmallestRatio;
    }
    gSize = lSize;
    gWidth = lNewWidth;
    gHeight = lNewHeight;
    delete gData;
    gData = lNewData;
    log("bestRatioResize end");
}

void PictureClass::lowRatioResize(int pNewWidth, int pNewHeight){
    log("lowRatioResize begin");
    double lWidthRatio = (double)((double)gWidth / (double)pNewWidth);
    double lHeightRatio = (double)((double)gHeight / (double)pNewHeight);
    double lLargestRatio = max(lWidthRatio, lHeightRatio);
    int lNewHeight = gHeight / lLargestRatio;
    int lNewWidth = gWidth / lLargestRatio;
    double lCurrentX = 0;
    double lCurrentY = 0;
    long lSize = lNewWidth * lNewHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize];
    for(int lY=0;lY<lNewHeight;lY++){
        for(int lX=0;lX<lNewWidth;lX++){
            getPixel(lCurrentX, lCurrentY);
            long bPosition = ((lY*lNewWidth)+lX)*3;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
            lCurrentX = lCurrentX + lLargestRatio;
        }
        lCurrentX = 0;
        lCurrentY = lCurrentY + lLargestRatio;
    }
    gSize = lSize;
    gWidth = lNewWidth;
    gHeight = lNewHeight;
    delete gData;
    gData = lNewData;
    log("lowRatioResize end");
}

void PictureClass::padRight(){
    log("padRight begin");
    int lNewWidth = gWidth + 1;
    long lSize = lNewWidth * gHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize+1];
    
    for(int lY=0;lY<gHeight;lY++){
        for(int lX=0;lX<gWidth;lX++){
            getPixel(lX, lY);
            long bPosition = ((lY*lNewWidth)+lX)*BYTES_PER_PIXEL;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
        }
    }
    for(int lY=0;lY<gHeight;lY++){
			if(lY==0){
				getPixel(gWidth - 1, lY);
			}
			long bPosition = ((lY*lNewWidth)+(lNewWidth-1))*BYTES_PER_PIXEL;
			memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
		}
    gSize = lSize;
    gWidth = lNewWidth;
    delete gData;
    gData = lNewData;
    log("padRight end");	
}


void PictureClass::padLeft(){
    log("padLeft begin");
    int lNewWidth = gWidth + 1;
    int lCurrentX = 0;
    long lSize = lNewWidth * gHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize+1];
    for(int lY=0;lY<gHeight;lY++){
			if(lY==0){
				getPixel(0, lY);
			}
			long bPosition = ((lY*lNewWidth))*BYTES_PER_PIXEL;
			memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));

		}
    lCurrentX = 1;
    
    for(int lY=0;lY<gHeight;lY++){
        for(int lX=0;lX<gWidth;lX++){
            getPixel(lX, lY);
            long bPosition = ((lY*lNewWidth)+lCurrentX)*BYTES_PER_PIXEL;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
            lCurrentX = lCurrentX + 1;
        }
        lCurrentX = 1;
    }
    gSize = lSize;
    gWidth = lNewWidth;
    delete gData;
    gData = lNewData;
    log("padLeft end");	
}

void PictureClass::chopColumn(int pColumn){
    log("chopColumn begin");
    int lNewWidth = gWidth - 1;
    int lCurrentX = 0;
    int lCurrentY = 0;
    long lSize = lNewWidth * gHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize*2];
    for(int lY=0;lY<gHeight;lY++){
        for(int lX=0;lX<gWidth;lX++){
            getPixel(lX, lY);
            long bPosition = ((lY*lNewWidth)+lCurrentX)*BYTES_PER_PIXEL;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
            if(lX!=pColumn){
                lCurrentX = lCurrentX + 1;
            }
        }
        lCurrentX = 0;
        lCurrentY = lCurrentY + 1;
    }
    gSize = lSize;
    gWidth = lNewWidth;
    delete gData;
    gData = lNewData;
    log("chopColumn end");
}

void PictureClass::padBottom(){
	log("padBottom begin");
	int lNewHeight = gHeight + 1;
	long lSize = gWidth * lNewHeight * BYTES_PER_PIXEL;
	BYTE *lNewData = new BYTE[lSize+1];
	memcpy(lNewData, gData, gSize);
	int lY = gHeight;
	for(int lX=0;lX<gWidth;lX++){
		if(lX==0){
			getPixel(lX, lY-1);
		}
		long bPosition = ((lY*gWidth)+lX)*BYTES_PER_PIXEL;
		memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));

	}

  gSize = lSize;
  gHeight = lNewHeight;
  delete gData;
  gData = lNewData;
	log("padBottom end");
}

void PictureClass::padTop(){
	log("padTop begin");
	int lNewHeight = gHeight + 1;
	long lSize = gWidth * lNewHeight * BYTES_PER_PIXEL;
	BYTE *lNewData = new BYTE[lSize+1];
	memcpy(lNewData+(gWidth*BYTES_PER_PIXEL), gData, gSize);
	for(int lX=0;lX<gWidth;lX++){
		if(lX==0){
			getPixel(lX, 0);
		}
		long bPosition = lX*BYTES_PER_PIXEL;
		memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
	}
  gSize = lSize;
  gHeight = lNewHeight;
  delete gData;
  gData = lNewData;
	log("padTop end");
}

void PictureClass::chopRow(int pRow){
    log("chopRow begin");
    int lNewHeight = gHeight - 1;
    int lCurrentX = 0;
    int lCurrentY = 0;
    long lSize = gWidth * lNewHeight * BYTES_PER_PIXEL;
    BYTE *lNewData = new BYTE[lSize*2];
    for(int lY=0;lY<gHeight;lY++){
        for(int lX=0;lX<gWidth;lX++){
            getPixel(lX, lY);
            long bPosition = ((lCurrentY*gWidth)+lCurrentX)*BYTES_PER_PIXEL;
            memcpy(lNewData+bPosition, &gCurrentColour.gColour, sizeof(ColourClass::ColourRef));
            lCurrentX = lCurrentX + 1;
        }
        lCurrentX = 0;
        if(lY!=pRow){
            lCurrentY = lCurrentY + 1;
        }
    }
    gSize = lSize;
    gHeight = lNewHeight;
    delete gData;
    gData = lNewData;
    log("chopRow end");
}

bool PictureClass::autoCropSide(){
    bool lLeft = hasColumnGotSkin(0);
    bool lRight = hasColumnGotSkin(gWidth-1);
    if(lLeft==lRight){
				if(lLeft){
					return false;
				}
        if(gLastLeftCrop){
            chopColumn(0);
        }else{
            chopColumn(gWidth-1);
        }
        gLastLeftCrop = !gLastLeftCrop;
    }else if(lLeft){
        chopColumn(gWidth-1);
    }else{
        chopColumn(0);
    }
    return true;
}

void PictureClass::padSide(){
	if(gLastPadSide){
		padRight();
		gLastPadSide = false;
	}else{
		padLeft();
		gLastPadSide = true;
	}
}

void PictureClass::padHeight(){
	if(gLastPadTop){
		padBottom();
		gLastPadTop = false;
	}else{
		padTop();
		gLastPadTop = true;
	}
}

bool PictureClass::autoCropTopBottom(){
    bool lTop = hasRowGotSkin(0);
    bool lBottom = hasRowGotSkin(gHeight - 1);
    if(lTop==lBottom){
				if(lTop){
					return false;
				}
        if(gLastTopCrop){
            chopRow(0);
        }else{
            chopRow(gHeight - 1);
        }
        gLastTopCrop = !gLastTopCrop;
    }
    if(lTop){
        chopRow(gHeight - 1);
    }else{
        chopRow(0);
    }
    return true;
}


void PictureClass::smartResize(int pNewWidth, int pNewHeight){
    log("smartResize begin");
    bestRatioResize(pNewWidth, pNewHeight);
    while(gWidth>pNewWidth){
        if(!autoCropSide()){
					padResize(pNewWidth, pNewHeight);
					break;
				}
    }
    while(gHeight>pNewHeight){
        if(!autoCropTopBottom()){
					padResize(pNewWidth, pNewHeight);
					break;
				}
    }
    log("smartResize end");

}

void PictureClass::padResize(int pNewWidth, int pNewHeight){
	lowRatioResize(pNewWidth, pNewHeight);
	removeInterference();
	autoLighten();
	removeInterference();
	while(gWidth<pNewWidth){
		padSide();
	}
	while(gHeight<pNewHeight){
		padHeight();
	}
	
}

int PictureClass::draw9(int pX, int pY){
    log("draw9 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT / 2);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT);
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    log("draw9 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw8(int pX, int pY){
    log("draw8 begin");
    RECT lRect;
    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT / 2);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT / 2);
    drawLine(lRect);

    lRect.top = pY + (TEXT_MAXHEIGHT);
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + (TEXT_MAXHEIGHT);
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX;
    lRect.right = pX;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.top = pY;
    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    log("draw8 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw0(int pX, int pY){
    log("draw0 begin");
    RECT lRect;
    lRect.left = pX;
    lRect.right = pX;
    lRect.top = pY;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY + TEXT_MAXHEIGHT;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    log("draw0 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw7(int pX, int pY){
    log("draw7 begin");
    RECT lRect;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY;
    lRect.bottom = pY;
    drawLine(lRect);

    lRect.left = pX + TEXT_MAXWIDTH;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    drawLine(lRect);

    log("draw7 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::draw4(int pX, int pY){
    log("draw4 begin");
    RECT lRect;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH;
    lRect.top = pY + ((TEXT_MAXHEIGHT / 4) * 3);
    lRect.bottom = lRect.top;
    drawLine(lRect);

    lRect.top = pY;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    lRect.bottom = pY + TEXT_MAXHEIGHT;
    lRect.left = pX + ((TEXT_MAXWIDTH / 4) * 3);
    lRect.right = lRect.left;
    drawLine(lRect);

    lRect.left = pX;
    lRect.top = pY + + ((TEXT_MAXHEIGHT / 4) *3);
    lRect.right = pX + ((TEXT_MAXWIDTH / 4) * 3);
    lRect.bottom = pY;
    drawLine(lRect);

    log("draw4 end");
    return pX + TEXT_MAXWIDTH + TEXT_PADDING;
}

int PictureClass::drawx(int pX, int pY){
    log("drawx begin");
    RECT lRect;
    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH / 2;
    lRect.top = pY;
    lRect.bottom = pY + TEXT_MAXHEIGHT / 2;
    drawLine(lRect);

    lRect.left = pX;
    lRect.right = pX + TEXT_MAXWIDTH / 2;
    lRect.top = pY + TEXT_MAXHEIGHT / 2;
    lRect.bottom = pY;
    drawLine(lRect);

    log("drawx end");
    return pX + TEXT_MAXWIDTH / 2 + TEXT_PADDING;
}

long PictureClass::getStartBuffer(int pX, int pY){
    return ((pY * gWidth) + pX) * 3;
}

void PictureClass::drawText(int pX, int pY, const char *pText){
    int lX = pX;
    for(unsigned int bPosition=0;bPosition<strlen(pText);bPosition++){
        switch (pText[bPosition]) {
            case '0' : lX = draw0(lX, pY); break;
            case '1' : lX = draw1(lX, pY); break;
            case '2' : lX = draw2(lX, pY); break;
            case '3' : lX = draw3(lX, pY); break;
            case '4' : lX = draw4(lX, pY); break;
            case '5' : lX = draw5(lX, pY); break;
            case '6' : lX = draw6(lX, pY); break;
            case '7' : lX = draw7(lX, pY); break;
            case '8' : lX = draw8(lX, pY); break;
            case '9' : lX = draw9(lX, pY); break;
            case 'x' : lX = drawx(lX, pY); break;
            default : cout << " PictureClass::drawText does not know what to do with " << pText[bPosition] << endl;
        }
    }
    //cout << "Drawing text " << pText << " at " << pX << ", " << pY << endl;

}
void PictureClass::getPixel(int pX, int pY){
    log("getPixel begin");
    memcpy(&gCurrentColour.gColour, gData+getStartBuffer(pX, pY), sizeof(ColourClass::ColourRef));
    log("getPixel end");
}

void PictureClass::setPen(short pRed, short pGreen, short pBlue){
    gPenColour.setRGB(pRed, pGreen, pBlue);
    gCurrentPenStyle = Pen_Solid;
}

void PictureClass::setPixel(int pX, int pY){
    log("setPixel begin");
    if(pX>=gWidth||pY>=gHeight){
        cout << "SetPixel has an amount too high " << pX  << "/" << gWidth << ", " << pY << "/" << gHeight << endl;
        log("setPixel end too high");
        return;
    }
    if(pX<0||pY<0){
        cout << "SetPixel has an amount too low " << pX  << "/" << gWidth << ", " << pY << "/" << gHeight << endl;
        log("setPixel end too low");
        return;
    }
    long lStart = getStartBuffer(pX, pY);
    if(gCurrentPenStyle==Pen_Invert){
        getPixel(pX, pY);
        gPenColour.setRGB(gCurrentColour.gColour.red, gCurrentColour.gColour.green, gCurrentColour.gColour.blue);
				gPenColour.invertColours();
    }
    memcpy(gData+lStart, &gPenColour.gColour, sizeof(ColourClass::ColourRef));
    log("setPixel end main");
}




bool PictureClass::saveJpeg(const char *pFilename){
    log("saveJpeg begin");
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    /* this is a pointer to one row of image data */
    JSAMPROW row_pointer[1];
    FILE *outfile = fopen( pFilename, "wb" );

    if ( outfile==NULL )
    {
        return false;
    }
    cinfo.err = jpeg_std_error( &jerr );
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    /* Setting the parameters of the output file here */
    cinfo.image_width = gWidth;
    cinfo.image_height = gHeight;
    cinfo.input_components = BYTES_PER_PIXEL;
    cinfo.in_color_space = color_space;
    /* default compression parameters, we shouldn't be worried about these */

    jpeg_set_defaults( &cinfo );
    cinfo.num_components = 3;
    //cinfo.data_precision = 4;
    cinfo.dct_method = JDCT_ISLOW;
    jpeg_set_quality(&cinfo, 100, TRUE);
    /* Now do the compression .. */
    jpeg_start_compress( &cinfo, TRUE );
    /* like reading a file, this time write one row at a time */
    while( cinfo.next_scanline < cinfo.image_height )
    {
    row_pointer[0] = &gData[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
    jpeg_write_scanlines( &cinfo, row_pointer, 1 );
    }
    /* similar to read file, clean up after we're done compressing */
    jpeg_finish_compress( &cinfo );
    jpeg_destroy_compress( &cinfo );
    fclose( outfile );
    /* success code is 1! */
    return true;
}

bool PictureClass::loadJpeg(const char *pFilename){
    FILE* file = fopen(pFilename, "rb");  //open the file
    struct jpeg_decompress_struct info;  //the jpeg decompress info
    struct jpeg_error_mgr err;           //the error handler

    info.err = jpeg_std_error(&err);     //tell the jpeg decompression handler to send the errors to err
    jpeg_create_decompress(&info);       //sets info to all the default stuff
    //if the jpeg file didnt load exit
    if(!file)
    {
      return false;
    }

    jpeg_stdio_src(&info, file);    //tell the jpeg lib the file we'er reading

    jpeg_read_header(&info, TRUE);   //tell it to start reading it

    //if it wants to be read fast or not

    jpeg_start_decompress(&info);    //decompress the file

    //set the x and y
    gWidth = info.output_width;
    gHeight = info.output_height;

    gSize = gWidth * gHeight* 3;

    gData = new BYTE[gSize];      //setup data for the data its going to be handling

    //read the scan lines
    BYTE* p1 = gData;
    BYTE** p2 = &p1;
    int numlines = 0;

    while(info.output_scanline < info.output_height)
    {
      numlines = jpeg_read_scanlines(&info, p2, 1);
      *p2 += numlines * 3 * info.output_width;
    }

    jpeg_finish_decompress(&info);   //finish decompressing this file
    fclose(file);                    //close the file

    return true;
  }

