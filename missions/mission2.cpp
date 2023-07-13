#include "./missions.h"

string printAirportSchedule(System& airports, vector<string> airportsNames)
{
    vector<string> missing_names;
    string res;
    bool allInDB = false;
    int airportNamesSize = airportsNames.size();
    allInDB = airports.checkIfAllInDbAndUpdateMissing(missing_names, airportsNames);

   if (!allInDB)
    {
        res =  "Not all ICOA code names inserted exist in current database.\n";
        res += "These names doesn't exist in the database:\n";
        for (int i = 0; i < missing_names.size(); i++)
            res += missing_names[i] + ' ';
        res += '\n';
    }

    string curAirportName;
    
    for(int i = 0; i < airportNamesSize; i++)
    {
        curAirportName = airportsNames[i]; 
        bool airpotIsExist = airports.isAirportExist(curAirportName);
        if(airpotIsExist)
        {
            res += "Printing schedule for " + curAirportName + ":\n";
            res += printFullAirportSchedule(curAirportName,airports);
            res += "\n";
        }
    }
    return res;
}

string printFullAirportSchedule(string& IcoaCode, System& airports)
{
    string res;
    int index = airports.getAirportIndexByName(IcoaCode);
    vector<FlightInfo*> combine;
    vector<SingleAirport*> airportsVector = airports.getAirportsVector();

    combine.reserve(airportsVector[index]->getArivals().size() + airportsVector[index]->getDepartures().size());

    for(auto& flightInfo: airportsVector[index]->getArivals())
        combine.push_back(flightInfo);
        
    for(auto& flightInfo: airportsVector[index]->getDepartures())
        combine.push_back(flightInfo);

    sort(combine.begin(), combine.end(), compare2Flights);
    for (auto& flightInfo: combine)
    {
        if (flightInfo->getArvOrDpt() == 'a') 
            res += "Flight #" + flightInfo->getCallsign() + " arriving from " + flightInfo->getEstDepartureAirport() + " at " + flightInfo->getLastSeen() + "\n";
        else
            res += "Flight #" + flightInfo->getCallsign() + " departing to " + flightInfo->getEstDepartureAirport() + " at " + flightInfo->getFirstSeen() + "\n";
    }
    return res;
}

int compare2Flights(FlightInfo* f1, FlightInfo* f2)
{
    char f1ArvOrDpt = f1->getArvOrDpt();
    char f2ArvOrDpt = f2->getArvOrDpt();

    if (f1ArvOrDpt == 'a' && f2ArvOrDpt =='a')
        return (stoi(f1->getLastSeen()) < stoi(f2->getLastSeen())); //comparing 2 arrivals

    else if (f1ArvOrDpt == 'd' && f2ArvOrDpt =='d')
        return (stoi(f1->getFirstSeen()) < stoi(f2->getFirstSeen())); //comparing 2 departures
    
    else if (f1ArvOrDpt == 'a' && f2ArvOrDpt =='d')
        return (stoi(f1->getLastSeen()) < stoi(f2->getFirstSeen())); //comparing arrival(f1) and departure(f2)   
    
    else 
        return (stoi(f1->getFirstSeen()) < stoi(f2->getLastSeen())); //comparing departure(f1) and arrival(f2)
}