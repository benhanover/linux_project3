#include "./missions.h"

//If the function fetched all wanted data it returns "true", otherwise it return "false"
bool fetchAirportsData(System& airports, vector<string> airportsCodeNames)
{
   bool fetchedAll = false;
   fetchedAll = airports.fetchDB(airportsCodeNames);
   return fetchedAll;
}