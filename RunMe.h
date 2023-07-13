#pragma once
#include "./missions/missions.h"
#include <csignal>
#include <unistd.h>
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 4096
#define SIZE_TO_READ 256
#define MAX_NAME_LEN 10

void runChildProcess(int* parentToChild,int* childToParent, System& airports);
void runParentProcess(int* parentToChild,int* childToParent, pid_t pid, pid_t childPID);
void handleSIGINT(int signalNumber);
void handleSIGUSR1(int signalNumber);
void printMenu();
void execute(System& airports);
void executeChoice(int choice,System& airports);
void unzipDB(const string& zipFilePath, const string& destinationPath);
void getInputForChoice(int choice, vector<string>& codeNames);
string getDataAndSendToParent(int choice,System& airports, vector<string> codeNames);
int getChoice();













