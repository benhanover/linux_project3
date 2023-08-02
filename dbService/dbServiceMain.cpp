#include "dbServiceFuncs.h"

int main()
{
   
    string namedPipeFsToDbService, namedPipeDbToFsService;
    createNamedPipes(namedPipeFsToDbService,namedPipeDbToFsService);
   
    int DataFileDescriptorFsToDb = open(namedPipeFsToDbService.c_str(), O_RDWR);
    int DataFileDescriptorDbToFs = open(namedPipeDbToFsService.c_str(), O_RDWR);


    /* string statusPipeFsToDb, statusPipeDbToFs;
    createStatusPipes(statusPipeFsToDb,statusPipeDbToFs);

    int StatusFileDescriptorFsToDb = open(statusPipeFsToDb.c_str(), O_RDWR);
    int StatusFileDescriptorDbToFs = open(statusPipeDbToFs.c_str(), O_RDWR);
 */

    bool thereIsZipFile;
    unzipDB(thereIsZipFile);


    runDbService(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs,
            /* StatusFileDescriptorFsToDb,StatusFileDescriptorDbToFs, */ thereIsZipFile);


    closeAndUnlinkNamedPipes(DataFileDescriptorFsToDb, DataFileDescriptorDbToFs, 
        namedPipeFsToDbService, namedPipeDbToFsService);

   /*  closeAndUnlinkStatusPipes(StatusFileDescriptorFsToDb, StatusFileDescriptorDbToFs, 
        statusPipeFsToDb, statusPipeDbToFs);
    */

    exit(0);
}

