#pragma once
#include "../utilities/utility.h"
#include <sstream>
#include <sys/wait.h>

//--------------------------functions------------------------------
void getInputFromUser(vector<string>& words, string message);


//--------------------------mission 1------------------------------
void fetchAirportsData(System& airports, vector<string> airportsCodeNames);


//--------------------------mission 2------------------------------
string printSingleAirportArv(System& airports,string& IcoaCodes);
string printAirportsArv(System& airports, vector<string> airportsCodeNames);


//--------------------------mission 3------------------------------
string printAirportSchedule(System& airports, vector<string> airportsNames);
string printFullAirportSchedule(string& IcoaCode, System& airports);
int compare2Flights(FlightInfo* f1, FlightInfo* f2);


//--------------------------mission 4------------------------------
string printAllAircraftsFlights(System& airports, vector<string> aircraftsNames);
string printSingleAircraftFlights(string& icao24, System& airports);
bool checkIfAllAircraftsInDB(System& airports, vector<string>& missing_names, vector<string> codesRecievedVec);


//--------------------------mission 5------------------------------
void zipDataBase(System& airports);


//--------------------------mission 6------------------------------
void gracefulExit(System& airports);