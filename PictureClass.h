#include <stdio.h>
#include <jpeglib.h>
#include <cstring>
#ifndef COLOURCLASS_H
    #include "ColourClass.h"
    #define COLOURCLASS_H
#endif
#include <sstream>
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

typedef unsigned char BYTE;

using namespace std;

class PictureClass{
    public:
        struct RECT{
            int left = 0;
            int top = 0;
            int right = 0;
            int bottom = 0;
        };
        struct Point{
            int x = 0;
            int y = 0;
            Point *next = 0;
        };

        struct ColourItemSort{
            double lookupValue = 0;
            Point *lastPoint = 0;
            ColourItemSort *previous = 0;
            ColourItemSort *next = 0;
            Point *allPoints = 0;
            short newValue = 0;
            long long count = 0;
        };
        ColourItemSort *gAllColourSortItems = 0;
        ColourItemSort *gAllNonColourSortItems = 0;

        const int TEXT_MAXHEIGHT = 20;
        const int TEXT_PADDING = 5;
        const int TEXT_MAXWIDTH = 500;

        BYTE *gData = 0;
        long long gSortItemColourCount = 0;
        long long gSortItemNonColourCount = 0;

        void deleteNonColourItems();

        long long gHeight = 0;
        long long gWidth = 0;
        long long unsigned gSize = 0;
        bool gIndividualLog = false;
        RECT gLastRect;
        int gCurrentPenStyle = 0;
        
        const int Pen_Invert = -1;
        const int Pen_Solid = 0;
        bool gLastLeftCrop = false;
        bool gLastPadSide = false;
        bool gLastPadTop = false;
        bool gLastTopCrop = false;

        ~PictureClass();
        PictureClass();
        void smartResize(int pNewWidth, int pNewHeight);
        void padResize(int pNewWidth, int pNewHeight);
        int draw0(int pX, int pY);
        bool autoCropTopBottom();
        void padRight();
        void padSide();
        void padHeight();
        void padLeft();
        void padBottom();
        void padTop();
        void skinDetect();
        bool autoCropSide();
        void chopColumn(int pColumn);
        void chopRow(int pRow);
        void lowRatioResize(int pNewWidth, int pNewHeight);
        void setPen(int pPen);
        bool hasRowGotSkin(int pRow);
        bool hasColumnGotSkin(int pColumn);
        void showSkin();
        void lineTo(int pX, int pY);
        void drawLine(RECT pRect);
        void newImage(int pWidth, int pHeight);
        short getNewColourValueFor(short pOldValue);
        short getNewNonColourValueFor(short pOldValue);
        void removeDuplicatePixels();
        void removeTint();
        bool removeInterferenceX();
        bool removeInterferenceY();
        void removeInterference();
        double getHighestColourMax();
        double getLowestColourMin();
        double changeBrightnessOfItem(bool pFirst, ColourItemSort *pColourSortItem, double pTargetBrightness, double pFactor);
        double autoBrightenSmallFactor(double pHighestBrightness, double pSmallFactor);
        void lastResortAutoBrighten(double pColourMax);
        bool autoBrighten(double pHighestBrightness, double pFactor, double pTryAgainAmount);
        void autoLighten();
        int recolourGradientFrom(int pX, int pY);
        void insertNonColourSortItem(short pLookupValue);
        void drawRectangle(RECT);
        void showSorts();
        void insertColourSortItem(double pLookupValue, int pX, int pY);
        void clearPicture();
        bool loadJpeg(const char*);
        bool saveJpeg(const char*);
        void bestRatioResize(int, int);
        void setPixel(int, int);
        void getPixel(int, int);
        void setPen(short int, short int, short int);
        void log(const char *pWhatToLog);
        long getStartBuffer(int, int);
        void drawText(int, int, const char *);
        int draw1(int, int);
        int draw2(int, int);
        int draw3(int, int);
        int draw4(int, int);
        int draw5(int, int);
        int draw6(int, int);
        int draw7(int, int);
        int draw8(int, int);
        int draw9(int, int);
        int drawx(int, int);
        bool comparePart(PictureClass *pCompareWidth, int pX, int pY, int lWidth, int lHeight);
        bool compare(PictureClass *pCompareWidth);
        void simplifyColours();
        void deleteColourItems();

        bool gLogging = false;

        int BYTES_PER_PIXEL = 3;
        J_COLOR_SPACE color_space = JCS_RGB;

        ColourClass gPenColour;
        ColourClass gCurrentColour;

    private:
};