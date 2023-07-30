#include "./missions.h"



void gracefulExit(System& airports)
{
    zipDataBase(airports);
    
    fs::path buildPath = fs::current_path();
    fs::path dbServicePath = fs::current_path().parent_path();
    string s_buildPath = buildPath;
    string s_dbServicePath = dbServicePath;

    if (chdir(s_dbServicePath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }

    system("rm -r DB");

    if (chdir(s_buildPath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }

    airports.deleteAll(); //free all allocated memory
}