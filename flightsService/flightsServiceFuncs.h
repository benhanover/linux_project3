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
#define NUM_SIGNAL_FS_STARTED -1

using namespace std;
namespace fs = std::filesystem;


const string dbServiceStartedStr = "dbServiceStarted";

void runFlightsService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs);
void sendFsStartedSignalToDb(int DataFileDescriptorFsToDb);
void getDbStatus(int DataFileDescriptorDbToFs);
void writeInputToDbService(int DataFileDescriptorFsToDb, int choice, string& codeNamesStr);
void readOutputFromDbAndPrint(int DataFileDescriptorDbToFs);
int getChoice();
void printMenuToShowOptionalChoices();
void getInputForChoice(int choice, string& codeNamesStr);
void getInputFromUser(string& input, string message);
void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFsToDbService, string namedPipeDbToFsService);




