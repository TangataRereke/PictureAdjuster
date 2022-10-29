ColourSingleton::ColourSingleton()
{
    
    cout << "Initialising colours" << endl;
    for(int bColour=0;bColour<16777216;bColour++){
        if(bColour % 100000==0){
      //      cout << "Up to " << bColour << endl;
        }
        new Colour(bColour, this);
    }
    cout << "Resynching colours" << endl;
    Colour *bColour = gAllColours;
    while(bColour!=0){
        bColour->resynch();
        bColour = bColour->gNextAll;
    }
    cout << "Extracting colours " << endl;
    //extractTable();
   // generateCode();
    //ctor
}

void ColourSingleton::generateCode(){
   
		cout << "\tSorting colours" << endl;
    ofstream testFile("/home/james/Documents/Code/AutoConvertPictures/skinColours.csv");
		
		Colour *lColour = gAllColours;
		while(lColour!=0){
			//newSortedColour(lColour->gRed, lColour->gGreen, lColour->gBlue, (double)lColour->isSkinColour);
			ColourClass lClass;
			lClass.setRGB(lColour->gRed, lColour->gGreen, lColour->gBlue);
			if(lColour->isSkinColour){
				if(!lClass.isSkin()){
					cout << "UH OH" << (int)lColour->gRed << "," << (int)lColour->gGreen << "," << (int)lColour->gBlue << endl;
				}
			}else{
				if(lClass.isSkin()){
					testFile << (int)lColour->gRed << "," << (int)lColour->gGreen << "," << (int)lColour->gBlue << endl;
				}
			}
			lColour = lColour->gNextAll;
		}
		testFile.close();

    cout << "Code generated" << endl;
}

void ColourSingleton::newSortedColour(short pRed, short pGreen, short pBlue, double pValue){
	SortedColour *lNewColour = new SortedColour;
	lNewColour->colour.red = pRed;
	lNewColour->colour.green = pGreen;
	lNewColour->colour.blue = pBlue;
	lNewColour->sortValue = pValue;
	SortedColour *lLast = 0;
	
	SortedColour *lIterColour = gAllSortedColours;
	//cout << "Adding colour " << pRed << "," << pGreen << "," << pBlue << endl;
	while(lIterColour!=0){
		if(lIterColour->sortValue==pValue){
			if(pRed>lIterColour->colour.red){
				if(pGreen>lIterColour->colour.green){
					if(pBlue>lIterColour->colour.blue){
						lNewColour->previous = lIterColour->previous;
						lNewColour->next = lIterColour;
						if(gAllSortedColours==lIterColour){
							lIterColour = lNewColour;
						}
						lIterColour->previous = lNewColour;
						return;
					}
				}
			}
		}else if(lIterColour->sortValue>pValue){
				lNewColour->previous = lIterColour->previous;
				lNewColour->next = lIterColour;
				if(gAllSortedColours==lIterColour){
					lIterColour = lNewColour;
				}
				lIterColour->previous = lNewColour;
				return;
		}
		lLast = lIterColour;
		lIterColour = lIterColour->next;
	}
	lNewColour->previous = lLast;
	lNewColour->next = 0;
	if(lLast!=0){
		lLast->next = lNewColour;
	}else{
		gAllSortedColours = lNewColour;
	}
	return;
}


void ColourSingleton::extractTable(){
    ofstream out("/home/james/Documents/Code/AutoConvertPictures/colourTable.csv");
    out << "Colour,Red,Green,Blue,Hue,Saturation,Value,Brightness,Min Balance,Max Balance,Prev Bright Red,Prev Bright Green,Prev Bright Blue,Next Bright Red,Next Bright Green,Next Bright Blue,Invert Red,Invert Green,Invert Blue,Skin Colour" << endl;
    Colour *lColour = gAllColours;
    while(lColour!=0){
        out << lColour->gColour << "," << (int)lColour->gRed << "," << (int)lColour->gGreen << "," << (int)lColour->gBlue << "," << lColour->gHue << "," << lColour->gSaturation << "," << lColour->gValue << "," << lColour->gBrightness << "," << lColour->gMinBrightness << "," << lColour->gMaxBrightness << "," << (int)lColour->gPreviousBrightRed << "," << (int)lColour->gPreviousBrightGreen << "," << (int)lColour->gPreviousBrightBlue << "," << (int)lColour->gNextBrightRed << "," << (int)lColour->gNextBrightGreen << "," << (int)lColour->gNextBrightBlue << "," << (int)lColour->gInvertedRed << "," << (int)lColour->gInvertedGreen << "," << (int)lColour->gInvertedBlue << "," << lColour->isSkinColour << endl;
        lColour = lColour->gNextAll;
    }
    out.close();
}

void ColourSingleton::insertColourHue(Colour *pColour){

}

Colour *ColourSingleton::getHSVColour(double pHue, double pSaturation, double pValue){
    return 0;
}


Colour *ColourSingleton::getColour(int pRed, int pGreen, int pBlue){
    Colour *lRColour = 0;
    if(pRed>128){
        lRColour = gLastColourByRed;
        while(lRColour!=0){
            if(lRColour->gRed==pRed){
                break;
            }
            lRColour = lRColour->gPreviousRedColour;
        }
    }else{
        lRColour = gAllColoursByRed;
        while(lRColour!=0){
            if(lRColour->gRed==pRed){
                break;
            }
            lRColour = lRColour->gNextRedColour;
        }
    }
    if(lRColour==0){
        return gAllColoursByRed;
    }
    Colour *lGColour = lRColour->gNextGreenColour;
    while(lGColour!=0){
        if(lGColour->gGreen==pGreen){
            break;
        }
        lGColour = lGColour->gNextGreenColour;
    }
    if(lGColour==0){
        return lRColour->gNextGreenColour;
    }
    Colour *lBColour = lGColour->gNextBlueColour;
    while(lBColour!=0){
        if(lBColour->gBlue==pBlue){
            return lBColour;
        }
        lBColour = lBColour->gNextBlueColour;
    }
    return lGColour->gNextBlueColour;
}

ColourSingleton::~ColourSingleton()
{
    cout << "Deleting colours" << endl;
    Colour *lColour = gAllColours;
    while(lColour!=0){
        Colour *lNext = lColour->gNextAll;
        delete lColour;
        lColour = lNext;
    }
    //dtor
}
