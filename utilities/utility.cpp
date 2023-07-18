#include "utility.h"

#define nullNumFlag -1

//----------------------------------------Main Functions-----------------------------------------

int System::getAirportIndexByName(string& airportName)
{
    while(true)
    {
        int index = 0;
        for (auto& airport : airportsVector)
        {
            if (airport->getIcaoCode() == airportName)
                return index;
            else
                index++;
        }
        cout << "Airport name doesn't exist in the database." << endl;
        cout << "Please enter new name." << endl;
        cin >> airportName;
    }
}

void System::fetchDB(vector<string> airportsIcaoCodes)
{
    //vector<string> airportsNamesVector;
    //getAllAirportsNames(airportsNamesVector);
    string airportNames = "";
    for (auto& name : airportsIcaoCodes) 
        airportNames += name += " ";

    fs::path buildPath = fs::current_path();
    fs::path DB_path = fs::current_path().parent_path()/"DB";

    
    string s_DB_path = DB_path;
    string s_buildPath = buildPath;

    if (chdir(s_DB_path.c_str()) != 0) {
        cout << "Failed to change directory.\n";
    }
    string clean = "./clean.sh "; 
    string flightScanner = "./flightScanner.sh ";

    system("chmod u+x clean.sh");
    system("chmod u+x flightScanner.sh");

    //delete previous DB
    system((clean += airportNames).c_str());  /////if it doesn't exist??
    deleteAll();
    //create data base
    system((flightScanner += airportNames).c_str());

    
    if (chdir(buildPath.c_str()) != 0) {
        std::cout << "Failed to change directory.\n";
    }
    
    vector<string> paths;
    paths.reserve(10);
    getAllPaths(paths);
    load_db(paths);
    this->dbLoaded = true;   //if fetched DB and loaded to "airports" -> bool var will changed to know DB is loaded

}

void System::load_db(vector<string>& paths)
{
    //for each airport there are 2 paths (apt, dpt)
    int numberOfAirports = paths.size() / 2;

    for (int i = 0; i < numberOfAirports; i++)
    {
        SingleAirport* currentAirport = new SingleAirport(getAirportNameFromPath(paths[2*i]));

        //one of calls below is for .apt and the second is .dpt - unorginized order
        currentAirport->updateAirportDataFlights(paths[2 * i]);
        currentAirport->updateAirportDataFlights(paths[(2 * i) + 1]);

        airportsVector.push_back(currentAirport);
    }
}

vector<FlightInfo*> System::getFlightsByCallsign(string& callsign)
{    
    vector<FlightInfo*> flightsByCallsign;
    while(flightsByCallsign.size() == 0)
    {
        for (auto &airport : airportsVector)
        {
            // arrivals
            for (auto &flightInfo : airport->getArivals())
                if (flightInfo->getCallsign() == callsign)
                    flightsByCallsign.push_back(flightInfo);
            // departures
            for (auto &flightInfo : airport->getDepartures())
                if (flightInfo->getCallsign() == callsign)
                    flightsByCallsign.push_back(flightInfo);
        }
        
        if(flightsByCallsign.size() == 0)
        {
            cout << "Callsign not found!" << endl;
            cout << "Please enter new callsign.";
            cin >> callsign;
        }
    }
    return flightsByCallsign;
}

//--------------------------------------Helpers functions-----------------------------------------
void SingleAirport::updateAirportDataFlights(string& path)
{
    string currentLine;
    //Apt / Dpt
    string pathType = getPathType(path);
    ifstream currentFile(path);

    if (!currentFile.is_open())
    {
        cout << "wrong path was given";
    }

    // Skip the first line
    getline(currentFile, currentLine); 

    while (currentFile)
    {
        getline(currentFile, currentLine);
        char* currentLineAsChar = (char*)currentLine.c_str();
        FlightInfo* currentFlightInfo = currentFlightInfo->getCurrentFlightInfo(currentLineAsChar, pathType);
        currentFlightInfo->addFlightToAirport(*this);
    }
    currentFile.close();
}

 FlightInfo* FlightInfo::getCurrentFlightInfo(char* currentLine, string& pathType) 
 {
    const char delim[] = ", ";
    string icao24, estDepartureAirport, estArrivalAirport, callsign;
    int firstSeen, lastSeen;

    char* token = strtok(currentLine, delim);
    int countTokens = 0;

    //Taking only the six arguments from the file.
    while (token && countTokens < 6)
    {
        countTokens++;
        switch (countTokens)
        {
        case 1:
            icao24 = token;
            break;
        case 2:
            if (strcmp(token, "null") != 0)
                firstSeen = stoi(token);
            else
                firstSeen = nullNumFlag;
            break;
        case 3:
            estDepartureAirport = token;
            break;
        case 4:
            if (strcmp(token, "null") != 0)
                lastSeen = stoi(token);
            else
                lastSeen = nullNumFlag;
            break;
        case 5:
            estArrivalAirport = token;
            break;
        case 6:
            callsign = token;
            break;
        default:
            cout << "switch case overflow" << endl;
            break;
        }

        token = strtok(nullptr, delim);
    }
    if (pathType == "apt")
    {
        return  new FlightInfo('a', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    } else
    {
        return new FlightInfo('d', icao24, firstSeen, estDepartureAirport, lastSeen, estArrivalAirport, callsign);
    }
}

void System::getAllPaths(vector<string>& paths)
{
    fs::path parentPath = fs::current_path().parent_path();
    
    for (const auto& entry : fs::recursive_directory_iterator(parentPath))
    {
        if (fs::is_directory(entry.path()))
        {   // Skip directories
            continue;
        }
        
        if (entry.path().extension() == ".apt" || entry.path().extension() == ".dpt") 
                paths.push_back(entry.path().string());
    }
}

string System::getAirportNameFromPath(string& path)
{
    string airportName;
    size_t lastSlashPos = path.find_last_of("/");
    airportName = path.substr(lastSlashPos + 1);
    size_t dotPos = airportName.find_last_of(".");
    airportName = airportName.substr(0, dotPos);
    return airportName;
}

string System::getPathType(string& path)
{
    string pathType;
    size_t dotPos = path.find_last_of(".");
    pathType = path.substr(dotPos + 1, 3);
    return pathType;
}

void System::getAllAirportsNames(vector<string>& airportsNamesVector) { 
    
    
    for (auto& airport: airportsVector)
       airportsNamesVector.push_back(airport->getIcaoCode());
}


bool System::checkIfAllInDbAndUpdateMissing(vector<string> &missing_names, vector<string> codesRecievedArr)
{
    bool flag = true;
    for (auto& codeRecived: codesRecievedArr)
    {
        if (!isAirportExist(codeRecived))
        {
           flag = false;
           missing_names.push_back(codeRecived);
        }
    }
    return flag;
}


bool System::isAircraftInDB(string code)
{
    for (auto airport : airportsVector)
    {
        vector<FlightInfo*> arrivals = airport->getArivals();
        vector<FlightInfo*> departures = airport->getDepartures();
        for (auto flightInfo : arrivals)
            if (flightInfo->getIcao24() == code)   
                return true;
        for (auto flightInfo : departures)
            if (flightInfo->getIcao24() == code)   
                return true;
    }

    return false;
}
bool System::isAirportExist(string airportName)
{
    for (auto& airport: airportsVector)
        if (airport->getIcaoCode() == airportName)
            return true;
    return false;
}


void System::zipDirectory(const std::string& directoryPath, const std::string& zipFilePath)
{
    zip_t* archive = zip_open(zipFilePath.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
    if (archive == nullptr)
    {
        std::cerr << "Failed to open the ZIP file: " << zip_strerror(archive) << std::endl;
        return;
    }

    std::filesystem::path basePath(directoryPath);
    addFileToZip(archive, basePath, basePath);

    if (zip_close(archive) != 0)
    {
        std::cerr << "Failed to close the ZIP archive: " << zip_strerror(archive) << std::endl;
        return;
    }

    std::cout << "Successfully zipped the directory." << std::endl;
}

void System::addFileToZip(zip_t* archive, const std::filesystem::path& filePath, const std::filesystem::path& baseDirectory)
{
    std::filesystem::path relativePath = filePath.lexically_relative(baseDirectory);

    if (std::filesystem::is_directory(filePath))
    {
        zip_dir_add(archive, relativePath.string().c_str(), ZIP_FL_ENC_GUESS);

        for (const auto& entry : std::filesystem::directory_iterator(filePath))
        {
            addFileToZip(archive, entry.path(), baseDirectory);
        }
    }
    else
    {
        // std::string fileName = filePath.filename().string();
        // if (fileName != "clean.sh" && fileName != "flightScanner.sh")
        zip_source_t *source = zip_source_file(archive, filePath.string().c_str(), 0, 0);
        if (source == nullptr)
        {
            std::cerr << "Failed to open the file: " << filePath << std::endl;
            return;
        }

        zip_file_add(archive, relativePath.string().c_str(), source, ZIP_FL_ENC_GUESS);
        zip_source_keep(source);
    }
}