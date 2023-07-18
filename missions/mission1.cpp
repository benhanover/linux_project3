#include "./missions.h"


void fetchAirportsData(System& airports, vector<string> airportsCodeNames)
{
   airports.fetchDB(airportsCodeNames);
}