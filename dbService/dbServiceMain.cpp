#include "dbServiceFuncs.h"

int main()
{
   
    string namedPipeFsToDbService, namedPipeDbToFsService;
    createNamedPipes(namedPipeFsToDbService,namedPipeDbToFsService);
   
    int DataFileDescriptorFsToDb = open(namedPipeFsToDbService.c_str(), O_RDWR);
    int DataFileDescriptorDbToFs = open(namedPipeDbToFsService.c_str(), O_RDWR);


    bool thereIsZipFile;
    unzipDB(thereIsZipFile);


    runDbService(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs, thereIsZipFile);


    closeAndUnlinkNamedPipes(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs, 
        namedPipeFsToDbService, namedPipeDbToFsService);

       exit(0);
}

