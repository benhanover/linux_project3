#pragma once
#include "./missions/missions.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <fcntl.h>
#include <algorithm>
#include <unistd.h>
#include <zip.h>
#include <sys/stat.h>
#include <csignal>
#define BUFFER_SIZE 4096
#define SIZE_TO_READ 256
#define MAX_NAME_LEN 10
#define SHUT_DOWN_CHOICE 6
#define MAX_MSG_STARTED_OR_RUNNING 30

using namespace std;
namespace fs = std::filesystem;


const string dbServiceStartedStr = "dbServiceStarted";
const string dbServiceKeepsRunningStr = "dbServiceKeepsRunning";

const string flightsServiceStartedStr = "fsServiceStarted";
const string flightsServiceKeepsRunningStr = "fsServiceKeepsRunning";

void getFsStatusAndClearDataPipeIfRestarted(int StatusFileDescriptorFsToDb, string& fsStartedOrKeepsRunning, int DataFileDescriptorFsToDb /*, bool& firstRunDbService*/);
void setNonBlockingMode(int fileDescriptor);
void setBlockingMode(int fileDescriptor);
void sendDbStatusToFs(int StatusFileDescriptorDbToFs, string& dbStartedOrKeepsRunning, int DataFileDescriptorDbToFs);

void runDbService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs, 
                /* int StatusFileDescriptorFsToDb, int StatusFileDescriptorDbToFs, */ bool thereIsZipFile);

int readChoiceFromFlightsService(int FileDescriptorFsToDb/* ,  int StatusFileDescriptorFsToDb */);
void readUserInputFromFlightsService(int FileDescriptorFsToDb,vector<string>& codeNames/* , int StatusFileDescriptorFsToDb, string& fsKeepsRunningWhenWaitingForInput */);
void writeOutputToFlightsService(int FileDescriptorDbToFs, string outputStr);
string getDataForParent(int choice,System& airports, vector<string> codeNames);
void unzipDB(bool& thereIsZipFile);

void createNamedPipes(string& namedPipeFsToDbService, string& namedPipeDbToFsService);
void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFsToDbService, string namedPipeDbToFsService);

/* void createStatusPipes(string& statusPipeFsToDb, string& statusPipeDbToFs);
void closeAndUnlinkStatusPipes(int StatusFileDescriptorFsToDb,int StatusFileDescriptorDbToFs, 
        string statusPipeFsToDb, string statusPipeDbToFs); */