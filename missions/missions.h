#pragma once
#include "../utilities/utility.h"
#include <sstream>
#include <sys/wait.h>

//--------------------------functions------------------------------
void getInputFromUser(vector<string>& words, string message);

//--------------------------mission 1------------------------------
string printSingleAirportArv(System& airports,string& IcoaCode);
string printAirportsArv(System& airports, vector<string> airportsCodeNames);

//--------------------------mission 2------------------------------
string printAirportSchedule(System& airports, vector<string> airportsNames);
string printFullAirportSchedule(string& IcoaCode, System& airports);
int compare2Flights(FlightInfo* f1, FlightInfo* f2);

//--------------------------mission 3------------------------------
string printAllAircraftsFlights(System& airports, vector<string> aircraftsNames);
string printSingleAircraftFlights(string& icao24, System& airports);
bool checkIfAllAircraftsInDB(System& airports, vector<string>& missing_names, vector<string> codesRecievedVec);

//--------------------------mission 4------------------------------
void refreshDataBase(System& airports);

//--------------------------mission 5------------------------------
void zipDataBase(System& airports);

//--------------------------mission 7------------------------------
void gracefulExit(System& airports);