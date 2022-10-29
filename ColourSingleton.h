#include <fstream>
#include <iostream>
#ifndef COLOUR_H
    #include "Colour.h"
    #define COLOUR_H
#endif
using namespace std;

class ColourSingleton{
    public:


        struct SortedColour{
            Colour colour;
            double sortValue = 0;
            SortedColour *next = 0;
            SortedColour *previous = 0;
        };

        SortedColour *gAllSortedColours = 0;
        Colour *gAllColours = 0;

        Colour *gLastColourByRed = 0;
        Colour *gLastColourByGreen = 0;
        Colour *gLastColourByBlue = 0;

        Colour *gAllColoursByRed = 0;
        Colour *gAllColoursByGreen = 0;
        Colour *gAllColoursByBlue = 0;

        void newSortedColour(short pRed, short pGreen, short pBlue, double pValue);
        void extractTable();
        void insertColourHue(Colour *pColour);
        Colour *getHSVColour(double pHue, double pSaturation, double pValue);
        Colour *getColour(int pRed, int pGreen, int pBlue);
        ColourSingleton();
        ~ColourSingleton();
    private:
};