#include "./dbServiceFuncs.h"

void runDbService(int FileDescriptorFsToDb,int FileDescriptorDbToFs, bool thereIsZipFile)
{
    cout << "in runDbService" << endl;


    System airports;
    int choice;
    bool gotShutDownOpcode = false;
    vector<string> codeNamesVec;
    string outputStr;

    if (thereIsZipFile)
        airports.load_db();

    while (!gotShutDownOpcode) 
    {
        int choice = readChoiceFromFlightsService(FileDescriptorFsToDb);

        if (choice == SHUT_DOWN_CHOICE){
            
            gracefulExit(airports);
            gotShutDownOpcode = true;
        }
        else {

            if(choice >= 1 && choice <= 4)
                    readUserInputFromFlightsService(FileDescriptorFsToDb,codeNamesVec);
            
            bool dbLoaded = airports.isDataBaseLoaded();
            string isDBloaded;
            if (dbLoaded) 
                isDBloaded = "true";
            else
                isDBloaded = "false";

            cout << "dbLoaded is: " << isDBloaded << endl;
            if ((choice >= 2 && choice <= 5) && dbLoaded == false) //there is no data available to execute choice 2/3/4/5
            {
                outputStr = "Currently there is not any data in the program. Can not run choice number " + to_string(choice) + ".\n";
                outputStr += "In order to get data, please choose option 1 and provide the desired ICAO codes.\n";

                cout << "dbService returns string that there's no data available" << endl;

            }
            else
                outputStr = getDataForParent(choice, airports, codeNamesVec);
        
            writeOutputToFlightsService(FileDescriptorDbToFs, outputStr);
        }
    }
   

}


int readChoiceFromFlightsService(int FileDescriptorFsToDb)
{
    cout << "in readChoiceFromFlightsService" << endl;


    int choice;
    read(FileDescriptorFsToDb, &choice, sizeof(choice));
    cout << "got choice:  " << choice << endl;
    return choice;
}

void readUserInputFromFlightsService(int FileDescriptorFsToDb,vector<string>& codeNames)
{
    cout << "in readUserInputFromFlightsService" << endl;


    int vectorSize;
    ssize_t bytesRead = read(FileDescriptorFsToDb, &vectorSize, sizeof(vectorSize));
    codeNames.clear();
    codeNames.reserve(vectorSize);

    char buffer[MAX_NAME_LEN];
    for (int i = 0; i < vectorSize; ++i)
    {
        bytesRead = read(FileDescriptorFsToDb, buffer, sizeof(buffer));
        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            codeNames.emplace_back(buffer);
            cout << "Got this code" << buffer << endl;
        }
        memset(buffer, 0, sizeof(buffer));
    }   

}

void writeOutputToFlightsService(int FileDescriptorDbToFs, string outputStr)
{
    cout << "in writeOutputToFlightsService" << endl;
    cout << "writing to FS:" << outputStr << endl;

    int outputStrSize = strlen(outputStr.c_str()) + 1;
    write(FileDescriptorDbToFs, &outputStrSize, sizeof(outputStrSize));
    write(FileDescriptorDbToFs, outputStr.c_str(), outputStrSize);
}

string getDataForParent(int choice,System& airports, vector<string> codeNames)
{
    cout << "in getDataForParent" << endl;


    string result;
    bool fetchedAll;
    switch(choice)
    {
        case 1:
        {
           fetchedAll = fetchAirportsData(airports, codeNames);
           if (fetchedAll)
                result = "The system fetched all data.\n";
            else 
                result = "The system could not fetch all data. You can try again with different ICAO codes.\n";
        }
        break;
        case 2: result = printAirportsArv(airports, codeNames);
        break;
        case 3: result = printAirportSchedule(airports, codeNames);
        break;
        case 4: result = printAllAircraftsFlights(airports, codeNames);
        break;
        case 5: zipDataBase(airports);
        break;
    }
    return result;
}


void unzipDB(bool& thereIsZipFile)
{

    cout << "in unzipDB" << endl;


    string zipFilePath = fs::current_path().parent_path()/"DB.zip";

    if (!filesystem::exists(zipFilePath))
    {
        std::cerr << "DB.zip does not exist." << std::endl;
        thereIsZipFile = false;
        return;
    }


    zip_t *archive = zip_open(zipFilePath.c_str(), ZIP_RDONLY, nullptr);
    if (archive == nullptr)
    {
        cerr << "Failed to open the ZIP file: " << zip_strerror(archive) << endl;
        return;
    }
    
    //zipContainedData = containsDataDirectories(archive);


    string destinationPath = fs::current_path().parent_path()/"DB";
    filesystem::path absDestinationPath = filesystem::absolute(destinationPath);

    // Create the destination directory manually
    filesystem::create_directory(absDestinationPath);


    int numEntries = zip_get_num_entries(archive, ZIP_FL_UNCHANGED);
    for (int i = 0; i < numEntries; ++i)
    {
        zip_stat_t entryStats;
        if (zip_stat_index(archive, i, ZIP_FL_UNCHANGED, &entryStats) != 0)
        {
            cerr << "Failed to retrieve the ZIP entry information at index " << i << endl;
            continue;
        }

        string entryName = entryStats.name;
        filesystem::path entryPath = absDestinationPath / entryName;

        if (entryStats.name[strlen(entryStats.name) - 1] == '/')
        {
            // Entry is a directory
            filesystem::create_directories(entryPath);
        }
        else
        {
            // Entry is a file
            zip_file_t *file = zip_fopen_index(archive, i, ZIP_FL_UNCHANGED);
            if (file == nullptr)
            {
                cerr << "Failed to open the ZIP file entry: " << entryName << endl;
                continue;
            }

            ofstream outputFile(entryPath.string(), ios::binary);
            if (!outputFile)
            {
                cerr << "Failed to create the output file: " << entryPath << endl;
                continue;
            }

            char buffer[1024];
            zip_int64_t bytesRead;
            while ((bytesRead = zip_fread(file, buffer, sizeof(buffer))) > 0)
            {
                outputFile.write(buffer, bytesRead);
            }

            outputFile.close();
            zip_fclose(file);
        }
    }

    /*
    //making the scripts executable
    string flightScannerScriptPath = destinationPath + "/flightScanner.sh";
    string command = "chmod +x " + flightScannerScriptPath;
    system(command.c_str());
    
    string cleanScriptPath = destinationPath + "/clean.sh";
    command = "chmod +x " + cleanScriptPath;
    system(command.c_str());*/

    zip_close(archive);
    thereIsZipFile = true;
    cout << "Successfully unzipped the directory." << endl;
}

/*bool containsDataDirectories(zip_t* archive)
{
        cout << "in containsDataDirectories" << endl;

     bool containsFilesInDirectories = false; // Flag to track if we find directories with files

    int numEntries = zip_get_num_entries(archive, ZIP_FL_UNCHANGED);
    for (int i = 0; i < numEntries; ++i) {
        zip_stat_t entryStats;
        if (zip_stat_index(archive, i, ZIP_FL_UNCHANGED, &entryStats) != 0) {
            std::cerr << "Failed to retrieve the ZIP entry information at index " << i << std::endl;
            continue;
        }

        if (entryStats.name[strlen(entryStats.name) - 1] == '/') {
            // Entry is a directory
            // We don't need to set the flag here as the loop will continue and check the contents of the directory
        } else {
            // Entry is a file (not a directory)
            containsFilesInDirectories = true; // Set the flag as we found a file in a directory
        }
    }
    if (containsDataDirectories) 
        cout << "there IS data" << endl;
    else 
        cout << "there IS NOOOOO data" << endl;


    return containsFilesInDirectories;
}*/


void createNamedPipes(string& namedPipeFlightsServiceToDbService, string& namedPipeDbServiceToFlightsService)
{
            cout << "in createNamedPipes" << endl;

    
    string namedPipeDirPath = "/tmp/flights_pipes";
    // Create namedPipe directory manually
    fs::create_directory(namedPipeDirPath);
    chmod(namedPipeDirPath.c_str(), 0777);

    namedPipeFlightsServiceToDbService = "/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    namedPipeDbServiceToFlightsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    mkfifo(namedPipeFlightsServiceToDbService.c_str(), 0666);
    mkfifo(namedPipeDbServiceToFlightsService.c_str(), 0666);

}

void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFlightsServiceToDbService, string namedPipeDbServiceToFlightsService)
{
    
                cout << "in closeAndUnlinkNamedPipes" << endl;

    close(FileDescriptorFsToDb);
    close(FileDescriptorDbToFs);

    unlink(namedPipeFlightsServiceToDbService.c_str());
    unlink(namedPipeDbServiceToFlightsService.c_str());
    
}