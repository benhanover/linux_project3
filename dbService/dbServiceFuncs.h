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

void runDbService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs, bool thereIsZipFile);
void readInputFromFlightsService(int DataFileDescriptorFsToDb,int& choice, vector<string>& codeNames);

//int readChoiceFromFlightsService(int FileDescriptorFsToDb);
//void readUserInputFromFlightsService(int FileDescriptorFsToDb,vector<string>& codeNames);
void writeOutputToFlightsService(int FileDescriptorDbToFs, string outputStr);
string getDataForParent(int choice,System& airports, vector<string> codeNames);
void unzipDB(bool& thereIsZipFile);

void createNamedPipes(string& namedPipeFsToDbService, string& namedPipeDbToFsService);
void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFsToDbService, string namedPipeDbToFsService);

void sendToFSThatDbStarted(int DataFileDescriptorDbToFs);
