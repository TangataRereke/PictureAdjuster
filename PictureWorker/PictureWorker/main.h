#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>
using namespace std;
bool gFinished = false;
int gPort = 65004;
int gSocket = 0;
string gBuffer = "";
        enum OutCommands{
            OUTCOMMAND_BEGIN = 1,
            OUTCOMMAND_END,
            OUTCOMMAND_NEWITEM,
            OUTCOMMAND_NEWITEM_ERROROUT,
            OUTCOMMAND_ENDITEM,
            OUTCOMMAND_FINISHED // 6
        };

string static Command_Finished = "\"Xn2x\"";
string static Command_Start = "\"Xn1x\"";
string static Command_StartError = "\"Xn4x\"";
string static Command_StartNoError = "\"Xn3x\"";
string static Command_EndItem = "\"Xn5x\"";
#endif // MAIN_H_INCLUDED
