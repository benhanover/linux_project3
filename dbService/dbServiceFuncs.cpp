#include "./dbServiceFuncs.h"

void runDbService(int FileDescriptorFsToDb,int FileDescriptorDbToFs)
{
    System airports;
    int choice;
    vector<string> codeNamesVec;
    string outputStr;

    bool dataUnzipped = checkIfDataUnzipped();
    if (dataUnzipped)
        airports.load_db();


    while (choice != SHUT_DOWN_CHOICE) 
    {
        int choice = readChoiceFromFlightsService(FileDescriptorFsToDb);
        readUserInputFromFlightsService(FileDescriptorFsToDb, codeNamesVec); 

        bool dbLoaded = airports.isDataBaseLoaded();
        if ((choice >= 2 && choice <= 5) && dbLoaded == false) //there is no data available to execute choice 2/3/4
        {
            outputStr = "Currently there is not any data in the program.\nCan not run choice no. " + choice + '\n';
            outputStr += "In order to get data, please choose option 1 and provide the desired ICAO codes.\n";
        }
        else
        {
            if(choice >= 1 && choice <= 4)
                readUserInputFromFlightsService(FileDescriptorFsToDb,codeNamesVec);

            outputStr = getDataForParent(choice, airports, codeNamesVec);
        } 

        writeOutputToFlightsService(FileDescriptorDbToFs, outputStr);
    }
   
}


int readChoiceFromFlightsService(int FileDescriptorFsToDb)
{
    int choice;
    read(FileDescriptorFsToDb, &choice, sizeof(choice));
    return choice;
}

void readUserInputFromFlightsService(int FileDescriptorFsToDb,vector<string>& codeNames)
{
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
        }
        memset(buffer, 0, sizeof(buffer));
    }   

}

void writeOutputToFlightsService(int FileDescriptorDbToFs, string outputStr)
{
    int outputStrSize = outputStr.size() + 1;
    write(FileDescriptorDbToFs, &outputStrSize, sizeof(outputStrSize));
    write(FileDescriptorDbToFs, outputStr.c_str(), outputStrSize);
}

string getDataForParent(int choice,System& airports, vector<string> codeNames)
{
    string result;
    bool fetchedAll;
    switch(choice)
    {
        case 1:
        {
           fetchedAll = fetchAirportsData(airports, codeNames);
           if (fetchedAll)
                result = "The system fetched all data.";
            else 
                result = "The system could not fetch all data.";
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


///// need to fix /////***********************
void unzipDB(/*const string& zipFilePath, const string& destinationPath*/)
{
    string zipFilePath = "/tmp/DB.zip";
    string destinationPath = fs::current_path().parent_path()/"DB";

    zip_t *archive = zip_open(zipFilePath.c_str(), ZIP_RDONLY, nullptr);
    if (archive == nullptr)
    {
        cerr << "Failed to open the ZIP file: " << zip_strerror(archive) << endl;
        return;
    }

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

    //making the scripts executable
    string flightScannerScriptPath = destinationPath + "/flightScanner.sh";
    string command = "chmod +x " + flightScannerScriptPath;
    system(command.c_str());
    
    string cleanScriptPath = destinationPath + "/clean.sh";
    command = "chmod +x " + cleanScriptPath;
    system(command.c_str());

    zip_close(archive);
    cout << "Successfully unzipped the directory." << endl;
}

void createNamedPipes(string& namedPipeFlightsServiceToDbService, string& namedPipeDbServiceToFlightsService)
{
    string namedPipeDirPath = "/tmp/flights_pipes";
    // Create namedPipe directory manually
    fs::create_directory(namedPipeDirPath);
    chmod(namedPipeDirPath.c_str(), 0777);

    namedPipeFlightsServiceToDbService = "/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    namedPipeDbServiceToFlightsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    mkfifo(namedPipeFlightsServiceToDbService.c_str(), 0666);
    mkfifo(namedPipeFlightsServiceToDbService.c_str(), 0666);

}

void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFlightsServiceToDbService, string namedPipeDbServiceToFlightsService)
{
    close(FileDescriptorFsToDb);
    close(FileDescriptorDbToFs);

    unlink(namedPipeFlightsServiceToDbService.c_str());
    unlink(namedPipeDbServiceToFlightsService.c_str());
    
}