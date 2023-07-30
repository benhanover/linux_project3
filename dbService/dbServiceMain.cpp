#include "dbServiceFuncs.h"

int main()
{
    bool zipContainedData;
    unzipDB(zipContainedData);

    cout << "in dbService" << endl;

    string namedPipeFlightsServiceToDbService, namedPipeDbServiceToFlightsService;
    createNamedPipes(namedPipeFlightsServiceToDbService,namedPipeDbServiceToFlightsService);
    
    int FileDescriptorFsToDb = open(namedPipeFlightsServiceToDbService.c_str(), O_RDWR);
    int FileDescriptorDbToFs = open(namedPipeDbServiceToFlightsService.c_str(), O_RDWR);

    runDbService(FileDescriptorFsToDb, FileDescriptorDbToFs, zipContainedData);

    closeAndUnlinkNamedPipes(FileDescriptorFsToDb, FileDescriptorDbToFs, 
        namedPipeFlightsServiceToDbService, namedPipeDbServiceToFlightsService);
   
    exit(0);
}

