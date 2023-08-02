# include "flightsServiceFuncs.h"


void runFlightsService(int DataFileDescriptorFsToDb,int DataFileDescriptorDbToFs)
{
    cout << "in runFlightsService" << endl;

    string fsStartedOrKeepsRunning = flightsServiceStartedStr; //the first time it is defined, the program started or restarted
    string dbStartedOrKeepsRunning; 
    
    bool gotShutDownOpcode = false;
    int vectorSize, choice = 0;

    while (!gotShutDownOpcode)  
    {    
        
        vector<string> codeNames;
        string output;

        choice = getChoice();
            cout << "got choice and sending to dbService: " << choice << endl;

        
        writeChoiceToDbService(DataFileDescriptorFsToDb, choice);

        if (choice == SHUT_DOWN_CHOICE) {
            gotShutDownOpcode = true;
        }
        else {

            if(choice >= 1 && choice <= 4)
            {
                getInputForChoice(choice, codeNames);
                
                writeUserInputToDb(DataFileDescriptorFsToDb, codeNames);

                if (choice == 1)
                    cout << "Sent request to dbService. Waiting for response." << endl;
                    cout << "It might take some time. Please be patient." << endl << endl;
            }
            
            readOutputFromDbAndPrint(DataFileDescriptorDbToFs);
        }
    
    }
}

void writeChoiceToDbService(int DataFileDescriptorFsToDb, int choice)
{
        cout << "in writeChoiceToDbService" << endl;

    write(DataFileDescriptorFsToDb, &choice, sizeof(choice));
    usleep(10);

}

void writeUserInputToDb(int DataFileDescriptorFsToDb,vector<string>& codeNames)
{
        cout << "in writeUserInputToDb" << endl;


    int vectorSize = codeNames.size();
    write(DataFileDescriptorFsToDb, &vectorSize, sizeof(vectorSize));
    for (const auto& name : codeNames)
    {
        write(DataFileDescriptorFsToDb, name.c_str(), name.size() + 1);  // Include null terminator
                cout << "writing to DB: " << name << endl;

        usleep(10);
    }
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


//Takes a sentence from user, for example "this is a senetence",
//and break it down into array of words that combine the sentence: "this", "is" "a" "sentence"
void getInputFromUser(vector<string>& words, string message)
{
    cout << message << endl;
    string line;
    getline(cin, line);

    istringstream iss(line);
    string code;
    while (iss >> code)
        words.push_back(code);
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
