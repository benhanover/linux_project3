#include "./RunMe.h"

System airports;
pid_t pid; 
bool dbLoaded = false;

int main()
{
    int choice;
    string projectPath = fs::current_path().parent_path()/"DB";
    string DBZipPath = fs::current_path().parent_path()/"DB.zip";

    unzipDB(DBZipPath, projectPath);

    signal(SIGINT, handleSIGINT);

    execute(airports);

    return 1;
}

//---------------------------------functions------------------------------------
void execute(System& airports)
{
    int choice;
    int parentToChild[2];  // Pipe for parent to child communication
    int childToParent[2];  // Pipe for child to parent communication

    if (pipe(parentToChild) == -1 || pipe(childToParent) == -1) {
        cout << "Pipe creation failed." << endl;
        return;
    }

    pid = fork();
    pid_t childPID;
    if (pid == -1)
    {
        cout << "Error forking process" << endl;
        return;
    }

    else if (pid == 0) // Child process
    {
        signal(SIGUSR1, handleSIGUSR1);
        runChildProcess(parentToChild, childToParent, airports);
        childPID = getpid();
    }

    else // Parent process
    {
        runParentProcess(parentToChild, childToParent, pid, childPID);
    }
}
void runChildProcess(int* parentToChild,int* childToParent, System& airports)
{
    /* vector<string> paths;
    paths.reserve(10);
    airports.getAllPaths(paths);
    airports.load_db(paths); */

    close(parentToChild[WRITE_END]);  // Close unused write end of parent-to-child pipe
    close(childToParent[READ_END]);  // Close unused read end of child-to-parent pipe

    int choice, vectorSize;
    vector<string> codeNames;
    string res;
    while (true) 
    {
        ssize_t bytesRead = read(parentToChild[READ_END], &choice, sizeof(choice));
        if (bytesRead <= 0) // End of data
            break;

        if (airports.)


        if(choice >= 1 && choice <= 4)
        {
            bytesRead = read(parentToChild[READ_END], &vectorSize, sizeof(vectorSize));
            codeNames.clear();
            codeNames.reserve(vectorSize);

            char buffer[MAX_NAME_LEN];
            for (int i = 0; i < vectorSize; ++i)
            {
                bytesRead = read(parentToChild[READ_END], buffer, sizeof(buffer));
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    codeNames.emplace_back(buffer);
                }
                memset(buffer, 0, sizeof(buffer));
            }            
        }
        res = getDataAndSendToParent(choice,airports, codeNames);
        
        int resSize = res.size() + 1;
        write(childToParent[WRITE_END], &resSize, sizeof(resSize));
        write(childToParent[WRITE_END], res.c_str(), resSize);
    }
    close(parentToChild[READ_END]);
    close(childToParent[WRITE_END]);
}

void runParentProcess(int* parentToChild,int* childToParent, pid_t pid, pid_t childPID)
{
    close(parentToChild[READ_END]);  // Close unused read end of parent-to-child pipe
    close(childToParent[WRITE_END]);  // Close unused write end of child-to-parent pipe
    int choice, vectorSize;

    while (true) 
    {
        vector<string> codeNames;
        choice = getChoice();
        getInputForChoice(choice, codeNames);

        if (choice != 6)
            write(parentToChild[WRITE_END], &choice, sizeof(choice));
        usleep(10);

        if(choice >= 1 && choice <= 4)
        {
            vectorSize = codeNames.size();
            write(parentToChild[WRITE_END], &vectorSize, sizeof(vectorSize));
            for (const auto& name : codeNames)
            {
                write(parentToChild[WRITE_END], name.c_str(), name.size() + 1);  // Include null terminator
                usleep(10);
            }

            //Read the output of the child process
            char buffer[BUFFER_SIZE];
            int resSize;
            read(childToParent[READ_END], &resSize, sizeof(resSize));
            while(resSize > 0)
            {
                ssize_t byteRead = read(childToParent[READ_END], buffer, SIZE_TO_READ);
                buffer[byteRead] = '\0';
                cout << buffer; 
                resSize -= byteRead;
                memset(buffer, 0, BUFFER_SIZE);
            }
        }
        
        else if(choice == 5) //flush the pipe
        {
            char buffer[1];
            read(childToParent[READ_END], buffer, sizeof(buffer));
        }
        
        else if(choice == 6)
        {
            int killRes = kill(pid, SIGUSR1);
            break;
        }
    }
    close(parentToChild[WRITE_END]);
    close(childToParent[READ_END]);

    // Wait for the child process to exit
    int status;
    waitpid(pid, &status, 0);
}

void getInputForChoice(int choice, vector<string>& codeNames)
{
    switch(choice)
    {
        case 1: getInputFromUser(codeNames, "Insert airports ICOA code names to fetch their data:");
        break;
        case 2: getInputFromUser(codeNames, "Insert airports ICOA code names to print incoming flights:");
        break;
        case 3: getInputFromUser(codeNames, "Insert airports ICOA code names to print their full flight schedule:");
        break;
        case 4: getInputFromUser(codeNames,"Please enter icao24 codes of aircrafts, in order to see their schedule.");
        break;
    }
}

void printMenu()
{
    cout << "*************************************" << endl;
    cout << "1 - Fetch airports data." << endl;
    cout << "2 - Print airports incoming flights." << endl;
    cout << "3 - Print airports full flight schedule." << endl;
    cout << "4 - Print aircrafts full flight schedule." << endl;
    cout << "5 - Zip DB files." << endl;
    cout << "6 - Shutdown." << endl;
    cout << "Please make your choice <1, 2, 3, 4, 5, 6>:" << endl;
}

int getChoice()
{
    char choice;
    bool flag = true;
    printMenu();
    cin >> choice;
    cin.ignore(); // Ignore any leftover newline characters from previous input
    
    while(flag)
    {
        if(choice > '0' && choice < '7')
            return choice - '0';
        else
        {   
            cout << "Invalid choice, please choose again!" << endl;
            printMenu();
            cin >> choice;
            cin.ignore(); 
        }
    }
    return choice;

}

string getDataAndSendToParent(int choice,System& airports, vector<string> codeNames)
{
    string result;
    switch(choice)
    {
        case 1: fetchAirportsData(airports, codeNames);
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

void unzipDB(const string& zipFilePath, const string& destinationPath)
{
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

void handleSIGINT(int signalNumber)
{
    if (pid > 0)
    {
        cout << "You sent SIGINT signal... exiting gracefully :)" << endl;
        int killRes = kill(pid, SIGUSR1);
        exit(signalNumber);
    }
}

void handleSIGUSR1(int signalNumber)
{
    cout << "from child received SIGUSR1" << endl;
    gracefulExit(airports);
}
