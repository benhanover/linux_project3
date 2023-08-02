#include "./missions.h"



string gracefulExit(System& airports)
{
    zipDataBase(airports);
    
    fs::path currentPath = fs::current_path();
    string s_currentPath = currentPath;
    
    system("rm -r DB");

    airports.deleteAll(); //free all allocated memory

    string dbFinishedStr = "dbService finished and shutdown successfully.\n";

    return dbFinishedStr;
}