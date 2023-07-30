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
#include <unistd.h>
#define BUFFER_SIZE 4096
#define SIZE_TO_READ 256
#define MAX_NAME_LEN 10
#define SHUT_DOWN_CHOICE 6

using namespace std;
namespace fs = std::filesystem;


void runDbService(int FileDescriptorFsToDb,int FileDescriptorDbToFs, bool zipContainedData);
int readChoiceFromFlightsService(int FileDescriptorFsToDb);
void readUserInputFromFlightsService(int FileDescriptorFsToDb,vector<string>& codeNames);
void writeOutputToFlightsService(int FileDescriptorDbToFs, string outputStr);
string getDataForParent(int choice,System& airports, vector<string> codeNames);
void unzipDB(bool& zipContainedData);
bool containsDataDirectories(zip_t* archive);

void createNamedPipes(string& namedPipeFlightsServiceToDbService, string& namedPipeDbServiceToFlightsService);
void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFlightsServiceToDbService, string namedPipeDbServiceToFlightsService);

