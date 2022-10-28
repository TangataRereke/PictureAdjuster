#include "Colour.h"

Colour::Colour(int pColourNumber, ColourSingleton *pSingleton)
{
    gSingleton = pSingleton;
    gColour = pColourNumber;
    gNextAll = 0;

    int gWorkingColour = gColour;
    gBlue = floor(gWorkingColour / 65536);
    gWorkingColour = gWorkingColour - (gBlue * 65536);
    gGreen = floor(gWorkingColour / 256);
    gRed = gWorkingColour - (gGreen * 256);

    float lMin = 0, lMax = 1, lDelta = 0;
    gBrightness = 1.012334;
    gBrightness = ((double)(gRed + gGreen + gBlue) / (double)255) / (double)3;
    lMin = min(min(gRed, gGreen), gBlue);
    lMax = max(max(gRed, gGreen), gBlue);
    gValue = lMax;
    lDelta = lMax - lMin;
    if(lMax!=0){
        gSaturation = lMax - lMin;
        if(gRed == lMax){
            gHue = (gGreen - gBlue) / lDelta;
        }else if(gGreen == lMax){
            gHue = 2 + (gBlue - gRed) / lDelta;
        }else{
            gHue = 4 + (gRed - gGreen) / lDelta;
        }
        gHue *= 60;
        if(gHue < 0){
            gHue += 360;
        }
    }else{
        gHue = -1;
    }
    gValue = gValue / 255;
    gSaturation = gSaturation / 255;

    isSkinValue = gValue >= .16;
    isSkinSaturation = gSaturation>=.03&&gSaturation<=.55;
    isSkinHue = !(gHue>70&&gHue<295);
    isSkinColour = isSkinHue && isSkinValue && isSkinValue;

    // Setup relationships

/*
        Colour *gNextBrightColour = 0;
        Colour *gPreviousBrightColour = 0;
        Colour *gNextValueColour = 0;
        Colour *gPreviousValueColour = 0;
        /Colour *gNextHueColour = 0;
        /Colour *gPreviousHueColour = 0;
        Colour *gNextSaturationColour = 0;
        Colour *gPreviousSaturationColour = 0;
        Colour *gNextRedColour = 0;
        Colour *gNextGreenColour = 0;
        Colour *gNextBlueColour = 0;
        Colour *gPreviousRedColour = 0;
        Colour *gPreviousGreenColour = 0;
        Colour *gPreviousBlueColour = 0;*/



    // Setup singletons
   /*   /Colour *gAllColours = 0;
        /Colour *gLastAllColours = 0;
        Colour *gAllColoursByRed = 0;
        Colour *gAllColoursByGreen = 0;
        Colour *gAllColoursByBlue = 0;
        Colour *gLastColourByRed = 0;
        Colour *gLastColourByGreen = 0;
        Colour *gLastColourByBlue = 0;
        /Colour *gAllColoursByHue = 0;
        /Colour *gLastColourByHue = 0;*/


    if(gSingleton->gAllColours==0){
        gSingleton->gAllColours = this;
    }else{
        gSingleton->gLastAllColours->gNextAll = this;
        gPreviousAll = gSingleton->gLastAllColours;
    }

    gSingleton->gLastAllColours = this;

    gSingleton->insertColourHue(this);
    //cout << "i: " << gColour << "  R: " << (int) gRed << "  G: " << (int) gGreen << "  B: " << (int) gBlue <<  "  H: " << gHue << "  S: " << gSaturation << "  V: " << gValue << "  Br: " << gBrightness << endl;
    //ctor
}

void Colour::resynch(){
    // Calculate the brightnesses
    /*int lRed = gRed - 1;
    int lGreen = gGreen - 1;
    int lBlue = gBlue - 1;
    if(lRed>=0&&lGreen>=0&&lBlue>=0){
        gPreviousBrightColour = gSingleton->getColour(lRed, lGreen, lBlue);
    }

    lRed = gRed+1;
    lGreen = gGreen+1;
    lBlue = gBlue+1;
    if(lRed<=255&&lGreen<=255&&lBlue<=255){
        gNextBrightColour = gSingleton->getColour(lRed, lGreen, lBlue);
    }*/

    int lRed = gRed - 1;
    int lGreen = gGreen - 1;
    int lBlue = gBlue - 1;
    while(lRed>=0&&lGreen>=0&&lBlue>=0){
        lRed--;
        lGreen--;
        lBlue--;
    }
    if(lRed<0||lGreen<0||lBlue<0){
        lRed++;
        lGreen++;
        lBlue++;
    }
    gMinBrightness = (double)(lRed + lGreen + lBlue) / (double)255 / (double)3;

    lRed = gRed + 1;
    lGreen = gGreen + 1;
    lBlue = gBlue + 1;
    while(lRed<=255&&lGreen<=255&&lBlue<=255){
        lRed++;
        lGreen++;
        lBlue++;
    }
    if(lRed>255||lGreen>255||lBlue>255){
        lRed--;
        lGreen--;
        lBlue--;
    }
    gMaxBrightness = (double)(lRed + lGreen + lBlue) / (double)255 / (double)3;

    lRed = gRed - 1;
    lGreen = gGreen - 1;
    lBlue = gBlue - 1;
    if(lRed<00||lGreen<0||lBlue<0){
        lRed++;
        lGreen++;
        lBlue++;
    }
    gPreviousBrightRed = lRed;
    gPreviousBrightGreen = lGreen;
    gPreviousBrightBlue = lBlue;

    lRed = gRed + 1;
    lGreen = gGreen + 1;
    lBlue = gBlue + 1;
    if(lRed>255||lGreen>255||lBlue>255){
        lRed--;
        lGreen--;
        lBlue--;
    }
    gNextBrightRed = lRed;
    gNextBrightGreen = lGreen;
    gNextBrightBlue = lBlue;

    gInvertedRed = 255 - gRed;
    gInvertedGreen = 255 - gGreen;
    gInvertedBlue = 255 - gBlue;
}

Colour::~Colour()
{
    //dtor
}
