#include "./dbServiceFuncs.h"

void runDbService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs, 
                /* int StatusFileDescriptorFsToDb, int StatusFileDescriptorDbToFs, */ bool thereIsZipFile)
{
    cout << "in runDbService" << endl;

    System airports;
    int choice;
    bool gotShutDownOpcode = false;
    vector<string> codeNamesVec;
    string outputStr;
    
    string dbStartedOrKeepsRunning = dbServiceStartedStr; //the first time it is defined, the program started or restarted
    string fsStartedOrKeepsRunning;
    bool firstRunDbService =true;

    if (thereIsZipFile)
        airports.load_db();

    while (!gotShutDownOpcode) 
    {    
       /*  sendDbStatusToFs(StatusFileDescriptorDbToFs, dbStartedOrKeepsRunning, DataFileDescriptorDbToFs);
        getFsStatusAndClearDataPipeIfRestarted(StatusFileDescriptorFsToDb,fsStartedOrKeepsRunning, DataFileDescriptorFsToDb); //, firstRunDbService);
 */

        int choice = readChoiceFromFlightsService(DataFileDescriptorFsToDb/* , StatusFileDescriptorFsToDb */);

        if (choice == SHUT_DOWN_CHOICE)
        {
            
            gracefulExit(airports);
            gotShutDownOpcode = true;
        }
        else
        {
            string fsKeepsRunningWhenWaitingForInput;
            if(choice >= 1 && choice <= 4)
                    readUserInputFromFlightsService(DataFileDescriptorFsToDb,codeNamesVec/* , StatusFileDescriptorFsToDb, fsKeepsRunningWhenWaitingForInput*/);
            
            bool dbLoaded = airports.isDataBaseLoaded();
            if ((choice >= 2 && choice <= 5) && dbLoaded == false) //there is no data available to execute choice 2/3/4/5
            {
                outputStr = "Currently there is not any data in the program. Can not run choice number " + to_string(choice) + ".\n";
                outputStr += "In order to get data, please choose option 1 and provide the desired ICAO codes.\n";

                cout << "dbService returns string that there's no data available" << endl;

            }
            else
            {   
              // if (fsKeepsRunningWhenWaitingForInput == flightsServiceKeepsRunningStr)
                    outputStr = getDataForParent(choice, airports, codeNamesVec);
            }

            //if (fsKeepsRunningWhenWaitingForInput == flightsServiceKeepsRunningStr)
                    writeOutputToFlightsService(DataFileDescriptorDbToFs, outputStr);
        }
    }

}

/* 
void getFsStatusAndClearDataPipeIfRestarted(int StatusFileDescriptorFsToDb, string& fsStartedOrKeepsRunning,
    int DataFileDescriptorFsToDb, bool& firstRunDbService)
{

            cout << "in getFsStatusAndClearDataPipeIfRestarted" << endl;

    char buffer[MAX_MSG_STARTED_OR_RUNNING];
    ssize_t bytesRead = read(StatusFileDescriptorFsToDb, buffer, sizeof(buffer));
    buffer[bytesRead] = '\0';
    
    cout <<"status of FS is: " << buffer << endl; 
    if (buffer == flightsServiceStartedStr)
    {
        fsStartedOrKeepsRunning = flightsServiceStartedStr;
        setNonBlockingMode(DataFileDescriptorFsToDb);
        
        //clearing named Pipe Fs To Db, to avoid old data if present in the data pipe
        char buffer[SIZE_TO_READ];
        int resSize;
        ssize_t byteRead;

        do
        {
                        cout << "in clearDataLoop" << endl;

           byteRead = read(DataFileDescriptorFsToDb, buffer, SIZE_TO_READ);
        }
        while (byteRead > 0 || !(byteRead == -1 && errno == EAGAIN));

        setBlockingMode(DataFileDescriptorFsToDb);
    }
    else 
    {
        fsStartedOrKeepsRunning = flightsServiceKeepsRunningStr;
    }

}

void setNonBlockingMode(int fileDescriptor)
{
    int flags = fcntl(fileDescriptor, F_GETFL, 0);
    fcntl(fileDescriptor, F_SETFL, flags | O_NONBLOCK);
}

void setBlockingMode(int fileDescriptor)
{
    int flags = fcntl(fileDescriptor, F_GETFL, 0);
    flags &= ~O_NONBLOCK; // Clear the O_NONBLOCK flag
    fcntl(fileDescriptor, F_SETFL, flags);
}

void sendDbStatusToFs(int StatusFileDescriptorDbToFs, string& dbStartedOrKeepsRunning, int DataFileDescriptorDbToFs)
{ 

            cout << "in sendDbStatusToFs" << endl;

    int statusStrSize;

    if (dbStartedOrKeepsRunning == dbServiceStartedStr)
    {
        statusStrSize = strlen(dbServiceStartedStr.c_str()) + 1;
        write(StatusFileDescriptorDbToFs, dbServiceStartedStr.c_str(), statusStrSize);
        
        string outputStr = "Notice: dbService Program Just started. Ignoring old Data sent from dbService.\n\n";
        writeOutputToFlightsService(DataFileDescriptorDbToFs, outputStr);

        dbStartedOrKeepsRunning = dbServiceKeepsRunningStr; //update for next, from now it will send it keeps running
    }
    else  //dbService did not just started, it keeps running, send info to FlightsService
    {
        statusStrSize = strlen(dbServiceKeepsRunningStr.c_str()) + 1;
        write(StatusFileDescriptorDbToFs, dbServiceKeepsRunningStr.c_str(), statusStrSize);
    }

}
 */

int readChoiceFromFlightsService(int DataFileDescriptorFsToDb/* , int StatusFileDescriptorFsToDb */)
{
    cout << "in readChoiceFromFlightsService" << endl;

    /*setNonBlockingMode(StatusFileDescriptorFsToDb);
    string fsStartedOrKeepsRunning;
    getFsStatusAndClearDataPipeIfRestarted(StatusFileDescriptorFsToDb,fsStartedOrKeepsRunning, DataFileDescriptorFsToDb);
    setBlockingMode(StatusFileDescriptorFsToDb);

    if (fsStartedOrKeepsRunning == flightsServiceKeepsRunningStr)
    {*/
        int choice;
        read(DataFileDescriptorFsToDb, &choice, sizeof(choice));
        cout << "got choice:  " << choice << endl;
        return choice;
   // }
    //else return 0;
}

void readUserInputFromFlightsService(int DataFileDescriptorFsToDb,vector<string>& codeNames/* , int StatusFileDescriptorFsToDb,  string& fsKeepsRunningWhenWaitingForInput */)
{
    cout << "in readUserInputFromFlightsService" << endl;
    
    /*setNonBlockingMode(StatusFileDescriptorFsToDb);
    getFsStatusAndClearDataPipeIfRestarted(StatusFileDescriptorFsToDb,fsKeepsRunningWhenWaitingForInput, DataFileDescriptorFsToDb);
    setBlockingMode(StatusFileDescriptorFsToDb);


    if (fsKeepsRunningWhenWaitingForInput == flightsServiceKeepsRunningStr)
    {*/
        int vectorSize;
        ssize_t bytesRead = read(DataFileDescriptorFsToDb, &vectorSize, sizeof(vectorSize));
        codeNames.clear();
        codeNames.reserve(vectorSize);

        char buffer[MAX_NAME_LEN];
        for (int i = 0; i < vectorSize; ++i)
        {
            bytesRead = read(DataFileDescriptorFsToDb, buffer, sizeof(buffer));
            if (bytesRead > 0)
            {
                buffer[bytesRead] = '\0';
                codeNames.emplace_back(buffer);
                cout << "Got this code" << buffer << endl;
            }
            memset(buffer, 0, sizeof(buffer));
        }   
   // }
}

void writeOutputToFlightsService(int DataFileDescriptorDbToFs, string outputStr)
{
    cout << "in writeOutputToFlightsService" << endl;
    cout << "writing to FS:" << outputStr << endl;

    int outputStrSize = strlen(outputStr.c_str()) + 1;
    write(DataFileDescriptorDbToFs, &outputStrSize, sizeof(outputStrSize));
    write(DataFileDescriptorDbToFs, outputStr.c_str(), outputStrSize);
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
        case 5:
        {
            zipDataBase(airports);
            result = "Successfully zipped the directory.\n";
        }
        break;
    }
    return result;
}


void unzipDB(bool& thereIsZipFile)
{

    cout << "in unzipDB" << endl;


    string zipFilePath = "/tmp/flights_pipes/DB.zip";

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


    string destinationPath = "/app/DB";
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

    zip_close(archive);
    thereIsZipFile = true;
    cout << "Successfully unzipped the directory." << endl;
}


void createNamedPipes(string& namedPipeFsToDbService, string& namedPipeDbToFsService)
{
            cout << "in createNamedPipes" << endl;

    
    string namedPipeDirPath = "/tmp/flights_pipes";
    // Create namedPipe directory manually
    fs::create_directory(namedPipeDirPath);
    chmod(namedPipeDirPath.c_str(), 0777);

    namedPipeFsToDbService = "/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    namedPipeDbToFsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    mkfifo(namedPipeFsToDbService.c_str(), 0666);
    mkfifo(namedPipeDbToFsService.c_str(), 0666);

}

void closeAndUnlinkNamedPipes(int DataFileDescriptorFsToDb, int DataFileDescriptorDbToFs, 
        string namedPipeFsToDbService, string namedPipeDbToFsService)
{
    
                cout << "in closeAndUnlinkNamedPipes" << endl;

    close(DataFileDescriptorFsToDb);
    close(DataFileDescriptorDbToFs);

    unlink(namedPipeFsToDbService.c_str());
    unlink(namedPipeDbToFsService.c_str());
    
}
/* 
void createStatusPipes(string& statusPipeFsToDb, string& statusPipeDbToFs)
{
  cout << "in createStatusPipes" << endl;

    
    string namedPipeDirPath = "/tmp/flights_pipes";
    // Create namedPipe directory manually
    fs::create_directory(namedPipeDirPath);
    chmod(namedPipeDirPath.c_str(), 0777);

    statusPipeFsToDb = "/tmp/flights_pipes/statusPipeFlightsServiceToDbService";
    statusPipeDbToFs = "/tmp/flights_pipes/statusPipeDbServiceToFlightsService";

    mkfifo(statusPipeFsToDb.c_str(), 0666);
    mkfifo(statusPipeDbToFs.c_str(), 0666);


}

void closeAndUnlinkStatusPipes(int StatusFileDescriptorFsToDb,int StatusFileDescriptorDbToFs, 
        string statusPipeFsToDb, string statusPipeDbToFs)
{

                cout << "in closeAndUnlinkStatusPipes" << endl;

    close(StatusFileDescriptorFsToDb);
    close(StatusFileDescriptorDbToFs);

    unlink(statusPipeFsToDb.c_str());
    unlink(statusPipeDbToFs.c_str());
    
} */