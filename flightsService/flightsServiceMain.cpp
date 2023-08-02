#include "flightsServiceFuncs.h"

int main()
{
    string namedPipeFsToDbService ="/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    string namedPipeDbToFsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    // dbService creates the named pipes.

    int DataFileDescriptorFsToDb = open(namedPipeFsToDbService.c_str(), O_RDWR);
    int DataFileDescriptorDbToFs = open(namedPipeDbToFsService.c_str(), O_RDWR);

    runFlightsService(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs);

    closeAndUnlinkNamedPipes(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs,
      namedPipeFsToDbService, namedPipeDbToFsService);
    
    exit(0);
    
}

