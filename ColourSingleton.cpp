#include "ColourSingleton.h"

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
    /*cout << "Generating code" << endl;
    ofstream headerFile("/home/james/Documents/Code/AutoConvertPictures/ColourClass.h");
    ofstream sourceFile("/home/james/Documents/Code/AutoConvertPictures/ColourClass.cpp");
    headerFile << "#ifndef COLOURCLASS_H" << endl << "#define COLOURCLASS_H" << endl << "#include <math.h>" << endl << "#include <iostream>" << endl;
    headerFile << "struct ColourRef{" << endl;
    headerFile << "\tshort red;" << endl;
    headerFile << "\tshort green;" << endl;
    headerFile << "\tshort blue;" << endl;
    headerFile << "};" << endl << endl;
    headerFile << "class ColourClass{" << endl << "\tpublic:" << endl;
    headerFile << "\t\tColourRef gColour; " << endl;
    headerFile << "\t\tvoid setRGB(short pRed, short pGreen, short pBlue);" << endl;

		const char *lHeaderStart = "\t\tshort ";
		const char *lHeaderEnd = "();\r\n";
		const char *lCodeHeaderStart = "short ColourClass::";
		const char *lCodeHeaderEnd = "(){\r\n";
		const char *lCodeEnd = "\r\n};\r\n\r\n";
		//const 

		sourceFile << "#include \"ColourClass.h\"" << endl << endl;
		
		sourceFile << "void ColourClass::setRGB(short pRed, short pGreen, short pBlue){\r\n\tgColour.red = pRed;\r\n\tgColour.green = pGreen;\r\n\tgColour.blue = pBlue;\r\n}\r\n" << endl;
		
		cout << "Saturation" << endl;
		
		headerFile << lHeaderStart << "getMax" << lHeaderEnd;
		sourceFile << lCodeHeaderStart << "getMax" << lCodeHeaderEnd << "\tif(gColour.red>gColour.green){" << endl << "\t\tif(gColour.red>gColour.blue){" << endl;
		sourceFile << "\t\t\treturn gColour.red;" << endl << "\t\t}else{" << endl << "\t\t\treturn gColour.blue;" << endl << "\t\t}" << endl << "\t}else if(gColour.green>gColour.blue){" << endl;
		sourceFile << "\t\treturn gColour.green;" << endl << "\t" << "}" << endl << "\treturn gColour.blue;"<< lCodeEnd;
	
		headerFile << lHeaderStart << "getMin" << lHeaderEnd;
		sourceFile << lCodeHeaderStart << "getMin" << lCodeHeaderEnd << "\tif(gColour.red<gColour.green){" << endl << "\t\tif(gColour.red<gColour.blue){" << endl;
		sourceFile << "\t\t\treturn gColour.red;" << endl << "\t\t}else{" << endl << "\t\t\treturn gColour.blue;" << endl << "\t\t}" << endl << "\t}else if(gColour.green<gColour.blue){" << endl;
		sourceFile << "\t\treturn gColour.green;" << endl << "\t" << "}" << endl << "\treturn gColour.blue;"<< lCodeEnd;
		
		headerFile << lHeaderStart << "getSaturation" << lHeaderEnd;
		
	
		sourceFile << lCodeHeaderStart << "getSaturation" << lCodeHeaderEnd << "\t\treturn getMax() - getMin();" << lCodeEnd;
		
		
		cout << "Is Skin" << endl;
		headerFile << "\t\tbool isSkin" << lHeaderEnd;
		*/
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
		/*sourceFile << "bool ColourClass::isSkin(){" << endl;
		SortedColour *lSColour = gAllSortedColours;
		short lStartRed = -1;
		short lStartGreen = -1;
		short lStartBlue = -1;
		short lLastRed = -1;
		short lLastGreen = -1;
		short lLastBlue = -1;
		short lNextRed = -1;
		short lNextGreen = -1;
		short lNextBlue = -1;
		bool lFirst = true;
		
		while(lSColour!=0){
			if(lSColour->sortValue==0){
				cout << lSColour->colour.red << "," << lSColour->colour.green << "," << lSColour->colour.blue << " | " << lSColour->sortValue << endl;
			}
			if(lLastRed!=lSColour->colour.red){
				if(lSColour->sortValue==1){
					if(lStartRed>=0){
						if(!lFirst){
							sourceFile << "\t} else ";
						}else{
							lFirst = false;
						}
						sourceFile << "\tif(gColour.red>=" << lStartRed << "&&gColour.red<=" << lNextRed << "&&gColour.green>=" << lStartGreen << "&&gColour.green<=";
						sourceFile << lNextGreen << "&&gColour.blue>=" << lStartBlue << "&&gColour.blue<=" << lNextBlue << "){" << endl << "\t\treturn false;" << endl;
					}
					lStartRed = -1;
					lStartGreen = -1;
					lStartBlue = -1;
					lNextRed = -1;
					lNextGreen = -1;
					lNextBlue = -1;
				}
			}
			if(lSColour->sortValue==0){
				if(lStartRed==-1){
					lStartRed = lSColour->colour.red;
				}
				if(lStartGreen==-1){
					lStartGreen = lSColour->colour.green;
				}
				if(lStartBlue==-1){
					lStartBlue = lSColour->colour.blue;
				}
				
				lNextRed = lSColour->colour.red;
				lNextGreen = lSColour->colour.green;
				lNextBlue = lSColour->colour.blue;
			}
			lLastRed = lSColour->colour.red;
				lLastGreen = lSColour->colour.green;
				lLastBlue = lSColour->colour.blue;
			SortedColour *dColour = lSColour;
			lSColour = lSColour->next;
			delete dColour;
		}
		cout << "Start Red: " << lStartRed << "Next red: " << lNextRed << endl;
		if(lLastRed>=0){
			if(lStartRed>0){
				if(!lFirst){
					sourceFile << "} else ";
				}else{
					lFirst = false;
				}
				sourceFile << "if(gColour.red>=" << lStartRed << "&&gColour.red<=" << lNextRed << "&&gColour.green>=" << lStartGreen << "&&gColour.green<=";
				sourceFile << lNextGreen << "&&gColour.blue>=" << lStartBlue << "&&gColour.blue<=" << lNextBlue << "){" << endl << "\t\treturn false;" << endl;
			}
		}
		
		sourceFile << "\r\n\t}\r\n\treturn true;" << endl << "}\r\n\r\n";
		
		
		
    sourceFile.close();

		headerFile << "};" << endl << endl << "#endif // COLOURCLASS_H" << endl;
    headerFile.close();
    cout << "Code generated" << endl;
    return;
    ofstream out("/home/james/Documents/Code/AutoConvertPictures/ColourClass.cpp");
    out << "struct ColourRef{" << endl;
    out << "\tunsigned char red;" << endl;
    out << "\tunsigned char green;" << endl;
    out << "\tunsigned char blue;" << endl;
    out << "};" << endl << endl;


    out << "class ColourClass" << endl << "{" << endl;
    out << "\tdouble getSaturation(ColourRef pColour);" << endl;
    Colour *bColour = gAllColours;
    bool lFirst = true;
    while(bColour!=0){
        out << "\t";
        if(lFirst){
            out << "if";
            lFirst = false;
        }else if(bColour->gNextAll==0){
            out << "else";
        }else{
            out <<"elseif";
        }
    	bColour = bColour->gNextAll;
        out << "(pColour.red==" << (int)bColour->gRed << "&&pColour.green==" << (int)bColour->gGreen << "&&pColour.blue==" << (int)bColour->gBlue << "){ return " << (int)bColour->gSaturation << ";}"<< endl;
    }
    cout << "Got to the end " << endl;
    out << "};" << endl << endl;
    out << "double ColourClass::getSaturation(ColourRef pColour){" << endl;

    out << ")" << endl;
    out.close();*/
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
