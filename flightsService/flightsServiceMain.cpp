#include "flightsServiceFuncs.h"

int main() {
    
    string namedPipeFlightsServiceToDbService ="/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    string namedPipeDbServiceToFlightsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    // dbService creates the named pipes.

    int FileDescriptorFsToDb = open(namedPipeFlightsServiceToDbService.c_str(), O_RDWR);
    int FileDescriptorDbToFs = open(namedPipeFlightsServiceToDbService.c_str(), O_RDWR);

    runFlightsService(FileDescriptorFsToDb, FileDescriptorDbToFs);

    closeAndUnlinkNamedPipes(FileDescriptorFsToDb, FileDescriptorDbToFs,
      namedPipeFlightsServiceToDbService, namedPipeDbServiceToFlightsService);
   
    exit(0);
    
}

