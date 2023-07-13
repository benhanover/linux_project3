#include "./missions.h"

string printAllAircraftsFlights(System& airports, vector<string> aircraftsNames)
{
    vector<string> missing_names;
    string res;
    bool allInDB = false;
    int aircraftsNamesSize = aircraftsNames.size();
    allInDB = checkIfAllAircraftsInDB(airports, missing_names, aircraftsNames);

   if (!allInDB)
    {
        res =  "Not all ICOA code names inserted exist in current database.\n";
        res += "These names doesn't exist in the database:\n";
        for (int i = 0; i < missing_names.size(); i++)
            {
                res += missing_names[i] + ' ';
            }
        res += '\n';
    }
    
    string curAircraft;
    for(int i = 0; i < aircraftsNamesSize; i++)
    {
       curAircraft = aircraftsNames[i];
  
        if (find(missing_names.begin(), missing_names.end(), curAircraft) != missing_names.end())
            continue;      
       else 
        res += printSingleAircraftFlights(curAircraft,airports);
    }
    return res;
}

bool checkIfAllAircraftsInDB(System& airports, vector<string>& missing_names, vector<string> codesRecievedVec)
{
    int numOfCodesRecieved = codesRecievedVec.size();

    for (int i = 0; i < numOfCodesRecieved; i++)
    {   
        string aircraft = codesRecievedVec[i];
        bool existInDB = airports.isAircraftInDB(aircraft);
        if(!existInDB)
            missing_names.push_back(aircraft);
    }

    if (missing_names.empty())
        return true; //no missing names, all arguments in DB
    else 
        return false;
}

string printSingleAircraftFlights(string& icao24, System& airports)
{
    string aircraftName, res;
    vector<SingleAirport*> airportsVector = airports.getAirportsVector();

    res += "Printing schedule for " + icao24 + ":\n";

    for(auto& airport: airportsVector)
    {
        for (auto& flightInfo: airport->getArivals())
        {
            if(flightInfo->getAircraftName() == icao24)
            {
                res += icao24 + " departed from  " + flightInfo->getEstDepartureAirport() + " at " + flightInfo->getFirstSeen() +  " arrived in " 
                  + flightInfo->getEstArrivalAirport() + " at " + flightInfo->getLastSeen() + "\n";
            }
        }
        for (auto& flightInfo: airport->getDepartures())
        {
            if(flightInfo->getAircraftName() == icao24)
            {
                res += flightInfo->getAircraftName() + " departed from  " + flightInfo->getEstDepartureAirport() + " at " + flightInfo->getFirstSeen() +  " arrived in " 
                    + flightInfo->getEstArrivalAirport() + " at " + flightInfo->getLastSeen() + "\n";
            }
        }
    }
    res += "\n";
    return res;
}
