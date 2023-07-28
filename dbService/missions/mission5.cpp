#include "./missions.h"


void zipDataBase(System& airports)
{
    fs::path projectPath = fs::current_path().parent_path()/"DB.zip";
    fs::path DB_path = fs::current_path().parent_path()/"DB";

    string s_DB_path = DB_path; 
    string s_projectPath = projectPath;

    airports.zipDirectory(s_DB_path, s_projectPath);
}