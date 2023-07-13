#include "./missions.h"



void gracefulExit(System& airports)
{
    zipDataBase(airports);
    
    fs::path buildPath = fs::current_path();
    fs::path projectPath = fs::current_path().parent_path();
    string s_projectPath = projectPath;
    string s_buildPath = buildPath;

    if (chdir(s_projectPath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }

    system("rm -r DB");

    if (chdir(buildPath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }

    airports.deleteAll(); //free all allocated memory
}