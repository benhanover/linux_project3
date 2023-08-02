#include "./missions.h"



string gracefulExit(System& airports)
{
    zipDataBase(airports);
    
    fs::path currentPath = fs::current_path();
    //fs::path dbServicePath = fs::current_path().parent_path();
    string s_currentPath = currentPath;
    //string s_dbServicePath = dbServicePath;

    /*if (chdir(s_dbServicePath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }*/

    system("rm -r DB");

    /*if (chdir(s_buildPath.c_str()) != 0) 
    {
        cout << "Failed to change directory.\n";
    }*/

    airports.deleteAll(); //free all allocated memory

    string dbFinishedStr = "dbService finished and shutdown successfully.\n";

    return dbFinishedStr;
}