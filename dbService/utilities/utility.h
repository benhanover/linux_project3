#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include <cstring>
#include <filesystem>
#include <algorithm>
#include <unistd.h>
#include <zip.h>
#include <sys/stat.h>

using namespace std;
namespace fs = std::filesystem;

class SingleAirport;
class FlightInfo;
class System
{
    vector<SingleAirport*> airportsVector;
    bool dbLoaded;  //indicates if there is database and it was loaded to "airports"

public:
    System()
    {
        airportsVector.reserve(10);
        dbLoaded = false;
    }
    ~System()
    {
       deleteAll(); 
    }    
    void addAirport(SingleAirport* airport)
    {
        airportsVector.push_back(airport);
    }

    void deleteAll()
    {
        for (auto &airport : airportsVector)
        {
            delete airport;
        }
        airportsVector.clear();
        
        dbLoaded = false;
    }
    vector<SingleAirport *> getAirportsVector() const { return airportsVector; }
    vector<FlightInfo *> getFlightsByCallsign(string &callsign);
    string getAirportNameFromPath(string &path);
    void getAllAirportsNames(vector<string> &airportNames);
    void getAllPaths(vector<string> &paths);
    void addFileToZip(zip_t *archive, const std::filesystem::path &filePath, const std::filesystem::path &baseDirectory);
    void zipDirectory(const std::string &directoryPath, const std::string &zipFilePath);
    void fetchDB(vector<string> airportsNamesVector, bool& fetchedAll);
    void load_db();
    bool checkIfAllInDB(vector<string> &paths, vector<string> &missing_names, int numOfCodesRecieved, vector<string> codesRecievedArr);
    bool checkIfAllInDbAndUpdateMissing(vector<string> &missing_names, vector<string> codesRecievedArr);
    bool isAircraftInDB(string code);
    bool isAirportExist(string airportName);
    int getAirportIndexByName(string& airportName);
    static string getPathType(string &path);
    bool isDataBaseLoaded() { return this->dbLoaded;}
};

class FlightInfo;
class SingleAirport
{
private:
    string icaoCode;
    vector<FlightInfo*> arrivals;
    vector<FlightInfo*> departures;

public:
    friend FlightInfo;
    SingleAirport() = default;
    SingleAirport(string _icaoCode) : icaoCode(_icaoCode)
    {
        arrivals.reserve(200);
        departures.reserve(200);
    }
    ~SingleAirport()
    {
        for(vector<FlightInfo*>::iterator it = arrivals.begin(); it != arrivals.end(); ++it)
          {
            delete *it;
          } 
        arrivals.clear();
        
        for(vector<FlightInfo*>::iterator it = departures.begin(); it != departures.end(); ++it)
        {
            delete *it;
        }
        departures.clear();
        cout << "delete single airport" << endl;
    }
    string getIcaoCode() const { return icaoCode;} 
    vector<FlightInfo*> getArivals() const { return arrivals; }
    vector<FlightInfo*> getDepartures() const { return departures; }

    void setIcaoCode(const string &airportName)
    {
        icaoCode = airportName;
    }
    void updateAirportDataFlights(string &path);

};

class FlightInfo
{
    char arvOrDpt;
    int firstSeen;
    int lastSeen;   
    string icao24; 
    string estDepartureAirport;          
    string estArrivalAirport;  
    string callsign;           

public:
    FlightInfo(char _arvOrDpt, string _icao24, int _firstSeen, string _estDepartureAirport, int _lastSeen,
               string _estArrivalAirport, string _callsign) 
    {
        arvOrDpt = _arvOrDpt;   
        icao24 = _icao24;
        firstSeen = _firstSeen;
        estDepartureAirport = _estDepartureAirport;
        lastSeen = _lastSeen;
        estArrivalAirport = _estArrivalAirport;
        callsign = _callsign;
    }
    char getArvOrDpt() const {return arvOrDpt; }
    FlightInfo* getCurrentFlightInfo(char* currentLine, string& pathType);
    string getIcao24() const {return icao24; }
    string getCallsign()const { return callsign; }
    string getEstArrivalAirport() const {return estArrivalAirport;}
    string getEstDepartureAirport() const {return estDepartureAirport;}
    string getFirstSeen() const { return to_string(firstSeen); }
    string getLastSeen() const { return to_string(lastSeen); }
    string getAircraftName() const{return icao24;}
    void addFlightToAirport(SingleAirport& airport)
    {
        if (getArvOrDpt() == 'a')
            airport.arrivals.push_back(this);
        else // arvOrDpt == 'd'
            airport.departures.push_back(this);
    }
};




