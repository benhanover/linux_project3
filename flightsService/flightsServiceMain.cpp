#include "flightsServiceFuncs.h"

int main() {
    
    string namedPipeFsToDbService ="/tmp/flights_pipes/namedPipeFlightsServiceToDbService";
    string namedPipeDbToFsService = "/tmp/flights_pipes/namedPipeDbServiceToFlightsService";

    // dbService creates the named pipes.

    int DataFileDescriptorFsToDb = open(namedPipeFsToDbService.c_str(), O_RDWR);
    int DataFileDescriptorDbToFs = open(namedPipeDbToFsService.c_str(), O_RDWR);

/* 
    string statusPipeFsToDb = "/tmp/flights_pipes/statusPipeFlightsServiceToDbService";
    string statusPipeDbToFs = "/tmp/flights_pipes/statusPipeDbServiceToFlightsService";

    int StatusFileDescriptorFsToDb = open(statusPipeFsToDb.c_str(), O_RDWR);
    int StatusFileDescriptorDbToFs = open(statusPipeDbToFs.c_str(), O_RDWR);

 */
    runFlightsService(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs
      /*, StatusFileDescriptorFsToDb,StatusFileDescriptorDbToFs */);

    closeAndUnlinkNamedPipes(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs,
      namedPipeFsToDbService, namedPipeDbToFsService);
    /* 
    closeAndUnlinkStatusPipes(StatusFileDescriptorFsToDb, StatusFileDescriptorDbToFs, 
        statusPipeFsToDb, statusPipeDbToFs);
   */ 

    exit(0);
    
}

