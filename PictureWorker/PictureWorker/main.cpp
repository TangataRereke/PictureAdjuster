#include "main.h"
bool connect(){
    struct sockaddr_in lServerAddress;
    if((gSocket = socket(AF_INET, SOCK_STREAM, 0))<0){
        perror("Socket failed for port");
        return false;
    }

    int lSocketOption = 1;
    if (setsockopt(gSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &lSocketOption, sizeof(lSocketOption)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set non blocking
    int lFlags = fcntl(gSocket, F_GETFL, 0);
    if(lFlags==-1){
        close(gSocket);
        perror("Non block");
        exit(EXIT_FAILURE);
    }



    memset(&lServerAddress, 0, sizeof(lServerAddress));
    lServerAddress.sin_family = AF_INET;
    lServerAddress.sin_port = htons(gPort);
    if(inet_pton(AF_INET, "127.0.0.1", &lServerAddress.sin_addr)<=0){
        perror("conversion failed");
        return false;
    }
    if(connect(gSocket, (struct sockaddr *)&lServerAddress, sizeof(lServerAddress)) < 0){
        perror("Connect failed");
        return false;
    }
    return true;
}

void readData(){
    char buffer[65536];
    memset(buffer, 0, 65536);
    size_t bSize = recv(gSocket, buffer, 65536, MSG_DONTWAIT);
    if(bSize==0){
        gFinished = true;
        return;
    }else if(bSize==string::npos){
        return;
    }
    gBuffer.append(buffer);
}

void sendCommand(int pCommand){
    stringstream lSS;
    lSS << "\"Xn" << pCommand << "x\"" << "\r\n";
    if(send(gSocket, lSS.str().c_str(), lSS.str().length(), MSG_DONTWAIT)!=(unsigned int)lSS.str().length()){
        perror("Error sending command");
    }
}

void processNextCommand(){
    size_t lPos = gBuffer.find(Command_Finished);
    if(lPos==string::npos){
        return;
    }
    if(gBuffer==Command_Finished){
        gBuffer = "";
        sendCommand(OUTCOMMAND_FINISHED);
        return;
    }
    string lStartString = gBuffer.substr(0, 6);
    if(lStartString==Command_Start){
        gBuffer = gBuffer.substr(6);
        lStartString = gBuffer.substr(0, 6);
    }
    bool lErrorCheck = false;
    if(lStartString==Command_StartError){
        lErrorCheck = true;
    }
    gBuffer = gBuffer.substr(6);
    lPos = gBuffer.find(Command_EndItem);
    string lCommand = gBuffer.substr(0, lPos);
    gBuffer = gBuffer.substr(lPos + 6);
    if(!system(lCommand.c_str())){
        if(lErrorCheck){
            gBuffer = "";
            sendCommand(OUTCOMMAND_FINISHED);
            return;
        }
    }
//    gFinished = true;
}

void poll(){
    readData();
    processNextCommand();
    //gFinished = true;
}

int main(int argc, char* argv[])
{
    if(argc>=2){
        gPort = atoi(argv[1]);
    }
    if(!connect()){
        return -1;
    }
    while(!gFinished){
        poll();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}
