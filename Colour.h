class ColourSingleton;
class Colour{
    public:
        Colour(int pColourNumber, ColourSingleton *pSingleton);
        ~Colour();

        void resynch();
        short gRed = 0;
        short gGreen = 0;
        short gBlue = 0;
        double gBrightness = 0;
        double gMinBrightness = 0;
        double gMaxBrightness = 0;

        short gPreviousBrightRed = 0;
        short gPreviousBrightGreen = 0;
        short gPreviousBrightBlue = 0;
        short gNextBrightRed = 0;
        short gNextBrightGreen = 0;
        short gNextBrightBlue = 0;
        short gInvertedRed = 0;
        short gInvertedGreen = 0;
        short gInvertedBlue = 0;
        int gColour = 0;
        double gValue = 0;
        double gSaturation = 0;
        double gHue = 0;

        bool isSkinValue = false;
        bool isSkinHue = false;
        bool isSkinSaturation = false;
        bool isSkinColour = false;

        Colour *gNextAll = 0;

        ColourSingleton *gSingleton = 0;
    private:
};