#include "./missions.h"


void zipDataBase(System& airports)
{
    //fs::path DB_dir_path = "/tmp/flights_pipes/DB";
    //fs::path zipFilePath = fs::current_path()/"DB.zip";

    string s_DB_dir_path = "/tmp/flights_pipes/DB"; 
    string s_zipFilePath = "/tmp/flights_pipes/DB.zip";

    airports.zipDirectory(s_DB_dir_path, s_zipFilePath);
}