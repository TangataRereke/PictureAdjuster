#include <memory.h>
#include <sstream>
#include <iostream>

using namespace std;

class ColourClass{
    public:
        struct ColourRef {
            char unsigned red;
            char unsigned green;
            char unsigned blue;
        };
        ColourRef applyTint(ColourRef pTint);
        ColourRef checkTint(ColourRef pMainTint, bool &pStop);
        short getMax();
        short getMin();
        const char *getDisplay();
        ColourRef gColour;
        bool isTintable();
        void setRGB(short pRed, short pGreen, short pBlue);
        void setInteger(long unsigned pColour);
        void setHue(short unsigned gHue);
        short unsigned getHue(){return gHue;};
        void setSaturation(double pValue);
        double getSaturation(){return gSaturation;};
        bool isMinorChange(ColourRef pCompare);
        void setBrightness(double pBrightness);
        bool isSkin();
        double getBrightness(){ return gBrightness;}

    private:
        short unsigned getCalculatedHue();
        void setRatiosBasedOffRGB();
        double getMaxRange();
        double getMidRange();
        double getMinRange();
        short unsigned gHue = 0;
        double gPercentRed = 0;
        double gPercentGreen = 0;
        double gPercentBlue = 0;
        double gOriginalRed = 0;
        double gOriginalGreen = 0;
        double gOriginalBlue = 0;
        double gSaturation = 0;
        double gBrightness = 0;
};