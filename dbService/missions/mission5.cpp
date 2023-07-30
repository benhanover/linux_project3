#include "./missions.h"


void zipDataBase(System& airports)
{
    fs::path DB_dir_path = fs::current_path().parent_path()/"DB";
    fs::path zipFilePath = fs::current_path().parent_path()/"DB.zip";

    string s_DB_dir_path = DB_dir_path; 
    string s_zipFilePath = zipFilePath;

    airports.zipDirectory(DB_dir_path, s_zipFilePath);
}