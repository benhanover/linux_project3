#include "./missions.h"

string printAirportsArv(System& airports, vector<string> airportsCodeNames)
{
    vector<string> missing_names;
    string res;
    bool allInDB = false;
    int numOfCodes = airportsCodeNames.size();
    allInDB = airports.checkIfAllInDbAndUpdateMissing(missing_names, airportsCodeNames);
    
   if (!allInDB) 
    {
        res =  "Not all ICOA code names inserted exist in current database.\n";
        res += "These names doesn't exist in the database:\n";
        for (int i = 0; i < missing_names.size(); i++)
            res += missing_names[i] + ' ';
        res += '\n';
    }

    string curAirportName;
    for(int i = 0; i < numOfCodes; i++)
    {
        curAirportName = airportsCodeNames[i]; 
        res += printSingleAirportArv(airports, curAirportName);
        res += "\n";
    }
    return res;
}

string printSingleAirportArv(System& airports,string& IcoaCode)
{
    bool exist = airports.isAirportExist(IcoaCode);
    string res;

    if (exist)
    {
        int airportIndex = airports.getAirportIndexByName(IcoaCode);
        vector<SingleAirport*> airportsVector = airports.getAirportsVector();
        res = "Printing incoming flights for " + IcoaCode + ":\n";

        for (auto& flightInfo: airportsVector[airportIndex]->getArivals())
        {
            if(!flightInfo->getCallsign().empty())
                {
                    res += "Flight #" + flightInfo->getCallsign() + " arriving from " + flightInfo->getEstDepartureAirport() +
                   ", tookoff at " + flightInfo->getFirstSeen() +  " landed at " + flightInfo->getLastSeen() + "\n";
                }
        }
    }
    return res;
}



