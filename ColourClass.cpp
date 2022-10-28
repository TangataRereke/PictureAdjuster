#include "ColourClass.h"

void ColourClass::setRGB(short pRed, short pGreen, short pBlue){
	gColour.red = pRed;
	gColour.green = pGreen;
	gColour.blue = pBlue;
}

short ColourClass::getMax(){
    return max(max(gColour.red, gColour.green),gColour.blue);
};

short ColourClass::getMin(){
    return min(min(gColour.red, gColour.green),gColour.blue);
};

short ColourClass::getSaturation(){
		return getMax() - getMin();
};

short ColourClass::getHue(){
	short lMax = getMax();
	short lMin = getMin();
	double lHue = 0;
	short lDelta = lMax - lMin;
	if(lDelta==0){
		return -1;
	}
	if(lMax==gColour.red){
		lHue = (gColour.green - gColour.blue) / lDelta;
	}else if(lMax==gColour.green){
		lHue = 2 + (gColour.blue - gColour.red) / lDelta;
	}else{
		lHue = 4 + (gColour.red - gColour.green) / lDelta;
	}
	lHue *= 60;
	if(lHue>=0){
		return lHue;
	}
	return lHue += 360;
}

short ColourClass::getValue(){
	return getMax();
}


bool ColourClass::isSkin(){
		if(getSaturation()<=13){
            return false;
		}
		double lPercent1 = 0;
		double lPercent2 = 0;
		short lMax = getMax();
		if(lMax<40){
			return false;
		}else if(gColour.blue==lMax){
        //    cout << "C" << endl;
            return false;
		}else if(gColour.red==lMax){
			if(gColour.green>=gColour.blue){
				lPercent1 = (double)((double)gColour.green / (double)gColour.red);
				lPercent2 = (double)((double)gColour.blue / (double)gColour.green);
				if(lPercent1>=.37&&lPercent1<=.80){
					return true;
				}else if(lPercent2>=.63){//}&&lPercent2<=.89){
					return true;
				}
			}else{
				lPercent1 = (double)((double)gColour.blue / (double)gColour.red);
				lPercent2 = (double)((double)gColour.green / (double)gColour.blue);
				if(lPercent1>=.42){//&&lPercent1<=.90){
					if(lPercent2>=.70){
						return true;
					}
				}
			}
		}
        //cout << "D" << endl;
		return false;
}


void ColourClass::setFromColourRef(ColourRef pSetFrom){
	memcpy(&gColour, &pSetFrom, sizeof(ColourRef));
}

bool ColourClass::isMajorChange(ColourRef pCompare){
	ColourClass lClass;
	lClass.setFromColourRef(pCompare);
	memcpy(&pCompare, &lClass.gColour, sizeof(ColourRef));
	double lRGThis = 0.0000;
	double lGBThis = 0.0000;
	double lRBThis = 0.0000;
	double lRGOther = 0.0000;
	double lGBOther = 0.0000;
	double lRBOther = 0.0000;
    if(gColour.red>0||gColour.green>0){
        if(gColour.red>gColour.green){
            lRGThis = (double)gColour.green / (double)gColour.red;
        }else{
            lRGThis = (double)gColour.red / (double)gColour.green;
        }
    }
    if(gColour.green>0||gColour.blue>0){
        if(gColour.green>gColour.blue){
            lGBThis = (double)gColour.blue / (double)gColour.green;
        }else{
            lGBThis = (double)gColour.green / (double)gColour.blue;
        }
    }
    if(gColour.red>0||gColour.blue>0){
        if(gColour.red>gColour.blue){
            lRBThis = (double)gColour.blue / (double)gColour.red;
        }else{
            lRBThis = (double)gColour.red / (double)gColour.blue;
        }
    }
    if(pCompare.red>0||pCompare.green>0){
        if(pCompare.red>pCompare.green){
            lRGOther = (double)pCompare.green / (double)pCompare.red;
        }else{
            lRGOther = (double)pCompare.red / (double)pCompare.green;
        }
    }
    if(pCompare.green>0||pCompare.blue>0){
        if(pCompare.green>pCompare.blue){
            lGBOther = (double)pCompare.blue / (double)pCompare.green;
        }else{
            lGBOther = (double)pCompare.green / (double)pCompare.blue;
        }
    }
    if(pCompare.red>0||pCompare.blue>0){
        if(pCompare.red>pCompare.blue){
            lRBOther = (double)pCompare.blue / (double)pCompare.red;
        }else{
            lRBOther = (double)pCompare.red / (double)pCompare.blue;
        }
    }

    double lFactor1 = 0.0000;
    double lFactor2 = 0.0000;
    double lFactor3 = 0.0000;
    if(lRGThis>lRGOther){
        lFactor1 = lRGThis - lRGOther;
    }else{
        lFactor1 = lRGOther - lRGThis;
    }
    if(lGBThis>lGBOther){
        lFactor2 = lGBThis - lGBOther;
    }else{
        lFactor2 = lGBOther - lGBThis;
    }
    if(lRBThis>lRBOther){
        lFactor3 = lRBThis - lRBOther;
    }else{
        lFactor3 = lRBOther - lRBThis;
    }

    // TO DO IF required, detect highest values - dopn't think its required
    double lFactor = 0.55;// still doing second
    if(lFactor1>lFactor||lFactor2>lFactor||lFactor3>lFactor){
        return true;
    }
    return false;

/*
    if(getSaturation()>16&&lClass.getSaturation()>16){
		int lThisHue = getHue();
		int lOtherHue = lClass.getHue();
		int lHueDifference = max(lThisHue, lOtherHue) - min(lThisHue, lOtherHue);
		if(max(lThisHue+360, lOtherHue+360)-min(lThisHue, lOtherHue)<lHueDifference){
            lHueDifference = max(lThisHue+360, lOtherHue+360)-min(lThisHue, lOtherHue);
		}
		if(lHueDifference>14){
			//return true;
		}
        return false;
    }
    return true;
    short lThisBrightness = getBrightness();
    short lOtherBrightness = lClass.getBrightness();
    short lTotal = 0;
    if(lThisBrightness>lOtherBrightness){
        lTotal = (gColour.red - pCompare.red) + (gColour.green - pCompare.green) + (gColour.blue - pCompare.blue);
    }else{
        lTotal = (pCompare.red - gColour.red) + (pCompare.green - gColour.green) + (pCompare.blue - gColour.blue);
    }
    return abs(lTotal) > 128;
	short lTotal = max(pCompare.red, gColour.red)-min(pCompare.red, gColour.red);
	lTotal = lTotal + max(pCompare.green, gColour.green)-min(pCompare.green, gColour.green);
	lTotal = lTotal + max(pCompare.blue, gColour.blue)-min(pCompare.blue, gColour.blue);
	return lTotal > 128;*/
	/*ColourClass lClass;
	memcpy(&pCompare, &lClass.gColour, sizeof(ColourRef));
	if(getSaturation()>32){
		int lThisHue = getHue();
		int lOtherHue = lClass.getHue();
		int lHueDifference = max(lThisHue, lOtherHue) - min(lThisHue, lOtherHue);
		if(max(lThisHue+360, lOtherHue+360)-min(lThisHue, lOtherHue)<lHueDifference){
            lHueDifference = max(lThisHue+360, lOtherHue+360)-min(lThisHue, lOtherHue);
		}
		if(lHueDifference>14){
			return true;
		}
	}
	int lDiffBrightness =  max(getBrightness(), lClass.getBrightness())- min(getBrightness(), lClass.getBrightness());
	return lDiffBrightness > 90;*/
}

const char *ColourClass::getDisplay(){
	stringstream lSS;
	lSS << "R" << (int)gColour.red << "G" << (int)gColour.green << "B" << (int)gColour.blue;
	return lSS.str().c_str();
}

bool ColourClass::isMinorChange(ColourRef pCompare){
	short lTotal = max(pCompare.red, gColour.red)-min(pCompare.red, gColour.red);
	lTotal = lTotal + max(pCompare.green, gColour.green)-min(pCompare.green, gColour.green);
	lTotal = lTotal + max(pCompare.blue, gColour.blue)-min(pCompare.blue, gColour.blue);
	return lTotal > 3;
}

int ColourClass::simplifyNumber(int pNumber){
	if(pNumber<32){
		return 0;
	}else if(pNumber<64){
		return 32;
	}else if(pNumber<96){
		return 64;
	}else if(pNumber<128){
		return 96;
	}else if(pNumber<160){
		return 128;
	}else if(pNumber<192){
		return 160;
	}else if(pNumber<224){
		return 192;
	}else if(pNumber<255){
		return 224;
	}
	return 255;
}

void ColourClass::simplifyColour(){
	gColour.red = simplifyNumber(gColour.red);
	gColour.green = simplifyNumber(gColour.green);
	gColour.blue = simplifyNumber(gColour.blue);
}

bool ColourClass::isTintable(){
	return getSaturation() > 33;
}

ColourRef ColourClass::applyTint(ColourRef pTint){
	int lColour = gColour.red - pTint.red;
	gColour.red = max(lColour, 0) / 2;
	lColour = gColour.green - pTint.green;
	gColour.green = max(lColour, 0) / 2;
	lColour = gColour.blue - pTint.blue;
	gColour.blue = max(lColour, 0) / 2;
	return gColour;
}

ColourRef ColourClass::checkTint(ColourRef pMainTint, bool &pStop){
	ColourRef lReturn;
	memcpy(&lReturn, &pMainTint, sizeof(ColourRef));
	if(gColour.red<lReturn.red){
		lReturn.red = gColour.red;
	}
	if(gColour.green<lReturn.green){
		lReturn.green = gColour.green;
	}
	if(gColour.blue<lReturn.blue){
		lReturn.blue = gColour.blue;
	}
	if(lReturn.red==0&&lReturn.green==0&&lReturn.blue==0){
		pStop = true;
	}
	return lReturn;
}

void ColourClass::setBrightness(double pNewBrightness){
	double lBrightness = getBrightness();
	if(lBrightness<pNewBrightness){
		while(lBrightness<pNewBrightness){
			if(gColour.red==255||gColour.green==255||gColour.blue==255){
				return;
			}
			gColour.red++;
			gColour.green++;
			gColour.blue++;
			lBrightness = getBrightness();
		}
	}else if(lBrightness>pNewBrightness){
		while(lBrightness>pNewBrightness){
			if(gColour.red==0||gColour.green==0||gColour.blue==0){
				return;
			}
			gColour.red--;
			gColour.green--;
			gColour.blue--;
			lBrightness = getBrightness();
		}
	}else{
     //   cout << lBrightness << "==" << pNewBrightness << endl;
	}
}

double ColourClass::getMinimumBalance(){
    return (getBrightness() - getMin())+1;
}


double ColourClass::getBrightness(){
    return getMax();
/*		short lLowest = getMin();
		if(lLowest==gColour.red){
			lLowest = gColour.red;
		}else if(lLowest==gColour.green){
			lLowest = gColour.green;
		}else{
			lLowest = gColour.blue;
		}
		return ((double) (gColour.red - lLowest)+(gColour.green - lLowest)+(gColour.blue - lLowest))/3;*/
}

double ColourClass::getMaximumBalance(){
	return 255;
}

void ColourClass::invertColours(){
	gColour.red = 255 - gColour.red;
	gColour.blue = 255 - gColour.blue;
	gColour.green = 255 - gColour.green;
}
