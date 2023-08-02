#pragma once
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
#include <errno.h>
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

void runFlightsService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs);
//void writeChoiceToDbService(int DataFileDescriptorFsToDb, int choice);
//void writeUserInputToDb(int DataFileDescriptorFsToDb,vector<string>& codeNames);
void writeInputToDbService(int DataFileDescriptorFsToDb, int choice, vector<string>& codeNames);

void writeToDbThatFlightsServiceRestarted(int DataFileDescriptorFsToDb);
void readOutputFromDbAndPrint(int DataFileDescriptorDbToFs);
int getChoice();
void printMenuToShowOptionalChoices();
void getInputForChoice(int choice, vector<string>& codeNames);
void getInputFromUser(vector<string>& words, string message);

void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFsToDbService, string namedPipeDbToFsService);
