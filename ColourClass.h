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
        double getMaximumBalancePossible();
        double getMinimumBalancePossible();
        void invertColours();
        double getMaximumBalance();
        double getMinimumBalance();
        double getBrightness();
        short getMax();
        short getMin();
        short getValue();
        bool isSkin();
        void setFromColourRef(ColourRef pSetFrom);
        bool isMajorChange(ColourRef pCompare);
        const char *getDisplay();
        bool isMinorChange(ColourRef pCompare);
        void setBrightness(double);
        ColourRef gColour;
        ColourRef checkTint(ColourRef pMainTint, bool & pStop);
        bool isTintable();
        void simplifyColour();
        void setRGB(short pRed, short pGreen, short pBlue);
        short getSaturation();
        short getHue();
        int simplifyNumber(int pNumber);
        ColourRef applyTint(ColourRef);
    private:
};