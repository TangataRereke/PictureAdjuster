#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <jpeglib.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include "PictureClass.h"
#include <sstream>
#include "ColourSingleton.h"
#include <thread>
#include <future>
using namespace std;
int width = 0;
int height = 0;
int size = 0;
int bytes_per_pixel = 3;
J_COLOR_SPACE color_space = JCS_RGB;
//#define THREAD_COUNT 1
#define THREAD_COUNT 16
//#define Input_Path "/home/james/Documents/PhotosP/Processed/"
#define Input_Path "/home/james/Pictures/ToJPG/"
#define Input_SplitPath "/home/james/Documents/Code/SplitSample/Input/"
//#define Output_Path "/home/james/Documents/PhotosP/Processed/New/"
//#define Output_Path "/home/james/Pictures/PictureFrame/"
#define Output_Path "/home/james/Pictures/ToJPGFixed/"
//#define Output_Path "/home/james/Pictures/1080p/"
//#define Output_Path "/home/james/Pictures/4k/"
#define Output_SplitPath "/home/james/Documents/Code/SplitSample/Output/"
#define Input_GridPath "/home/james/GridPictures/In/"
#define Output_GridPath "/home/james/GridPictures/Out/"
BYTE *data = 0;
class PictureClass;
struct DuplicateItem
{
    struct stat statbuf;
    PictureClass *picture;
    string name;
    DuplicateItem *next;
};

DuplicateItem *allDups = 0;
DuplicateItem *lastDup = 0;
void coutTimeAsString()
{
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    char buffer [80];

    strftime (buffer,80,"%I:%M%p.",timeinfo);
    cout << "\t" << buffer;
}
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void createSampleImage(int pWidth, int pHeight)
{
    PictureClass *obj = new PictureClass;
    obj->setPen(1, 1, 1);
    obj->newImage(pWidth, pHeight);

    PictureClass::RECT lLeftRect;
    PictureClass::RECT lRightRect;
    lLeftRect.left = 0;
    lLeftRect.right = 99;
    lLeftRect.top = 0;
    lLeftRect.bottom = 99;
    lRightRect.left = pWidth - 101;
    lRightRect.right = pWidth -1;
    lRightRect.top = 0;
    lRightRect.bottom = 99;
    int lTop = 0;
    int lTotal = floor(pHeight / 100);
    int lMod = 6;
    for(int bCount = 0; bCount<lTotal; bCount++)
    {

        if((bCount%lMod)==0)
        {
            obj->setPen(255,0,0);
        }
        else if((bCount%lMod)==1)
        {
            obj->setPen(0,255,0);
        }
        else if((bCount%lMod)==2)
        {
            obj->setPen(0,0,255);
        }
        else if((bCount%lMod)==3)
        {
            obj->setPen(255,255,0);
        }
        else if((bCount%lMod)==4)
        {
            obj->setPen(255,0,255);
        }
        else if((bCount%lMod)==5)
        {
            obj->setPen(0,255,25);
        }
        obj->drawRectangle(lLeftRect);
        obj->drawRectangle(lRightRect);
        lTop = lTop + 100;
        lLeftRect.top = lTop;
        lLeftRect.bottom = lTop + 100;
        lRightRect.top = lLeftRect.top;
        lRightRect.bottom = lLeftRect.bottom;
    }

    lLeftRect.top = 0;
    lLeftRect.bottom = 99;
    lRightRect.top = pHeight - 101;
    lRightRect.bottom = pHeight - 1;
    lTop = 100;
    lTotal = floor(pWidth / 100) - 2;
    for(int bCount = 0; bCount<lTotal; bCount++)
    {

        if((bCount%lMod)==0)
        {
            obj->setPen(255,0,0);
        }
        else if((bCount%lMod)==1)
        {
            obj->setPen(0,255,0);
        }
        else if((bCount%lMod)==2)
        {
            obj->setPen(0,0,255);
        }
        else if((bCount%lMod)==3)
        {
            obj->setPen(255,255,0);
        }
        else if((bCount%lMod)==4)
        {
            obj->setPen(255,0,255);
        }
        else if((bCount%lMod)==5)
        {
            obj->setPen(0,255,255);
        }
        obj->drawRectangle(lLeftRect);
        obj->drawRectangle(lRightRect);
        lTop = lTop + 100;
        lLeftRect.left = lTop;
        lLeftRect.right = lTop + 100;
        lRightRect.left = lLeftRect.left;
        lRightRect.right = lLeftRect.right;
    }

    stringstream lST;
    lST << pWidth << "x" << pHeight;
    // obj.setPen(PictureClass::Pen_Invert);
    obj->setPen(255,255,255);
    obj->drawText(pWidth / 3, pHeight / 3, lST.str().c_str());


    stringstream lSS;
    lSS << "/home/james/Documents/PhotosP/Processed/sample_" << pWidth << "by"<< pHeight << ".jpg";
    cout << "Saved " << lSS.str() << endl;
    obj->saveJpeg(lSS.str().c_str());
    delete obj;
}
void createSampleImages()
{
    createSampleImage(480,384);
    createSampleImage(800,600);
    createSampleImage(1024,600);
    createSampleImage(1024,768);
    createSampleImage(1152,864);
    createSampleImage(1280,720);
    createSampleImage(1280,768);
    createSampleImage(1280,800);
    createSampleImage(1280,1024);
    createSampleImage(1360,768);
    createSampleImage(1366,768);
    createSampleImage(1440,900);
    createSampleImage(1536,864);
    createSampleImage(1600,900);
    createSampleImage(1680,1050);
    createSampleImage(1920,1080);
}

void autoResizePicture(const char *pPath, const char *pFilename)
{
    PictureClass *obj = new PictureClass;
    stringstream lIF;
    lIF << pPath << pFilename;
    //return;
    coutTimeAsString();
    cout << "   Loaded " << lIF.str() << endl;
    obj->loadJpeg(lIF.str().c_str());

    /* Don't change comment out >> remove */
    //obj->removeInterference();
    obj->autoLighten();
    //obj->removeTint();
 //   obj->smartResize(775, 440);
 //  obj->smartResize(1920, 1080);
    //obj->smartResize(3840, 2160);
    //obj->removeInterference();
    obj->removeDuplicatePixels();


    // End of main
    stringstream lS;
    lS << Output_Path << pFilename;
    //lS << Input_Path << "New/" << pFilename;
    obj->saveJpeg(lS.str().c_str());
    delete obj;
    coutTimeAsString();
    cout << "   Saved " << lIF.str() << endl;
}
void autoSplitPicture(const char *pPath, const char *pFilename)
{
    PictureClass *obj = new PictureClass;
    stringstream lIF;
    lIF << pPath << pFilename;
    //return;
    coutTimeAsString();
    cout << "   Loaded " << lIF.str() << endl;
    obj->loadJpeg(lIF.str().c_str());

    /* Don't change comment out >> remove */
    obj->skinDetect();


    // End of main
    stringstream lS;
    lS << Output_SplitPath << pFilename;
    //lS << Input_Path << "New/" << pFilename;
    obj->saveJpeg(lS.str().c_str());
    delete obj;
    coutTimeAsString();
    cout << "   Saved " << lIF.str() << endl;
}

void autoResizePictures(const char *pPath)
{
    string dir = string(pPath);
    vector<string> files = vector<string>();
    getdir(dir,files);
    time_t lTimer;
    time(&lTimer);

    int threadCount = 0;
    std::vector<std::thread> lThreads;
    unsigned int total = files.size();
    for (unsigned int i = 0; i < total; i++)
    {
        int length = files[i].length();
        if(length<3)
        {
            continue;
        }
        if(files[i].find(".gz")!=string::npos){
            continue;
        }
        if(files[i].find(".jpg")==string::npos)
        {
            stringstream lNewPath;
            lNewPath << pPath << files[i] << "/";
            cout << "Thread" << endl;
            for (auto& lThread : lThreads)
                lThread.join();
            threadCount=0;
            lThreads.clear();

            cout << "FINISHED Thread" << endl;

            autoResizePictures(lNewPath.str().c_str());
            continue;
        }
        lThreads.push_back(thread(autoResizePicture,pPath,files[i].c_str()));
        threadCount++;
        cout << "Thread" << endl;
        if(threadCount>=THREAD_COUNT)
        {
            for (auto& lThread : lThreads)
                lThread.join();
            threadCount=0;
            lThreads.clear();
            cout << "FINISHED Thread" << endl;
            time_t bNewTime;
            time(&bNewTime);
            long double bTimeTaken = difftime(bNewTime, lTimer);
            unsigned long newSeconds = (long double)((double)bTimeTaken / (double)(i+1)) * (double)((double)total - (double)i);
            unsigned long hours = newSeconds / 3600;
            newSeconds = newSeconds - (hours * 3600);
            unsigned long minutes = newSeconds / 60;
            newSeconds = newSeconds - (minutes * 60);
            cout << i << " out of " << total << " %" <<  (double)((i+1)*100)/total << " time left: " << hours << ":" << minutes << ":" << newSeconds << " time since: " << bTimeTaken <<  endl;
        }

        //cout << bFile->position << "\t" << bFile->name << endl;
    }
    cout << "Thread" << endl;
    for (auto& lThread : lThreads)
        lThread.join();
    cout << "FINISHED Thread" << endl;
    time_t bNewTime;
    time(&bNewTime);
    long double bTimeTaken = difftime(bNewTime, lTimer);
    unsigned long newSeconds = bTimeTaken;
    unsigned long hours = newSeconds / 3600;
    newSeconds = newSeconds - (hours * 3600);
    unsigned long minutes = newSeconds / 60;
    newSeconds = newSeconds - (minutes * 60);
    cout << "Finished it took " << hours << ":" << minutes << ":" << newSeconds << endl;
}

void autoSplitPictures(const char *pPath)
{
    string dir = string(pPath);
    vector<string> files = vector<string>();
    getdir(dir,files);
    time_t lTimer;
    time(&lTimer);

    int threadCount = 0;
    std::vector<std::thread> lThreads;
    unsigned int total = files.size();
    for (unsigned int i = 0; i < total; i++)
    {
        int length = files[i].length();
        if(length<3)
        {
            continue;
        }
        if(files[i].find(".jpg")==string::npos)
        {
            stringstream lNewPath;
            lNewPath << pPath << files[i] << "/";
            cout << "Thread" << endl;
            for (auto& lThread : lThreads)
                lThread.join();
            threadCount=0;
            lThreads.clear();

            cout << "FINISHED Thread" << endl;

            autoSplitPictures(lNewPath.str().c_str());
            continue;
        }
        lThreads.push_back(thread(autoSplitPicture,pPath,files[i].c_str()));
        threadCount++;
        cout << "Thread" << endl;
        if(threadCount>=THREAD_COUNT)
        {
            for (auto& lThread : lThreads)
                lThread.join();
            threadCount=0;
            lThreads.clear();
            cout << "FINISHED Thread" << endl;
            time_t bNewTime;
            time(&bNewTime);
            long double bTimeTaken = difftime(bNewTime, lTimer);
            unsigned long newSeconds = (long double)((double)bTimeTaken / (double)(i+1)) * (double)((double)total - (double)i);
            unsigned long hours = newSeconds / 3600;
            newSeconds = newSeconds - (hours * 3600);
            unsigned long minutes = newSeconds / 60;
            newSeconds = newSeconds - (minutes * 60);
            cout << i << " out of " << total << " %" <<  (double)((i+1)*100)/total << " time left: " << hours << ":" << minutes << ":" << newSeconds << " time since: " << bTimeTaken <<  endl;
        }

        //cout << bFile->position << "\t" << bFile->name << endl;
    }
    cout << "Thread" << endl;
    for (auto& lThread : lThreads)
        lThread.join();
    cout << "FINISHED Thread" << endl;
    time_t bNewTime;
    time(&bNewTime);
    long double bTimeTaken = difftime(bNewTime, lTimer);
    unsigned long newSeconds = bTimeTaken;
    unsigned long hours = newSeconds / 3600;
    newSeconds = newSeconds - (hours * 3600);
    unsigned long minutes = newSeconds / 60;
    newSeconds = newSeconds - (minutes * 60);
    cout << "Finished it took " << hours << ":" << minutes << ":" << newSeconds << endl;
}

void gridPictures(){
}


int main()
{
    cout << Output_Path << endl;
//    autoSplitPictures(Input_SplitPath);
    autoResizePictures(Input_Path);
//    gridPictures();
    return 1;
}
