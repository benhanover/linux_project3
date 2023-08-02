# include "flightsServiceFuncs.h"


void runFlightsService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs)
{
    cout << "in runFlightsService" << endl;

    //string fsStartedOrKeepsRunning = flightsServiceStartedStr; //the first time it is defined, the program started or restarted
    string dbStarted; 
    
    bool gotShutDownOpcode = false;
    int vectorSize, choice = 0;

    cout << "flightsServise just started. Waiting for dbService." << endl;
    sendFsStartedSignalToDb(DataFileDescriptorFsToDb);
    getDbStatus(DataFileDescriptorDbToFs);

    while (!gotShutDownOpcode)  
    {    
        string codeNamesStr;
        string output;

        choice = getChoice();
            cout << "got choice and sending to dbService: " << choice << endl;
        
        if(choice >= 1 && choice <= 4)
                getInputForChoice(choice, codeNamesStr);
        
        writeInputToDbService(DataFileDescriptorFsToDb, choice, codeNamesStr);

        if (choice == SHUT_DOWN_CHOICE)
            gotShutDownOpcode = true;
        
        if (choice == 1)
        {
            cout << "Sent request to dbService. Waiting for response." << endl;
            cout << "It might take some time. Please be patient." << endl << endl;
        }      

        readOutputFromDbAndPrint(DataFileDescriptorDbToFs);
    
    }

    cout << "Shutting down flightsService." << endl;
}

     

void sendFsStartedSignalToDb(int DataFileDescriptorFsToDb)
{
            cout << "in sendFsStartedSignalToDb" << endl;

    int numSignal = NUM_SIGNAL_FS_STARTED;
    string emptyStr = "";
    writeInputToDbService(DataFileDescriptorFsToDb, numSignal, emptyStr);
    //write(DataFileDescriptorFsToDb, &numSignal, sizeof(numSignal));
}

void getDbStatus(int DataFileDescriptorDbToFs)
{
        cout << "in getDbStatus" << endl;

    readOutputFromDbAndPrint(DataFileDescriptorDbToFs);
}


void writeInputToDbService(int DataFileDescriptorFsToDb, int choice, string& codeNamesStr)
{
        cout << "in writeInputToDbService" << endl;
    string inputForDb;
    inputForDb = to_string(choice);
    if (choice >= 1 && choice <= 4)
        inputForDb += " " + codeNamesStr;

    int inputForDbSize = strlen(inputForDb.c_str()) + 1;
    write(DataFileDescriptorFsToDb, &inputForDbSize, sizeof(inputForDbSize));
    write(DataFileDescriptorFsToDb, inputForDb.c_str(), inputForDbSize);

}


void readOutputFromDbAndPrint(int DataFileDescriptorDbToFs)
{

    cout << "in readOutputFromDbAndPrint" << endl;
    

    //Read the output from dbService
    char buffer[BUFFER_SIZE];
    int resSize;
    read(DataFileDescriptorDbToFs, &resSize, sizeof(resSize));
    while(resSize > 0)
    {
        ssize_t byteRead = read(DataFileDescriptorDbToFs, buffer, SIZE_TO_READ);
        buffer[byteRead] = '\0';
        
        if (buffer == dbServiceStartedStr)
            {
                cout << "dbService just started." << endl;
                cout << "If you were waiting for response, the data from before may not be available." << endl;
                cout << "Please try Again." << endl;

            }
        else 
            cout << buffer; 

        resSize -= byteRead;
        memset(buffer, 0, BUFFER_SIZE);
    }

}

int getChoice()
{
    char choice;
    bool flag = true;
    printMenuToShowOptionalChoices();
    cin >> choice;
    cin.ignore(); // Ignore any leftover newline characters from previous input
    
    while(flag)
    {
        if(choice >= '1' && choice <= '6')
            return choice - '0';
        else
        {   
            cout << "Invalid choice, please choose again!" << endl;
            printMenuToShowOptionalChoices();
            cin >> choice;
            cin.ignore(); 
        }
    }
    return choice;

}

void printMenuToShowOptionalChoices()
{
    cout << endl << endl;
    cout << "*************************************" << endl;
    cout << "1 - Fetch airports data." << endl;
    cout << "2 - Print airports incoming flights." << endl;
    cout << "3 - Print airports full flight schedule." << endl;
    cout << "4 - Print aircrafts full flight schedule." << endl;
    cout << "5 - Zip DB files." << endl;
    cout << "6 - Shutdown." << endl;
    cout << "Please make your choice <1, 2, 3, 4, 5, 6>:" << endl;
}

void getInputForChoice(int choice, string& codeNamesStr)
{
    switch(choice)
    {
        case 1: getInputFromUser(codeNamesStr, "Insert airports ICOA code names to fetch their data:");
        break;
        case 2: getInputFromUser(codeNamesStr, "Insert airports ICOA code names to print incoming flights:");
        break;
        case 3: getInputFromUser(codeNamesStr, "Insert airports ICOA code names to print their full flight schedule:");
        break;
        case 4: getInputFromUser(codeNamesStr,"Please enter icao24 codes of aircrafts, in order to see their schedule.");
        break;
    }
}


//Takes a sentence from user, for example "this is a senetence",
//and break it down into array of words that combine the sentence: "this", "is" "a" "sentence"
void getInputFromUser(string& input, string message)
{
    cout << message << endl;
    //string line;
    getline(cin, input);

    /* istringstream iss(line);
    string code;
    while (iss >> code)
        words.push_back(code); */
}

void closeAndUnlinkNamedPipes(int FileDescriptorFsToDb, int FileDescriptorDbToFs, 
        string namedPipeFlightsServiceToDbService, string namedPipeDbServiceToFlightsService)
{
     // close write and read pipes which were in use
    close(FileDescriptorFsToDb);
    close(FileDescriptorDbToFs);

    unlink(namedPipeFlightsServiceToDbService.c_str());
    unlink(namedPipeDbServiceToFlightsService.c_str());
    
}
