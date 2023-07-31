# include "flightsServiceFuncs.h"


void runFlightsService(int FileDescriptorFsToDb,int FileDescriptorDbToFs)
{
    cout << "in runFlightsService" << endl;

    bool gotShutDownOpcode = false;
    int vectorSize, choice = 0;

    while (!gotShutDownOpcode)  
    {
        vector<string> codeNames;
        string output;

        choice = getChoice();
            cout << "got choice and sending to dbService: " << choice << endl;

        
        writeChoiceToDbService(FileDescriptorFsToDb, choice);

        if (choice == SHUT_DOWN_CHOICE) {
            gotShutDownOpcode = true;
        }
        else {

            if(choice >= 1 && choice <= 4)
            {
                getInputForChoice(choice, codeNames);
                
                writeUserInputToDb(FileDescriptorFsToDb, codeNames);
            }
            
            readOutputFromDbAndPrint(FileDescriptorDbToFs);
        }
       
    }
}

void writeChoiceToDbService(int FileDescriptorFsToDb, int choice)
{
        cout << "in writeChoiceToDbService" << endl;

    write(FileDescriptorFsToDb, &choice, sizeof(choice));
    usleep(10);

}

void writeUserInputToDb(int FileDescriptorFsToDb,vector<string>& codeNames)
{
        cout << "in writeUserInputToDb" << endl;


    int vectorSize = codeNames.size();
    write(FileDescriptorFsToDb, &vectorSize, sizeof(vectorSize));
    for (const auto& name : codeNames)
    {
        write(FileDescriptorFsToDb, name.c_str(), name.size() + 1);  // Include null terminator
                cout << "writing to DB: " << name << endl;

        usleep(10);
    }
}

void readOutputFromDbAndPrint(int FileDescriptorDbToFs)
{

        cout << "in readOutputFromDbAndPrint" << endl;


    //Read the output from dbService
    char buffer[BUFFER_SIZE];
    int resSize;
    read(FileDescriptorDbToFs, &resSize, sizeof(resSize));
    while(resSize > 0)
    {
        ssize_t byteRead = read(FileDescriptorDbToFs, buffer, SIZE_TO_READ);
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

//********************************change the name of the function  ??????????????  **********************************

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