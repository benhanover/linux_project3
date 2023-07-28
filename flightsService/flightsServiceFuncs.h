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
#include <unistd.h>
#define BUFFER_SIZE 4096
#define SIZE_TO_READ 256
#define MAX_NAME_LEN 10
#define SHUT_DOWN_CHOICE 6


using namespace std;
namespace fs = std::filesystem;


void runFlightsService(int FileDescriptorFsToDb,int FileDescriptorDbToFs);

void writeChoiceToDbService(int FileDescriptorFsToDb, int choice);
void writeUserInputToDb(int FileDescriptorFsToDb,vector<string>& codeNames);
void readOutputFromDbAndPrint(int FileDescriptorDbToFs);

int getChoice();
void printMenuToShowOptionalChoices();
void getInputForChoice(int choice, vector<string>& codeNames);
void getInputFromUser(vector<string>& words, string message);

void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFlightsServiceToDbService, string namedPipeDbServiceToFlightsService);



