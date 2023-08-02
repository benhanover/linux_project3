#include "./missions.h"


string zipDataBase(System& airports)
{
    string s_DB_dir_path = "/app/DB"; 
    string s_zipFilePath = "/app/DB.zip";

    airports.zipDirectory(s_DB_dir_path, s_zipFilePath);

    string outputStr = "Successfully zipped the directory.\n";
    return outputStr;

}