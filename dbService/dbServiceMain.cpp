#include "dbServiceFuncs.h"

int main()
{
    bool thereIsZipFile;
    unzipDB(thereIsZipFile);


    string namedPipeFlightsServiceToDbService, namedPipeDbServiceToFlightsService;
    createNamedPipes(namedPipeFlightsServiceToDbService,namedPipeDbServiceToFlightsService);
    
    int FileDescriptorFsToDb = open(namedPipeFlightsServiceToDbService.c_str(), O_RDWR);
    int FileDescriptorDbToFs = open(namedPipeDbServiceToFlightsService.c_str(), O_RDWR);

    runDbService(FileDescriptorFsToDb, FileDescriptorDbToFs, thereIsZipFile);

    closeAndUnlinkNamedPipes(FileDescriptorFsToDb, FileDescriptorDbToFs, 
        namedPipeFlightsServiceToDbService, namedPipeDbServiceToFlightsService);
   
    exit(0);
}

